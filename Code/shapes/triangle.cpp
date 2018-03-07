#include "triangle.h"

#include <cmath>
#include <iostream>

using namespace std;

Hit Triangle::intersect(Ray const &ray)
{
    double EPSILON = 0.0000001;

    Vector edge1 = p1 - p0;
    Vector edge2 = p2 - p0;
    Vector h = ray.D.cross(edge2);
    double a = edge1.dot(h);
    if (a > -EPSILON && a < EPSILON) {
        return Hit::NO_HIT();
    }

    double f = 1.0 / a;
    Vector s = ray.O - p0;
    double u = f * s.dot(h);
    if (u < 0.0 || u > 1.0) {
        return Hit::NO_HIT();
    }

    Vector q = s.cross(edge1);
    double v = f * ray.D.dot(q);
    if (v < 0.0 || u + v > 1.0) {
        return Hit::NO_HIT();
    }

    double t = f * edge2.dot(q);
    if (t > EPSILON) {
        if (normal.dot(ray.D) > 0) {
            return Hit(t, -normal);
        } else {
            return Hit(t, normal);
        }
    } else {
        return Hit::NO_HIT();
    }
}

Triangle::Triangle(Point const &p0, Point const &p1, Point const &p2)
:
    p0(p0),
    p1(p1),
    p2(p2),
    normal((p1 - p0).cross(p2 - p1).normalized())
{}
