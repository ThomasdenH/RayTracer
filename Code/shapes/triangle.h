#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "../object.h"

class Triangle: public Object
{
    public:
        Triangle(Point const &p0, Point const &p1, Point const &p2);

        virtual Hit intersect(Ray const &ray);

        Point const p0;
        Point const p1;
        Point const p2;
        Vector const normal;
};

#endif
