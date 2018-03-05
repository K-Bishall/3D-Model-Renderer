#include "Model.h"
#include<cmath>
#include<thread>

#define PI 3.1415

/**
 *
 */
/*
void Model::rotate(float angle)
{
    int v = vertexCount;
    std::thread t1(&rotateModel,this,angle,0,v/4);
    std::thread t2(&rotateModel,this,angle,v/4,v/2);
    std::thread t3(&rotateModel,this,angle,v/2,v/4*3);
    std::thread t4(&rotateModel,this,angle,v/4*3,v);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
}
*/
void Model::rotate(float angle)
{

    angle = angle * PI /180;
    float cosA = cos(angle);
    float sinA = sin(angle);

    Vector3d tempV;
    float x,z;
    int i;
    for(i=0; i<vertexCount; i++)
    {
        tempV = vertexTable[i];
        z = tempV.z * cosA - tempV.x * sinA;
        x = tempV.z * sinA + tempV.x * cosA;

        tempV.z = z;
        tempV.x = x;
        vertexTable[i] = tempV;
    }
    return;
}
