#ifndef MODEL_H_
#define MODEL_H_

#include "../object.h"
#include "triangle.h"

#include <string>

class Model: public Object
{
    public:
        Model(std::string const &filename, Point const &pos, double scale);

        virtual Hit intersect(Ray const &ray);

        virtual TextureCoordinate (Point const &point);

        std::vector<Triangle> triangles;
};

#endif
