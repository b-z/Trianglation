#include <iostream>
#include <fstream>
#include <string>

#include "Delaunay.h"
#include "Polygonal.h"
#include "Triangle.h"

#include "test.h"

int main() {
    Test t;
    t.test(2000, POLYGON_REGULAR, "result_regular_.tsv");
    t.test(2000, POLYGON_BAR, "result_bar_.tsv");
    std::cout << "finish\n";
    std::cin.get();
    return 0;
}