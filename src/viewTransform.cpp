#include "Model.h"

#include<iostream>

/**
 * @brief This function sets up a viewing coordinate based on
 * view reference (camera position), Normal Vector and View Up Vector
 * @param Camera Point, Look-at-point, view UP vector and Zvp
 * @return void - sets up view coordinate
 */
void Model::setViewCoordinate(Vector3d &viewRefPoint, Vector3d &lookAtPoint, Vector3d &V, float ZviewPlane,
                                 float zprp, float scalefactor)
{
    viewRef = viewRefPoint;
    Zvp = ZviewPlane;
    Zprp = zprp;
    scaleFactor = scalefactor;
    dp = Zprp - Zvp;

    n = (viewRef - lookAtPoint).unitVector();
    u = (V*n).unitVector();
    v = n * u;

    //repeated values in viewTransform; so calculated at once
    val1 = u.x * viewRef.x + u.y * viewRef.y + u.z * viewRef.z;
    val2 = v.x * viewRef.x + v.y * viewRef.y + v.z * viewRef.z;
    val3 = n.x * viewRef.x + n.y * viewRef.y + n.z * viewRef.z;
/*
    std::cout<<"viewRef ";
    viewRef.printData();
    std::cout<<std::endl;

    std::cout<<"N ";
    (viewRef - lookAtPoint).printData();
    std::cout<<std::endl;

    std::cout<<"V*n ";
    (V*n).printData();
    std::cout<<std::endl;

    std::cout<<"u ";
    u.printData();
    std::cout<<std::endl;

    std::cout<<"v ";
    v.printData();
    std::cout<<std::endl;

    std::cout<<"n ";
    n.printData();
    std::cout<<std::endl;
*/
}


/**
 * @brief viewTransform transforms the Model in world coordinate
 * to viewing coordinate.And also PERSPECTIVE PROJECTION is performed.
 * This function is called from within loadModel().
 * @param viewRef - camera point, N - normal vector, U - view Up vector
 * @return void - transforms vertex table to view coordinate
 */
Vector3d Model::viewTransform(const Vector3d &tempV)
{
    Vector3d tempViewV;

    tempViewV.x = u.x * tempV.x + u.y * tempV.y + u.z * tempV.z - val1;
    tempViewV.y = v.x * tempV.x + v.y * tempV.y + v.z * tempV.z - val2;
    tempViewV.z = n.x * tempV.x + n.y * tempV.y + n.z * tempV.z - val3;

    tempViewV.x *= scaleFactor;
    tempViewV.y *= scaleFactor;
    tempViewV.z *= scaleFactor;

    //Now is the time for PERSPECTIVE PROJECTION of ViewCoordinate point
    float h = dp / (Zprp - tempViewV.z);
    tempViewV.x /= h;
    tempViewV.y /= h;

    //translate projected point to SFML window
    tempViewV.x += windowX/3;
    tempViewV.y += windowY/3;

/*
    std::cout<<"View ";
    tempViewV.printData();
    std::cout<<std::endl<<std::endl;
*/
    return tempViewV;
}
