#include "Model.h"

#include<iostream>

using std::cout;
using std::endl;

/**
 * @brief Convert model into unit cube and then to cube in range 0 to 100. THis is done to define any model in
 * same range
 */
void Model::normalize ()
{
    //xvmin = yvmin = zvmin = 0;
    float xvmax = 100, yvmax = 100, zvmax = 100;
    float sx = xvmax / (xmax - xmin);
    float sy = yvmax / (ymax - ymin);
    float sz = zvmax / (zmax - zmin);

    int i;
    for(i = 0; i < vertexCount; i++) {
        Vector3d &v = vertexTable[i];
        v.x = (v.x - xmin) * sx;
        v.y = (v.y - ymin) * sy;
        v.z = (v.z - zmin) * sz;
    }

    //At this point our model is defined in 100x100x100 cube
    //so
    xmax = (xmax - xmin) * sx;
    ymax = (ymax - ymin) * sy;
    zmax = (zmax - zmin) * sz;
    xmin = ymin = zmin = 0;
}

// to auto adjust the camera and view plane
void Model::autoFocus()
{
    camera = Vector3d((xmax + xmin)/2, (ymax + ymin)/2,(2*zmax + zmin));
    lookAt = Vector3d((xmax + xmin)/2, (ymax + ymin)/2, zmax);
    viewUp = Vector3d(0,1,0);
    Zprp = camera.z;
}

/**
 * @brief This function sets up a viewing coordinate based on
 * view reference (camera position), Normal Vector and View Up Vector
 * @param Camera Point, Look-at-point, view UP vector and Zvp
 * @return void - sets up view coordinate
 */
void Model::setViewCoordinate()
{
    n = (camera - lookAt).unitVector();
    u = (viewUp.crossProduct(n)).unitVector();
    v = n.crossProduct(u);

    //repeated values in viewTransform; so calculated at once
    val1 = u.x * camera.x + u.y * camera.y + u.z * camera.z;
    val2 = v.x * camera.x + v.y * camera.y + v.z * camera.z;
    val3 = n.x * camera.x + n.y * camera.y + n.z * camera.z;
/*
    std::cout<<"camera ";
    camera.printData();
    std::cout<<std::endl;

    std::cout<<"N ";
    (camera - lookAtPoint).printData();
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
 * @param camera - camera point, N - normal vector, U - view Up vector
 * @return void - transforms vertex table to view coordinate
 */
void Model::viewTransform()
{
    int i;
    float xt,yt,zt;
    for(i = 0; i < vertexCount; i++) {
        Vector3d &vt = vertexTable[i];

        xt = u.x * vt.x + u.y * vt.y + u.z * vt.z - val1;
        yt = v.x * vt.x + v.y * vt.y + v.z * vt.z - val2;
        zt = n.x * vt.x + n.y * vt.y + n.z * vt.z - val3;

        vt.x = xt * scaleFactor;
        vt.y = yt * scaleFactor;
        vt.z = zt * scaleFactor;

        // cout<<"vt "<<xt<<" "<<yt<<" "<<zt<<endl;
    }

    xt = u.x * xmax + u.y * ymax + u.z * zmax - val1;
    yt = v.x * xmax + v.y * ymax + v.z * zmax - val2;
    zt = n.x * xmax + n.y * ymax + n.z * zmax - val3;
    xmax = xt * scaleFactor; ymax = yt * scaleFactor; zmax = zt * scaleFactor;

    xt = u.x * xmin + u.y * ymin + u.z * zmin - val1;
    yt = v.x * xmin + v.y * ymin + v.z * zmin - val2;
    zt = n.x * xmin + n.y * ymin + n.z * zmin - val3;
    xmin = xt * scaleFactor; ymin = yt * scaleFactor; zmin = zt * scaleFactor;

  //  Zvp = ((ymax - ymin - windowY) * Zprp + windowY * zmax) / (ymax - ymin);
    Zvp = (Zprp - zmax)/2;
    dp = Zprp - Zvp;

    // cout<<"max "<<xmax<<" "<<ymax<<" "<<zmax<<endl;
    // cout<<"min "<<xmin<<" "<<ymin<<" "<<zmin<<endl;

    // tempViewV.x *= scaleFactor;
    // tempViewV.y *= scaleFactor;
    // tempViewV.z *= scaleFactor;

/*
    std::cout<<"View ";
    tempViewV.printData();
    std::cout<<std::endl<<std::endl;
*/
}

Vector3d Model::viewTransform(Vector3d &vt) {
    float xt,yt,zt;
    xt = u.x * vt.x + u.y * vt.y + u.z * vt.z - val1;
    yt = v.x * vt.x + v.y * vt.y + v.z * vt.z - val2;
    zt = n.x * vt.x + n.y * vt.y + n.z * vt.z - val3;

    vt.x = xt * scaleFactor;
    vt.y = yt * scaleFactor;
    vt.z = zt * scaleFactor;

    return v;
}

Vector3d Model::project(const Vector3d &v)
{

    //Now is the time for PERSPECTIVE PROJECTION of ViewCoordinate point
    float xp,yp, zp = v.z;
    h = (Zprp - v.z)/dp;
    xp = v.x / h;
    yp = v.y / h;

    //translate projected point to SFML window
    xp += windowX/2;
    yp += windowY/2;
    // v.y = -v.y;

    // cout<<"vp "<<v.x<<" "<<v.y<<" "<<v.z<<endl;
    return Vector3d(xp,yp,zp);
}

void Model::toView(Vector3d &v)
{
    v.x -= windowX/2;
    v.y -= windowY/2;

    v.x *= h;
    v.y *= h;
}
