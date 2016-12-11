#pragma once

#include "Polygonal.h"
#include "Triangle.h"
#include <string>

#define REWRITE
#define CORRECT

class Delaunay
{
public:
    Delaunay(void) {
    }
    Delaunay(Polygonal *p) {
        setPolygon(p);
    }
    ~Delaunay() {
    }
    void setPolygon(Polygonal *p) {
        polygon = p;
    }
    Polygonal *getPolygon() {
        return polygon;
    }
    vector<Triangle> getResult() {
        return triangles;
    }
    void computeTriangulation();
private:
    void computeEdges();
#ifndef REWRITE
    void stage1();
    void stage2();
    void stage3();
    void stage4();
    void stage5();
    int getMinimumCirclePointIDFromCandidates(int p1, int p2);
#else

#endif
#ifdef CORRECT
    bool isPointInCircumscribedCircle(Point p, Point p1, Point p2, Point p3);
    double det(double a, double b, double c, double d, double e, double f, double g, double h, double i);
#endif
    bool isPointOnTheLeftOfVector(int p, Edge e);
    bool isOtherEdgesIntersectWithVectors(Edge e1, Edge e2);
    bool isOtherPointsInTriangle(int p1, int p2, int p3);
    bool isIn(Point p, Point p1, Point p2, Point p3);
    int isPolygonEdge(int p1, int p2);
    void removeEdge(int idx);
    bool isIntersection(Edge e1, Edge e2);
    double mult(Point a, Point b, Point c);
    double getCircumscribedCircleRadius(Point a, Point b, Point c);
    void reduceTriangle(int p);
private:
    Polygonal *polygon;
    TriangleArray triangles;
    EdgeArray edges;
    int N;
#ifndef REWRITE
    int K;
    IntArray candidates;
#endif
public:
    int count;
};
