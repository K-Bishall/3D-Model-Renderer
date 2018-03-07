#include "Model.h"
#include<cmath>

#define PI 3.1415

void Model::rotate(float angle)
{

    angle = angle * PI /180;
    float cosA = cos(angle);
    float sinA = sin(angle);

    Vector3d center((xmax + xmin)/2,(ymax + ymin)/2,(zmax + zmin)/2);
    // std::cout<<"cos "<<cosA<<" sin "<<sinA<<std::endl<<std::endl;
    // center.printData();
    // std::cout<<std::endl<<std::endl;

    float x,z;
    int i;
    for(i=0; i<vertexCount; i++)
    {
        Vector3d &v = vertexTable[i];
        // v.printData();std::cout<<std::endl<<std::endl;
        v = v - center;
        z = v.z * cosA - v.x * sinA;
        x = v.z * sinA + v.x * cosA;

        v.z = z;
        v.x = x;
        v = v + center;
        // v.printData();
        // std::cout<<std::endl;
    }
}
