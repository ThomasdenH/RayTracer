#include "model.h"

#include "triangle.h"
#include "../objloader.h"

#include <string>
#include <vector>
#include <iostream>

using namespace std;

Hit Model::intersect(Ray const &ray)
{
    // Find hit object and distance
    Hit min_hit(numeric_limits<double>::infinity(), Vector());
    for (unsigned idx = 0; idx != triangles.size(); ++idx)
    {
        Hit hit(triangles.at(idx).intersect(ray));
        if (hit.t < min_hit.t)
        {
            min_hit = hit;
        }
    }
    return min_hit;
}

Model::Model(std::string const &filename, Point const &pos, double scale) {
    OBJLoader* objLoader = new OBJLoader(filename);
    triangles = vector<Triangle>();
    vector<Vertex> vertexData = objLoader->vertex_data();

    for (unsigned i = 0; i < objLoader->numTriangles(); i++) {
        Vertex v0 = vertexData.at(i * 3);
        Vertex v1 = vertexData.at(i * 3 + 1);
        Vertex v2 = vertexData.at(i * 3 + 2);
        Point const &p1 = scale * Point(v0.x, v0.y, v0.z) + pos;
        Point const &p2 = scale * Point(v1.x, v1.y, v1.z) + pos;
        Point const &p3 = scale * Point(v2.x, v2.y, v2.z) + pos;
        triangles.push_back(Triangle(p1, p2, p3));
    }
}
