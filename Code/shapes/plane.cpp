#include "plane.h"

#include <cmath>
#include <iostream>

using namespace std;

Hit Plane::intersect(Ray const &ray)
{
    double ln = ray.D.dot(normal);

    if (ln == 0)
        return Hit::NO_HIT();

    double t = (position - ray.O).dot(normal) / ln;
    if (t < 0)
        return Hit::NO_HIT();
    return Hit(t, normal);
}

Plane::Plane(Point const &pos, Vector const &norm)
:
    position(pos),
    normal(norm)
{}
