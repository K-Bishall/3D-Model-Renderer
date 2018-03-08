#include "Model.h"
#include<cmath>

#define PI 3.1415

void Model::rotate(float angle)
{

    angle = angle * PI /180;
    float cosA = cos(angle);
    float sinA = sin(angle);

    Vector3d center((xmax + xmin)/2,(ymax + ymin)/2,(zmax + zmin)/2);

    float x,z;
    int i;
    for(i=0; i<vertexCount; i++)
    {
        Vector3d &v = vertexTable[i];
        v = v - center;
        z = v.z * cosA - v.x * sinA;
        x = v.z * sinA + v.x * cosA;

        v.z = z;
        v.x = x;
        v = v + center;
    }
}
