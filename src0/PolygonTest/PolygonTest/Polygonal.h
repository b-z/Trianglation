#pragma once

#include <iostream>
using namespace std;
#include <vector>

#define DOUBLE_PI 6.28318530717958647692
#define PI 3.14159265358979323846
#define HALF_PI 1.57079632679489661923


class Point
{
public:
    double x, y;
public:
    Point(void) :x(0.0), y(0.0) {}
    Point(double x_, double y_) :x(x_), y(y_) {}
};
typedef vector<Point> PointArray;

class Edge {
public:
    Edge(int p, int q) {
        p1 = p;
        p2 = q;
    }
    int p1; // point's ID in the polygon
    int p2;
};
typedef vector<Edge> EdgeArray;


class Polygonal;
typedef vector<int> IntArray;
typedef vector<IntArray> IntArray2;

class Ring
{
public:
    IntArray pointIDArray;
    int ringIDinRegion;
    int regionIDinPolygon;
    Polygonal* polygon;

public:
    Ring(void) :ringIDinRegion(0), regionIDinPolygon(0), polygon(NULL) {}
};
typedef vector<Ring> RingArray;

class Region
{
public:
    RingArray ringArray;
    int regionIDinPolygon;
    Polygonal* polygon;

public:
    Region(void) :regionIDinPolygon(0), polygon(NULL) {}
};
typedef vector<Region> RegionArray;

class Polygonal
{
public:
    PointArray pointArray;
    RegionArray regionArray;

public:
    void clear() {
        pointArray.clear();
        regionArray.clear();
    }
};

extern void     distanceMinPointRing(double&d, int& idRegion, int& idRing, Point& pt, Polygonal& pn);
extern void     distanceMinPointPolygon(double&d, int& id, Point& pt, Polygonal& pn);
extern double   distancePointPoint(Point& p1, Point& p2);
extern double   distancePointSegment(Point& pt, Point& p1, Point& p2);
extern void     getIntArrayPointInPolygon(IntArray& vi, Polygonal& pn, Point& p, double eT);
extern bool     findPointInRing(Polygonal& pn, int& idRegion, int& idRing, int& idPointInRing, int pointInPolygon);
extern void     insertPointInPolygon(Polygonal& pn, int& idRegion, int& idRing, int& idPointInRing, Point& newPoint);
extern void     intArrayInit(IntArray& vi, int data);
extern void     intArrayInitRing(IntArray& vi, Polygonal& pn, int idRgion, int idRing, double eT);
extern void     intArrayInitPoint(IntArray& vi, Polygonal& pn, int v, double eT);
extern void     intArrayInitPointSame(IntArray& vi, Polygonal& pn, double eT);
extern void     intArrayInitPolygon(IntArray& vi, Polygonal& pn);
extern void     intArrayInitPolygonSamePoint(IntArray& vr, Polygonal& pr, IntArray& vs, Polygonal& ps, double eT);
extern void     intArrayInitRegion(IntArray& vi, Polygonal& pn, int idRegion, double eT);
extern void     moveRing(Polygonal& pn, int idRegion, int idRing, double vx, double vy);
extern void     movePoint(Polygonal& pn, int id, double vx, double vy);
extern void     movePointIntArray(Polygonal& pn, IntArray& vi, double vx, double vy);
extern void     movePolygon(Polygonal& pn, double vx, double vy);
extern void     moveRegion(Polygonal& pn, int idRegion, double vx, double vy);
extern bool removeRegion(Polygonal& pn, int idRegion);
extern bool removePoint(Polygonal& pn, int id);
extern bool removeRing(Polygonal& pn, int idRegion, int idRing);
extern void subtractOneAboveID(Polygonal& pn, int id);
extern bool polygonNewInRingRegular(Polygonal& p, int idRegion, int n, double r, double cx, double cy);
extern void pointConvertFromScreenToGlobal(Point& result, Point pointScreen, double scale, Point translation, int screenX, int screenY);
extern void polygonNewOutRingRegular(Polygonal& p, int n, double r, double cx, double cy);
extern void pointConvertFromGlobalToScreen(Point& result, Point pointGlobal, double scale, Point translation, int screenX, int screenY);

extern Point operator +(Point a, Point b);
extern Point operator -(Point a, Point b);
extern double operator ^(Point a, Point b); // distance to
extern double operator >>(Point a, Point b); // angle to
extern double operator !(Point a); // argument