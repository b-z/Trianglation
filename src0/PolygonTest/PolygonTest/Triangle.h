#pragma once

#include <iostream>
using namespace std;
#include <vector>
#include <cmath>

#include "Polygonal.h"
class Delaunay;
class Triangle {
public:
    int vertices[3];
    int color[3];
    int fill;
public:
    Triangle() {
        Triangle(0, 0, 0);
    }
    Triangle(int a, int b, int c) {
        vertices[0] = a;
        vertices[1] = b;
        vertices[2] = c;
        fill = rand() % 6;
        color[0] = rand() % 256;
        color[1] = rand() % 256;
        color[2] = rand() % 256;
    }
};

typedef vector<Triangle> TriangleArray;

class TriangleMesh {
public:
    TriangleArray triangles;
    Polygonal *polygon;
public:
    TriangleMesh() {
        clear();
    }
    void clear() {
        polygon = NULL;
        triangles.clear();
    }
    void buildTriangleMesh(Delaunay* d);
};


