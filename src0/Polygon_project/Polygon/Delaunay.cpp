#include "stdafx.h"
#include "Delaunay.h"
#include <cmath>
#include <iostream>
#define P(a) polygon->pointArray[(a)]

void Delaunay::computeEdges() {
    edges.clear();
    for (unsigned int i = 0; i < polygon->regionArray.size(); i++) {
        for (unsigned int j = 0; j < polygon->regionArray[i].ringArray.size(); j++) {
            for (unsigned int k = 0; k < polygon->regionArray[i].ringArray[j].pointIDArray.size(); k++) {
                IntArray pa = polygon->regionArray[i].ringArray[j].pointIDArray;
                int u1 = k;
                int u2 = k + 1 == pa.size() ? 0 : k + 1;
                Edge edge(pa[u1], pa[u2]);
                edges.push_back(edge);
            }
        }
    }
}

void Delaunay::computeTriangulation() {
    triangles.clear();
    computeEdges();
    N = edges.size();
    if (N < 3) {
        return;
    }
    K = 0;
    stage1();
}

void Delaunay::stage1() {
    candidates.clear();
    if (N == 3) {
        Triangle t(edges[0].p1, edges[1].p1, edges[2].p1);
        triangles.push_back(t);
        return;
    }
    K = 0;
    stage2();
}

void Delaunay::stage2() {
    K++;
    if (K == N) {
        stage5();
    } else if (isPointOnTheLeftOfVector(edges[K].p2, edges[0])) {
        stage3();
    } else {
        stage2();
    }
}

void Delaunay::stage3() {
    debug+="3 ";
    Edge e1(edges[0].p1, edges[K].p2);
    Edge e2(edges[0].p2, edges[K].p2);
    if (isOtherEdgesIntersectWithVectors(e1, e2) || isOtherPointsInTriangle(e1.p1, e2.p1, e2.p2)) {
        stage2();
    } else {
        stage4();
    }
}

void Delaunay::stage4() {
    debug+="4 ";
    candidates.push_back(edges[K].p2);
    if (K == N - 1) {
        stage5();
    } else {
        stage2();
    }
}

void Delaunay::stage5() {
    int o = getMinimumCirclePointIDFromCandidates(edges[0].p1, edges[0].p2);

    Triangle t(edges[0].p1, edges[0].p2, o);
    triangles.push_back(t);
    
    int m = isPolygonEdge(edges[0].p1, o);
    int n = isPolygonEdge(edges[0].p2, o);

    // 5.1
    if (m == -1 && n == -1) {
        debug+="51 ";

        N++;
        Edge e(o, edges[0].p2);
        // edges.push_back(e);
        edges.insert(edges.begin() + 1, e);
        edges[0].p2 = o;
    }

    // 5.2
    if (m != -1 && n == -1) {
        debug+="52 ";

        N--;
        edges[0].p1 = o;
        edges[m].p1 = edges[N].p1;
        edges[m].p2 = edges[N].p2;
        removeEdge(N);
    }

    if (m == -1 && n != -1) {
        debug+="53 ";

        N--;
        edges[0].p2 = o;
        edges[n].p1 = edges[N].p1;
        edges[n].p2 = edges[N].p2;
        removeEdge(N);
    }

    // 5.3
    if (m != -1 && n != -1) {
        debug+="54 ";

        removeEdge(max(n, m));
        removeEdge(min(n, m));
        removeEdge(0);
        N -= 3;
    }

    stage1();
}

bool Delaunay::isPointOnTheLeftOfVector(int p, Edge e) {
    Point p1 = P(p) - P(e.p1);
    Point p2 = P(e.p2) - P(e.p1);
    return p1.y * p2.x > p1.x * p2.y;
}

bool Delaunay::isIntersection(Edge e1, Edge e2) {
    Point p11 = P(e1.p1);
    Point p12 = P(e1.p2);
    Point p21 = P(e2.p1);
    Point p22 = P(e2.p2);
    
	return !(
        max(p11.x, p12.x) < min(p21.x, p22.x) ||
		max(p11.y, p12.y) < min(p21.y, p22.y) ||
		max(p21.x, p22.x) < min(p11.x, p12.x) ||
		max(p21.y, p22.y) < min(p11.y, p12.y) ||
		mult(p21, p12, p11) * mult(p12, p22, p11) < 0 ||
		mult(p11, p22, p21) * mult(p22, p12, p21) < 0
    );	
}

double Delaunay::mult(Point a, Point b, Point c) {
    return (a.x - c.x) * (b.y - c.y) - (b.x - c.x) * (a.y - c.y);
}


bool Delaunay::isOtherPointsInTriangle(int p1, int p2, int p3) {
    for (unsigned int i = 0; i < polygon->pointArray.size(); i++) {
        if (i == p1 ||
            i == p2 ||
            i == p3) {
            continue;
        }
        if (isIn(P(i), P(p1), P(p2), P(p3))) {
            return true;
        }
    }
    return false;
}

bool Delaunay::isIn(Point p, Point p1, Point p2, Point p3) {
    Point u1 = p1 - p;
    Point u2 = p2 - p;
    Point u3 = p3 - p;
    if ((p ^ p1) == 0 || (p ^ p2) == 0 || (p ^ p3) == 0) {
        return false;
    }
    double sum = (u1 >> u2) + (u2 >> u3) + (u3 >> u1);
    if (abs(sum) < 1e-5) {
        return false;
    }
    return true;
}

bool Delaunay::isOtherEdgesIntersectWithVectors(Edge e1, Edge e2) {
    for (unsigned int i = 0; i < edges.size(); i++) {
        if (!(edges[i].p1 == e1.p1 ||
            edges[i].p1 == e1.p2 ||
            edges[i].p2 == e1.p1 ||
            edges[i].p2 == e1.p2) &&
            isIntersection(edges[i], e1)
            ) {
            return true;
        }
        if (!(edges[i].p1 == e2.p1 ||
            edges[i].p1 == e2.p2 ||
            edges[i].p2 == e2.p1 ||
            edges[i].p2 == e2.p2) &&
            isIntersection(edges[i], e2)
            ) {
            return true;
        }
    }
    return false;
}

int Delaunay::getMinimumCirclePointIDFromCandidates(int p1, int p2) {
    double m = DBL_MAX;
    int p = -1;
    // assert(candidates.size() > 0);
    if (candidates.size() == 0) {
        throw -1;
    }
    for (unsigned int i = 0; i < candidates.size(); i++) {
        double r = getCircumscribedCircleRadius(P(p1), P(p2), P(candidates[i]));
        if (r < m) {
            m = r;
            p = candidates[i];
        }
    }
    ASSERT(p >= 0);
    return p;
}

double Delaunay::getCircumscribedCircleRadius(Point A, Point B, Point C) {
    double a = B ^ C;
    double b = C ^ A;
    double c = A ^ B;
    double p = (a + b + c) * (a + b - c) * (b + c - a) * (c + a - b);
    if (p <= 0) {
        p = DBL_EPSILON;
    }
    return a * b * c / sqrt(p);
}

int Delaunay::isPolygonEdge(int p1, int p2) {
    for (unsigned int i = 0; i < edges.size(); i++) {
        int u1 = edges[i].p1;
        int u2 = edges[i].p2;
        if (u1 == p1 && u2 == p2 || u1 == p2 && u2 == p1) {
            return i;
        }
    }
    return -1;
}

void Delaunay::removeEdge(int idx) {
    edges.erase(edges.begin() + idx);
}
