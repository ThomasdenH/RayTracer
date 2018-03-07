#ifndef QUAD_H_
#define QUAD_H_

#include "../object.h"
#include "triangle.h"

class Quad: public Object
{
    public:
        Quad(Point const &p0, Point const &p1, Point const &p2, Point const &p3);
        ~Quad();

        virtual Hit intersect(Ray const &ray);

        Triangle* t1;
        Triangle* t2;
};

#endif
