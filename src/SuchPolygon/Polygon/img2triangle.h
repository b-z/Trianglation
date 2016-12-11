#pragma once

#include <opencv2\opencv.hpp>
#include <string>
#include "Polygonal.h"
#include "Delaunay.h"
#define P(a) polygon->pointArray[(a)]
class Img2triangle {
public:
    Img2triangle(std::string path) {
        img = cv::imread(path);
        points.clear();
        /*polygon = new Polygonal;
        polygon->clear();*/
    }
    void gen() {
        int w = img.cols;
        int h = img.rows;
        int r = 3;
        for (int i = w/2; i < w-3; i+=w/15) {
            for (int j = h/2; j < h-3; j+=h/15) {
                //if (rand() % 1000 < 2) {
                points.push_back(imageCoord2ScreenCoord(Point(i+rand()%(r*2+1)-r, j+rand()%(r*2+1)-r)));
                if (i!=w/2 && j!=h/2) points.push_back(imageCoord2ScreenCoord(Point(w-i+rand()%(r*2+1)-r, h-j+rand()%(r*2+1)-r)));
                if (j!=h/2) points.push_back(imageCoord2ScreenCoord(Point(i+rand()%(r*2+1)-r, h-j+rand()%(r*2+1)-r)));
                if (i!=w/2) points.push_back(imageCoord2ScreenCoord(Point(w-i+rand()%(r*2+1)-r, j+rand()%(r*2+1)-r)));
                    //points.push_back(Point(i,-j - 1));
                //}
            }
        }
        polygonNewOutRingRegular(*polygon, 60, w*0.7, 0,0);
        for (int i=0;i<points.size();i++) {
            polygonNewInRingRegular(*polygon, 0, 3, 0.01, points[i].x, points[i].y);
        }
        //polygon->pointArray = points;
        //int in_count = points.size();
        //polygon->pointArray.push_back(Point(-1,1));
        //polygon->pointArray.push_back(Point(-1,-h-1));
        //polygon->pointArray.push_back(Point(w+1,-h-1));
        //polygon->pointArray.push_back(Point(w+1,1));
        //Region region;
        //Ring out;
        //IntArray outids;

        //outids.push_back(in_count);
        //outids.push_back(in_count+1);
        //outids.push_back(in_count+2);
        //outids.push_back(in_count+3);
        //out.pointIDArray = outids;

        //region.ringArray.push_back(out);

        //for (int i = 0; i < in_count; i+=2) {
        //    Ring in;
        //    in.pointIDArray.push_back(i);
        //    in.pointIDArray.push_back(i+1);
        //    in.pointIDArray.push_back(i+1);
        //    region.ringArray.push_back(in);
        //}
        //
        //polygon->regionArray.push_back(region);

    }
    void tri(CPolygonDoc* pDoc) {
        Delaunay d(polygon);
        try {
            d.computeTriangulation();
        } catch(int e) {}

        pDoc->meshA.clear();
        pDoc->meshA.buildTriangleMesh(&d);

        int len = polygon->regionArray[0].ringArray.size();
        for (int i=1; i<len;i++) {
            IntArray ia = polygon->regionArray[0].ringArray[i].pointIDArray;
            P(ia[1]) = P(ia[0]);
            P(ia[2]) = P(ia[0]);
        }
        
        coloring(pDoc);
    }
    Point imageCoord2ScreenCoord(Point t) {
        int w = img.cols;
        int h = img.rows;
        return Point(t.x - w/2, -t.y + h/2);
    }
    Point screenCoord2ImageCoord(Point t) {
        int w = img.cols;
        int h = img.rows;
        return Point(t.x + w/2, -t.y + h/2);
    }
    void coloring(CPolygonDoc* pDoc) {
        int len = pDoc->meshA.triangles.size();
        
        for (int i = 0; i < len; i++) {
            Triangle t = pDoc->meshA.triangles[i];

            Point p0 = screenCoord2ImageCoord(P(t.vertices[0]));
            Point p1 = screenCoord2ImageCoord(P(t.vertices[1]));
            Point p2 = screenCoord2ImageCoord(P(t.vertices[2]));
            int x0 = max(0., min(p0.x, min(p1.x, p2.x)));
            int x1 = min(img.cols - 1., max(p0.x, max(p1.x, p2.x)));
            int y0 = max(0., min(p0.y, min(p1.y, p2.y)));
            int y1 = min(img.rows - 1., max(p0.y, max(p1.y, p2.y)));
            int tcolor[3] = {0, 0, 0};
            int count = 0;
            for (int u = x0; u <= x1; u++) {
                for (int v = y0; v <= y1; v++) {
                    cv::Vec3b color = img.at<cv::Vec3b>(cv::Point(u, v));
                    if (isIn(Point(u, v), p0, p1, p2)) {
                        tcolor[0] += color[0];
                        tcolor[1] += color[1];
                        tcolor[2] += color[2];
                        count++;
                    }

                }
            }
            if (count == 0) count = 1;
            tcolor[0] /= count;
            tcolor[1] /= count;
            tcolor[2] /= count;
            pDoc->meshA.triangles[i].color[0] = tcolor[2];
            pDoc->meshA.triangles[i].color[1] = tcolor[1];
            pDoc->meshA.triangles[i].color[2] = tcolor[0];
        }
        int a = pDoc->meshA.triangles[0].color[0];
    }
    bool isIn(Point p, Point p1, Point p2, Point p3) {
        Point u1 = p1 - p;
        Point u2 = p2 - p;
        Point u3 = p3 - p;
        if ((p ^ p1) == 0 || (p ^ p2) == 0 || (p ^ p3) == 0) {
            return true;
        }
        double sum = (u1 >> u2) + (u2 >> u3) + (u3 >> u1);
        if (abs(sum) < 1e-8) {
            return false;
        }
        return true;
    }
    void foo(CPolygonDoc* pDoc) {
        /*cv::Mat med;
        cv::medianBlur(img, med, 5);
        cv::Mat edges;
        cv::Canny(med, edges, 3, 9);
        cv::imshow("", med);
        cv::waitKey();*/
        polygon = &(pDoc->a);
        polygon->clear();
        gen();
        tri(pDoc);
    }
    void detect() {
        cv::GoodFeaturesToTrackDetector gftt(5000, 0.01, 1);
        gftt.detect(img, keypoints);
        cv::Mat tmp;
        cv::drawKeypoints(img, keypoints, tmp);
        cv::imshow("", tmp);
        cv::waitKey();
    }
public:
    cv::Mat img;
    PointArray points;
    Polygonal *polygon;
    std::vector<cv::KeyPoint> keypoints;
    TriangleArray triangles;
};
