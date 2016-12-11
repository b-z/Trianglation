#pragma once

#include <cmath>
#include <cassert>
#include <ctime>
#include <iostream>
#include <fstream>
#include <string>

#include "Polygonal.h"
#include "Delaunay.h"

#define POLYGON_REGULAR 1
#define POLYGON_BAR 2

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

class Test {
public:
    Polygonal *polygon;

public:
    Test() {
        polygon = new Polygonal();
    }
    ~Test() {
        delete polygon;
    }
    void generatePolygon(int size, int type) {
        assert(size >= 3);
        polygon->clear();
        PointArray points;
        switch (type)
        {
        case POLYGON_REGULAR:
            for (int i = 0; i < size; i++) {
                double theta = 2 * PI / size * i;
                points.push_back(Point(cos(theta), sin(theta)));
            }
            break;
        case POLYGON_BAR:
            for (int i = 0; i < size / 2; i++) {
                points.push_back(Point(i, 0));
            }
            for (int i = size / 2; i < size; i++) {
                points.push_back(Point(size - 1 - i, 1));
            }
            break;
        }
        polygon->pointArray = points;

        IntArray pointIDs;
        for (int i = 0; i < size; i++) {
            pointIDs.push_back(i);
        }
        Ring ring;
        ring.pointIDArray = pointIDs;
        RingArray rings;
        rings.push_back(ring);
        Region region;
        region.ringArray = rings;
        RegionArray regions;
        regions.push_back(region);
        polygon->regionArray = regions;
    }
    double testCurrentPolygon(int& count) {
        double time_total = 0;
        double time_once = 0;
        int iteration_times = 1;
        Delaunay d(polygon);
        while (time_total < 1) {
            time_once = 1e30;
            for (int i = 0; i < 1; i++) {
                time_t t0 = clock();
                for (int i = 0; i < iteration_times; i++) {
                    d.computeTriangulation();
                }
                time_t t1 = clock();
                count = d.count;
                time_total = double(t1 - t0) / CLOCKS_PER_SEC;
                time_once = min(time_once, time_total / iteration_times);
            }

            iteration_times *= 2;
        }
        return time_once;
    }
    void test(int max_size, int type, std::string file) {
        double size = 3;
        ofstream fout(file);
        using std::cout;
        fout << "size" << "\t" << "result" << "\t" << "log2(size)" << "\t" << "log2((result))" << "\t" << "count" << "\t" << "log2((count))" << "\n";
        cout << "size" << "\t" << "result" << "\t" << "log2(size)" << "\t" << "log2((result))" << "\t" << "count" << "\t" << "log2((count))" << "\n";
        while (size < max_size) {
            generatePolygon(int(size), type);
            int count;
            double result = testCurrentPolygon(count);
            fout << int(size) << "\t" << result << "\t" << log2(int(size)) << "\t" << log2((result)) << "\t" << count << "\t" << log2((count)) << "\n";
            cout << int(size) << "\t" << result << "\t" << log2(int(size)) << "\t" << log2((result)) << "\t" << count << "\t" << log2((count)) << "\n";
            double scale = 1.1;
            while (int(size) == int(size * scale)) {
                size *= scale;
            }
            size *= scale;
        }
        fout.close();
    }
};