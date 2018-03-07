#include "quad.h"

#include <cmath>
#include <iostream>

using namespace std;

Hit Quad::intersect(Ray const &ray)
{
    Hit int1 = t1->intersect(ray);
    Hit int2 = t2->intersect(ray);
    if (std::isnan(int1.t)) {
        return int2;
    } else if (std::isnan(int2.t)) {
        return int1;
    } else if (int1.t < int2.t) {
        return int1;
    } else {
        return int2;
    }
}

/**
 * Create a new quad. Point 1 and 2 should define a diagonal.
 */
Quad::Quad(Point const &p0, Point const &p1, Point const &p2, Point const &p3)
:
    t1(new Triangle(p0, p1, p2)),
    t2(new Triangle(p1, p2, p3))
{}

Quad::~Quad() {
    free(t1);
    free(t2);
}
