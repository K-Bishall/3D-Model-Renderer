#ifndef MODEL_H
#define MODEL_H

#include "Face.h"
#include "Vector3d.h"

#include <vector>
#include <array>
#include "SFML/Graphics.hpp"

const int windowX = 1024, windowY = 576;

/**
 * @brief Class Model is everything for a model to load and transform
 * It has functions for loading obj file, construct tables, transform and stuffs
 */
class Model
{
private:
    std::vector<Vector3d> vertexTable;
    std::vector<Vector3d> normalTable;
    std::vector<Face> faceTable;

    Vector3d viewRef;
    float Zvp, Zprp; //< Z-VIEW POINT and Z-PROJECTION REFERENCE POINT
    Vector3d u, v, n; //< unit vectors uvn for view plane

    // values calculated in world to view transform
    float dp, val1, val2, val3; //< calculated in setViewCoordinate and used in viewTransform

    int vertexCount, faceCount, normalCount;

public:
    float scaleFactor; //< to scale the model to fit in window


private:
//These are the attributes and tables we need for Z-BUFFER
    float *zBuffer = new float[windowY * windowX];
    sf::Color *colorBuffer = new sf::Color[windowY * windowX];
    sf::Vertex vertex; //< to draw vertex -- used in drawModel() in renderer.cpp

public:
//These are the attributes required for ILLUMINATION AND SHADING
    Vector3d lightSource;
    float Iamb; //< ambient light intensity
    float Ipoint; //< point source intensity
    float Ka, Kd, Ks; //< reflection coefficients respectively for ambient, diffused and specular reflection
    float ns; //< specular reflection parameter


public:
    Model();
    virtual ~Model() {delete zBuffer; delete colorBuffer;}

    bool loadModel(const char *);

    void rotate(float);    //< defined in transformation.cpp
//    void rotateModel(float);

    //setup viewCoordinate--takes viewRef,N and U
    //defined in viewTransform.cpp
    void setViewCoordinate(Vector3d &, Vector3d &, Vector3d &, float, float, float);
    //defined in viewTransform.cpp
    Vector3d viewTransform(const Vector3d &);

    void printData();


public:
//And these are the methods for the RASTERIZATION -- Z-BUFFER AND SHADING
//these are defined in rasterization.cpp
    void initializeBuffers();
    float edgeFunction(const Vector3d &, const Vector3d &, const int&, const int&);
//And these are in renderer.cpp
    void renderModel();
    void render(int,int);
    void drawModel(sf::RenderWindow &);

};



//-----------To define the 2D boundry of triangle to reduce point check-----------//
struct BoundingBox
{
    int xMin,xMax;
    int yMin,yMax;
};

/**
 * This returns the rectangular boundry of the triangle in 2D
 */
BoundingBox getBoundry(const Vector3d &v0, const Vector3d &v1, const Vector3d &v2);

#endif // MODEL_H
