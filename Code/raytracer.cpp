#include "raytracer.h"

#include "image.h"
#include "light.h"
#include "material.h"
#include "triple.h"

// =============================================================================
// -- Include all your shapes here ---------------------------------------------
// =============================================================================

#include "shapes/sphere.h"
#include "shapes/plane.h"
#include "shapes/triangle.h"
#include "shapes/quad.h"
#include "shapes/model.h"

// =============================================================================
// -- End of shape includes ----------------------------------------------------
// =============================================================================

#include "json/json.h"

#include <string>
#include <exception>
#include <fstream>
#include <iostream>

using namespace std;        // no std:: required
using json = nlohmann::json;

bool Raytracer::parseObjectNode(json const &node)
{
    ObjectPtr obj = nullptr;

// =============================================================================
// -- Determine type and parse object parametrers ------------------------------
// =============================================================================

    if (node["type"] == "sphere")
    {
        Point pos(node["position"]);
        double radius = node["radius"];
        obj = ObjectPtr(new Sphere(pos, radius));
    }
    else if (node["type"] == "plane")
    {
        Point pos(node["position"]);
        Vector vec(node["normal"]);
        obj = ObjectPtr(new Plane(pos, vec));
    }
    else if (node["type"] == "triangle") {
        Point p0(node["p0"]);
        Point p1(node["p1"]);
        Point p2(node["p2"]);
        obj = ObjectPtr(new Triangle(p0, p1, p2));
    }
    else if (node["type"] == "quad") {
        Point p0(node["p0"]);
        Point p1(node["p1"]);
        Point p2(node["p2"]);
        Point p3(node["p3"]);
        obj = ObjectPtr(new Quad(p0, p1, p2, p3));
    }
    else if (node["type"] == "mesh") {
        std::string const filename(node["filename"].get<std::string>());
        Point pos(node["position"]);
        double scale(node["scale"]);
        obj = ObjectPtr(new Model(filename, pos, scale));
    }
    else
    {
        cerr << "Unknown object type: " << node["type"] << ".\n";
    }

// =============================================================================
// -- End of object reading ----------------------------------------------------
// =============================================================================

    if (!obj)
        return false;

    // Parse material and add object to the scene
    obj->material = parseMaterialNode(node["material"]);
    scene.addObject(obj);
    return true;
}

Light Raytracer::parseLightNode(json const &node) const
{
    Point pos(node["position"]);
    Color col(node["color"]);
    return Light(pos, col);
}

Material Raytracer::parseMaterialNode(json const &node) const
{
    Color color(node["color"]);
    double ka = node["ka"];
    double kd = node["kd"];
    double ks = node["ks"];
    double n  = node["n"];
    return Material(color, ka, kd, ks, n);
}

bool Raytracer::readScene(string const &ifname)
try
{
    // Read and parse input json file
    ifstream infile(ifname);
    if (!infile) throw runtime_error("Could not open input file for reading.");
    json jsonscene;
    infile >> jsonscene;

// =============================================================================
// -- Read your scene data in this section -------------------------------------
// =============================================================================

    if (!jsonscene["ImageSize"].empty()) {
        width = jsonscene["ImageSize"][0];
        height = jsonscene["ImageSize"][1];
    } else {
        width = 400;
        height = 400;
    }

    Point eye(jsonscene["Eye"]);
    scene.setEye(eye);

    if (!jsonscene["Shadows"].empty()) {
        bool shadows(jsonscene["Shadows"]);
        scene.setShadows(shadows);
    } else {
        scene.setShadows(false);
    }

    if (!jsonscene["AntiAliasing"].empty()) {
        unsigned antiAliasing(jsonscene["AntiAliasing"]);
        scene.setAntiAliasing(antiAliasing);
    } else {
        scene.setAntiAliasing(1);
    }

    for (auto const &lightNode : jsonscene["Lights"])
        scene.addLight(parseLightNode(lightNode));

    unsigned objCount = 0;
    for (auto const &objectNode : jsonscene["Objects"])
        if (parseObjectNode(objectNode))
            ++objCount;

    cout << "Parsed " << objCount << " objects.\n";

// =============================================================================
// -- End of scene data reading ------------------------------------------------
// =============================================================================

    return true;
}
catch (exception const &ex)
{
    cerr << ex.what() << '\n';
    return false;
}

void Raytracer::renderToFile(string const &ofname)
{
    // TODO: the size may be a settings in your file
    Image img(width, height);
    cout << "Tracing...\n";
    scene.render(img);
    cout << "Writing image to " << ofname << "...\n";
    img.write_png(ofname);
    cout << "Done.\n";
}
