#include "scene.h"

#include "hit.h"
#include "image.h"
#include "material.h"
#include "ray.h"

#include <cmath>
#include <limits>

using namespace std;

Color Scene::trace(Ray const &ray)
{
    // Find hit object and distance
    Hit min_hit(numeric_limits<double>::infinity(), Vector());
    ObjectPtr obj = nullptr;
    int objectIndex = -1;
    for (unsigned idx = 0; idx != objects.size(); ++idx) {
        Hit hit(objects[idx]->intersect(ray));
        if (hit.t < min_hit.t) {
            min_hit = hit;
            obj = objects[idx];
            objectIndex = idx;
        }
    }

    // No hit? Return background color.
    if (!obj) return Color(0.0, 0.0, 0.0);

    Material material = obj->material;          //the hit objects material
    Point hit = ray.at(min_hit.t);              //the hit point
    Vector N = min_hit.N;                       //the normal at hit point
    Vector V = -ray.D;                          //the view vector

    /****************************************************
    * This is where you should insert the color
    * calculation (Phong model).
    *
    * Given: material, hit, N, V, lights[]
    * Sought: color
    *
    * Hints: (see triple.h)
    *        Triple.dot(Vector) dot product
    *        Vector + Vector    vector sum
    *        Vector - Vector    vector difference
    *        Point - Point      yields vector
    *        Vector.normalize() normalizes vector, returns length
    *        double * Color     scales each color component (r,g,b)
    *        Color * Color      dito
    *        pow(a,b)           a to the power of b
    ****************************************************/

    Color color = material.color;                  // place holder

    // Add ambient lighting
    Color resultColor = color * material.ka;

    for (unsigned i = 0; i < getNumLights(); i++) {
        Vector L = (lights[i]->position - hit).normalized();
        Ray ray = Ray(lights[i]->position, -1 * L);
        // Check if the point is in the shadow for this light
        if (!this->shadows || !this->inShadow(ray, objectIndex)) {
            // Add diffuse lighting
            resultColor += color * material.kd * max(0.0, L.dot(N));

            // Add specular lighting
            Vector R = (2 * (N.dot(L)) * N - L).normalized();
            resultColor += pow(max(0.0, R.dot(V)), 50.0) * material.ks;
        }
    }

    return resultColor;
}

void Scene::render(Image &img)
{
    double factor = 1.0 / (double) antiAliasing / (double) antiAliasing;
    double dSize = 1.0 / (double) (antiAliasing + 1);
    unsigned w = img.width();
    unsigned h = img.height();
    for (unsigned y = 0; y < h; ++y)
    {
        for (unsigned x = 0; x < w; ++x)
        {
            Color color = Color();
            for (unsigned dx = 1; dx <= antiAliasing; dx++) {
                for (unsigned dy = 1; dy <= antiAliasing; dy++) {
                    Point pixel((x + dSize * dx), (h - 1 - y + dSize * dy), 0);
                    Ray ray(eye, (pixel - eye).normalized());
                    Color partialColor = trace(ray);
                    color += partialColor * factor;
                }
            }
            color.clamp();
            img(x, y) = color;
        }
    }
}

/**
 * Returns whether an object is in the shadow.
 */
bool Scene::inShadow(Ray ray, int objectIndex) {
    Hit objectHit(objects[objectIndex]->intersect(ray));

    for (unsigned i = 0; i < objects.size(); i++) {
        if ((int) i != objectIndex) {
            Hit hit(objects[i]->intersect(ray));
            if (hit.t < objectHit.t) {
                return true;
            }
        }
    }
    return false;
}

// --- Misc functions ----------------------------------------------------------

void Scene::setAntiAliasing(unsigned antiAliasing) {
    this->antiAliasing = antiAliasing;
}

void Scene::addObject(ObjectPtr obj)
{
    objects.push_back(obj);
}

void Scene::addLight(Light const &light)
{
    lights.push_back(LightPtr(new Light(light)));
}

void Scene::setEye(Triple const &position)
{
    eye = position;
}

void Scene::setShadows(bool shadows)
{
    this->shadows = shadows;   
}

unsigned Scene::getNumObject()
{
    return objects.size();
}

unsigned Scene::getNumLights()
{
    return lights.size();
}
