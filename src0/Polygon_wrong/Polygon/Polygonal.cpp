#include "stdafx.h"
#include "Polygonal.h"
#include <cmath>

void distanceMinPointRing(double&d, int& idRegion, int& idRing, Point& pt, Polygonal& pn)
{
    d = 0.0;
    idRegion = -1;
    idRing = -1;
    int nr = pn.regionArray.size();
    int i, j, k, nl, nv, v1, v2;
    double dt;
    for (i = 0; i < nr; i++)
    {
        nl = pn.regionArray[i].ringArray.size();
        for (j = 0; j < nl; j++)
        {
            nv = pn.regionArray[i].ringArray[j].pointIDArray.size();
            for (k = 0; k < nv; k++)
            {
                v1 = pn.regionArray[i].ringArray[j].pointIDArray[k];
                if (k == nv - 1)
                    v2 = pn.regionArray[i].ringArray[j].pointIDArray[0];
                else v2 = pn.regionArray[i].ringArray[j].pointIDArray[k + 1];
                dt = distancePointSegment(pt, pn.pointArray[v1], pn.pointArray[v2]);
                if ((idRing == -1) || (d>dt))
                {
                    d = dt;
                    idRegion = i;
                    idRing = j;
                }
            }
        }
    }
}
void distanceMinPointPolygon(double&d, int& id, Point& pt, Polygonal& pn)
{
    d = 0.0;
    id = -1;
    int n = pn.pointArray.size();
    if (n <= 0)
        return;
    d = distancePointPoint(pt, pn.pointArray[0]);
    id = 0;
    int i;
    double dt;
    for (i = 1; i < n; i++)
    {
        dt = distancePointPoint(pt, pn.pointArray[i]);
        if (dt < d)
        {
            d = dt;
            id = i;
        }
    }
}
double distancePointPoint(Point& p1, Point& p2)
{
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    double d2 = dx*dx + dy*dy;
    double d = sqrt(d2);
    return d;
}
double distancePointSegment(Point& pt, Point& p1, Point& p2)
{
    double dx0 = p2.x - p1.x;
    double dy0 = p2.y - p1.y;
    double dx1 = pt.x - p1.x;
    double dy1 = pt.y - p1.y;
    double dx2 = pt.x - p2.x;
    double dy2 = pt.y - p2.y;
    double d1 = dx1*dx1 + dy1*dy1;
    double d2 = dx2*dx2 + dy2*dy2;
    double d01 = dx1*dx0 + dy1*dy0;
    double d02 = -dx2*dx0 - dy2*dy0;
    double d, d0;
    if ((d01 > 0) && (d02 > 0))
    {
        d0 = dx0*dx0 + dy0*dy0;
        d = d01*d01 / d0;
        d = d1 - d;
        d = sqrt(d);
        return d;
    }
    if (d1 > d2)
        d = d2;
    else d = d1;
    d = sqrt(d);
    return d;
}
void getIntArrayPointInPolygon(IntArray& vi, Polygonal& pn, Point& p, double eT)
{
    int i, n;
    double d;
    n = pn.pointArray.size();
    for (i = 0; i < n; i++)
    {
        d = distancePointPoint(p, pn.pointArray[i]);
        if (d <= eT)
        {
            vi[i] = i;
        }
    }
}
bool findPointInRing(Polygonal& pn, int& idRegion, int& idRing, int& idPointInRing, int pointInPolygon)
{
    idRegion = 0;
    idRing = 0;
    idPointInRing = 0;
    int nr, nL, nv;
    int i, j, k;
    nr = pn.regionArray.size();
    for (i = 0; i < nr; i++)
    {
        nL = pn.regionArray[i].ringArray.size();
        for (j = 0; j < nL; j++)
        {
            nv = pn.regionArray[i].ringArray[j].pointIDArray.size();
            for (k = 0; k < nv; k++)
            {
                if (pn.regionArray[i].ringArray[j].pointIDArray[k] == pointInPolygon)
                {
                    idRegion = i;
                    idRing = j;
                    idPointInRing = k;
                    return true;
                }
            }
        }
    }
    return false;
}

void insertPointInPolygon(Polygonal& pn, int& idRegion, int& idRing, int& idPointInRing, Point& newPoint)
{
    int nv = pn.pointArray.size();
    pn.pointArray.push_back(newPoint);
    pn.regionArray[idRegion].ringArray[idRing].pointIDArray.insert(
        pn.regionArray[idRegion].ringArray[idRing].pointIDArray.begin() + idPointInRing + 1,
        nv);
}
void intArrayInit(IntArray& vi, int data)
{
    int n = vi.size();
    int i;
    for (i = 0; i < n; i++)
        vi[i] = data;
}
void intArrayInitRing(IntArray& vi, Polygonal& pn, int idRegion, int idRing, double eT)
{
    int i, v;
    int n = pn.pointArray.size();
    vi.resize(n);
    n = pn.regionArray[idRegion].ringArray[idRing].pointIDArray.size();
    for (i = 0; i < n; i++)
    {
        v = pn.regionArray[idRegion].ringArray[idRing].pointIDArray[i];
        vi[v] = v;
    }
    intArrayInitPointSame(vi, pn, eT);
}
void intArrayInitPoint(IntArray& vi, Polygonal& pn, int v, double eT)
{
    int n = pn.pointArray.size();
    if (n <= 0)
    {
        vi.clear();
        return;
    }
    vi.resize(n);
    int i;
    double d;
    for (i = 0; i < n; i++)
    {
        if (i == v)
            vi[i] = i;
        else
        {
            d = distancePointPoint(pn.pointArray[i], pn.pointArray[v]);
            if (d <= eT)
                vi[i] = i;
            else vi[i] = -1;
        }
    }
}
void intArrayInitPointSame(IntArray& vi, Polygonal& pn, double eT)
{
    int i, j, n;
    double d;
    n = vi.size();
    if (n <= 0)
        return;
    for (i = 0; i < n; i++)
    {
        if (vi[i] >= 0)
        {
            for (j = 0; j < n; j++)
            {
                if (vi[j] < 0)
                {
                    d = distancePointPoint(pn.pointArray[i], pn.pointArray[j]);
                    if (d <= eT)
                        vi[j] = j;
                }
            }
        }
    }
}
void intArrayInitPolygon(IntArray& vi, Polygonal& pn)
{
    int i;
    int n = pn.pointArray.size();
    vi.resize(n);
    for (i = 0; i < n; i++)
        vi[i] = i;
}
void intArrayInitPolygonSamePoint(IntArray& vr, Polygonal& pr, IntArray& vs, Polygonal& ps, double eT)
{
    int i, j;
    int n0, n1;
    double da;
    n1 = pr.pointArray.size();
    if (n1 <= 0)
    {
        vr.clear();
        return;
    }
    vr.resize(n1);
    intArrayInit(vr, -1);
    n0 = ps.pointArray.size();
    for (i = 0; i < n0; i++)
    {
        if (vs[i] < 0)
            continue;
        for (j = 0; j < n1; j++)
        {
            if (vr[j] < 0)
            {
                da = distancePointPoint(ps.pointArray[i], pr.pointArray[j]);
                if (da <= eT)
                    vr[j] = j;
            }
        }
    }
}
void intArrayInitRegion(IntArray& vi, Polygonal& pn, int idRegion, double eT)
{
    int i, j, nr, v;
    int n = pn.pointArray.size();
    vi.resize(n);
    nr = pn.regionArray[idRegion].ringArray.size();
    for (i = 0; i < nr; i++)
    {
        n = pn.regionArray[idRegion].ringArray[i].pointIDArray.size();
        for (j = 0; j < n; j++)
        {
            v = pn.regionArray[idRegion].ringArray[i].pointIDArray[j];
            vi[v] = v;
        }
    }
    intArrayInitPointSame(vi, pn, eT);
}
void moveRing(Polygonal& pn, int idRegion, int idRing, double vx, double vy)
{
    int nr, nL, nv;
    int i, id;
    nr = pn.regionArray.size();
    if ((idRegion < 0) || (idRegion >= nr))
        return;
    nL = pn.regionArray[idRegion].ringArray.size();
    if ((idRing < 0) || (idRing >= nL))
        return;
    nv = pn.regionArray[idRegion].ringArray[idRing].pointIDArray.size();
    for (i = 0; i < nv; i++)
    {
        id = pn.regionArray[idRegion].ringArray[idRing].pointIDArray[i];
        pn.pointArray[id].x += vx;
        pn.pointArray[id].y += vy;
    }
}
void movePoint(Polygonal& pn, int id, double vx, double vy)
{
    int n = pn.pointArray.size();
    if ((id < 0) || (id >= n))
        return;
    pn.pointArray[id].x += vx;
    pn.pointArray[id].y += vy;
}

void movePointIntArray(Polygonal& pn, IntArray& vi, double vx, double vy)
{
    int n = vi.size();
    int i;
    for (i = 0; i < n; i++)
        movePoint(pn, vi[i], vx, vy);
}
void movePolygon(Polygonal& pn, double vx, double vy)
{
    int n = pn.pointArray.size();
    int i;
    for (i = 0; i < n; i++)
    {
        pn.pointArray[i].x += vx;
        pn.pointArray[i].y += vy;
    }
}
void moveRegion(Polygonal& pn, int idRegion, double vx, double vy)
{
    int nr, nL, nv;
    int i, j, k, id;
    nr = pn.regionArray.size();
    if ((idRegion < 0) || (idRegion >= nr))
        return;
    i = idRegion;
    nL = pn.regionArray[i].ringArray.size();
    for (j = 0; j < nL; j++)
    {
        nv = pn.regionArray[i].ringArray[j].pointIDArray.size();
        for (k = 0; k < nv; k++)
        {
            id = pn.regionArray[i].ringArray[j].pointIDArray[k];
            pn.pointArray[id].x += vx;
            pn.pointArray[id].y += vy;
        }
    }
}
/* 将在全局坐标系下的点转换成为在屏幕坐标下的点
 result:      输出的在屏幕坐标下的点;
 pointGlobal: 输入的在全局坐标系下的点;
 scale:       输入的比例因子;
 translation: 输入的平移坐标值。*/
void pointConvertFromGlobalToScreen(Point& result, Point pointGlobal, double scale, Point translation, int screenX, int screenY)
{
    result.x = (pointGlobal.x - translation.x)*scale;
    result.y = (pointGlobal.y - translation.y)*scale;
    result.x += (screenX / 2);
    result.y = screenY / 2 - result.y;
}
/* 将在屏幕坐标下的点转换成为在全局坐标系下的点
 result:      输出的在全局坐标系下的点;
 pointScreen: 输入的在屏幕坐标系下的点;
 scale:       输入的比例因子;
 translation: 输入的平移坐标值。*/
void pointConvertFromScreenToGlobal(Point& result, Point pointScreen, double scale, Point translation, int screenX, int screenY)
{
    result.x = pointScreen.x - screenX / 2;
    result.y = screenY / 2 - pointScreen.y;
    result.x = result.x / scale + translation.x;
    result.y = result.y / scale + translation.y;
}
/* 给多边形p增加新的内环，该内环是外接圆半径为r的正n边形。*/
bool polygonNewInRingRegular(Polygonal& p, int idRegion, int n, double r, double cx, double cy)
{
    if (n < 3)
        return false;
    int nr = p.regionArray.size();
    if ((idRegion < 0) || (idRegion >= nr))
        return false;
    int nL = p.regionArray[idRegion].ringArray.size();
    if (nL <= 0)
        return false;
    p.regionArray[idRegion].ringArray.resize(nL + 1);
    int s = p.pointArray.size();
    int t = s + n;
    int i, k;
    p.pointArray.resize(t);
    double da = DOUBLE_PI / n;
    double d = 0.0;
    for (i = s; i < t; i++, d += da)
    {
        p.pointArray[i].x = cx + r*cos(d);
        p.pointArray[i].y = cy + r*sin(d);
    }
    p.regionArray[idRegion].ringArray[nL].polygon = &p;
    p.regionArray[idRegion].ringArray[nL].regionIDinPolygon = idRegion;
    p.regionArray[idRegion].ringArray[nL].ringIDinRegion = nL;
    p.regionArray[idRegion].ringArray[nL].pointIDArray.resize(n);
    for (i = 0, k = t - 1; i < n; i++, k--)
    {
        p.regionArray[idRegion].ringArray[nL].pointIDArray[i] = k;
    }
    return true;
}
/* 给多边形p增加新的外环，该外环是外接圆半径为r的正n边形。*/
void polygonNewOutRingRegular(Polygonal& p, int n, double r, double cx, double cy)
{
    if (n < 3)
        return;
    int s = p.pointArray.size();
    int t = s + n;
    int i, k;
    p.pointArray.resize(t);
    double da = DOUBLE_PI / n;
    double d = 0.0;
    for (i = s; i < t; i++, d += da)
    {
        p.pointArray[i].x = cx + r*cos(d);
        p.pointArray[i].y = cy + r*sin(d);
    }
    int rs = p.regionArray.size();
    p.regionArray.resize(rs + 1);
    p.regionArray[rs].polygon = &p;
    p.regionArray[rs].regionIDinPolygon = rs;
    p.regionArray[rs].ringArray.resize(1);
    p.regionArray[rs].ringArray[0].polygon = &p;
    p.regionArray[rs].ringArray[0].regionIDinPolygon = rs;
    p.regionArray[rs].ringArray[0].ringIDinRegion = 0;
    p.regionArray[rs].ringArray[0].pointIDArray.resize(n);
    for (i = 0, k = s; i < n; i++, k++)
    {
        p.regionArray[rs].ringArray[0].pointIDArray[i] = k;
    }
}
bool removeRing(Polygonal& pn, int idRegion, int idRing)
{
    int nL, nLv, iLv, v;
    nL = pn.regionArray[idRegion].ringArray.size();
    if ((idRing == 0) || (nL < 2))
        return removeRegion(pn, idRegion);
    nLv = pn.regionArray[idRegion].ringArray[idRing].pointIDArray.size();
    for (iLv = 0; iLv < nLv; iLv++)
    {
        v = pn.regionArray[idRegion].ringArray[idRing].pointIDArray[iLv];
        pn.pointArray.erase(pn.pointArray.begin() + v);
        subtractOneAboveID(pn, v);
    }
    pn.regionArray[idRegion].ringArray.erase(
        pn.regionArray[idRegion].ringArray.begin() + idRing);
    return true;
}
bool removePoint(Polygonal& pn, int id)
{
    int ir, iL, iLv, nLv;
    bool rf = findPointInRing(pn, ir, iL, iLv, id);
    if (!rf)
        return false;
    nLv = pn.regionArray[ir].ringArray[iL].pointIDArray.size();
    if (nLv < 4)
        return (removeRing(pn, ir, iL));
    pn.regionArray[ir].ringArray[iL].pointIDArray.erase(
        pn.regionArray[ir].ringArray[iL].pointIDArray.begin() + iLv);
    pn.pointArray.erase(pn.pointArray.begin() + id);
    subtractOneAboveID(pn, id);
    return true;
}
bool removeRegion(Polygonal& pn, int idRegion)
{
    int nr, nL, nLv, iL, iLv, v;
    nr = pn.regionArray.size();
    if (nr < 2)
    {
        pn.clear();
        return true;
    }
    nL = pn.regionArray[idRegion].ringArray.size();
    for (iL = 0; iL < nL; iL++)
    {
        nLv = pn.regionArray[idRegion].ringArray[iL].pointIDArray.size();
        for (iLv = 0; iLv < nLv; iLv++)
        {
            v = pn.regionArray[idRegion].ringArray[iL].pointIDArray[iLv];
            pn.pointArray.erase(pn.pointArray.begin() + v);
            subtractOneAboveID(pn, v);
        }
    }
    pn.regionArray.erase(pn.regionArray.begin() + idRegion);
    return true;
}
void subtractOneAboveID(Polygonal& pn, int id)
{
    int nr = pn.regionArray.size();
    int nL, nLv, ir, iL, iLv;
    for (ir = 0; ir < nr; ir++)
    {
        nL = pn.regionArray[ir].ringArray.size();
        for (iL = 0; iL < nL; iL++)
        {
            nLv = pn.regionArray[ir].ringArray[iL].pointIDArray.size();
            for (iLv = 0; iLv < nLv; iLv++)
            {
                if (pn.regionArray[ir].ringArray[iL].pointIDArray[iLv] >= id)
                    pn.regionArray[ir].ringArray[iL].pointIDArray[iLv]--;
            }
        }
    }
}

Point operator +(Point a, Point b) {
    return Point(a.x + b.x, a.y + b.y);
}

Point operator -(Point a, Point b) {
    return Point(a.x - b.x, a.y - b.y);
}

double operator ^(Point a, Point b) { // distance to
    return sqrt((a.y - b.y) * (a.y - b.y) + (a.x - b.x) * (a.x - b.x));
}

double operator !(Point a) { // argument
    if (a.y != 0) {
        return 2 * atan(a.y / (a.x + (a ^ Point(0, 0))));
    }
    return a.x > 0 ? 0 : PI;
}

double operator >>(Point a, Point b) { // angle to
    double r = !b - !a;
    if (r <= -PI)
        r += 2 * PI;
    else if (r > PI)
        r -= 2 * PI;
    return r;
}