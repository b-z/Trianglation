//#include "stdafx.h"

#include "Triangle.h"
#include "Delaunay.h"

void TriangleMesh::buildTriangleMesh(Delaunay* d) {
    polygon = d->getPolygon();
    triangles = d->getResult();
}

