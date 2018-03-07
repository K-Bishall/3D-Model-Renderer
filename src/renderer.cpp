#include "Model.h"

#include <cmath>
#include <thread>

void Model::renderModel()
{
    initializeBuffers();
    int fcount = faceCount;
    std::thread t1(&render,this,0,fcount/4);
    std::thread t2(&render,this,fcount/4,fcount/2);
    std::thread t3(&render,this,fcount/2,fcount/4*3);
    std::thread t4(&render,this,fcount/4*3,fcount);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
}

void Model::render(int fcountMin, int fcountMax)
{
//--variables for looping over all the points in a triangle--//
    int i,py,px; //< loop counters
    int index;
    Vector3d v0,v1,v2; //< vertices of a triangle
    Vector3d n0,n1,n2; //< normals at the vertices
    Face f;
    BoundingBox bbox;
    float yMin, yMax, xMin, xMax;

//--variables for interpolation and z buffer--//
    float area, w0, w1, w2;
    float pz; //< depth of point P(x,y)

//--variables for illumination and shading--//
    lightSource = viewTransform(lightSource); //<< transform the light source along with all the vertices
    Vector3d N; //< normal at the point
    Vector3d L; //< vector from point to light source
    Vector3d V; //< vector from point to camera
    Vector3d R; //< reflection vector
    Vector3d H; //< unit vector of (L+V)
    Vector3d P; //< position vector of point P
    float Iconst = Ka * Iamb; //< this is constant for all points
    float dL; //< distance between point and light source
    float Ip; //< intensity at the point
    Vector3d viewer = viewTransform(camera);

    // float I0, I1, I2;

    int r = 139, g = 0, b = 139; //< RGB value for color -- FOR NOW THE MODEL IS CONSIDERED TO HAVE SINGLE COLOR
    int r1,g1,b1;
//-- NOW IS THE TIME TO ITERATE OVER ALL THE TRIANGLEs --//
    for(i = fcountMin; i < fcountMax; i++)
    {
        f = faceTable[i];
        v0 = project(vertexTable[f.v0]); //v0.printData(); std::cout<<std::endl;
        v1 = project(vertexTable[f.v1]); //v1.printData(); std::cout<<std::endl;
        v2 = project(vertexTable[f.v2]); //v1.printData(); std::cout<<std::endl;
/*
        n0 = normalTable[f.n0];
        P = vertexTable[f.v0];
        N = n0.unitVector();
        L = (lightSource - P);
        dL = L.getMagnitude();
        V = (viewer - P);
        H = (L + V).unitVector();
        dL = 1 + dL * dL;
        I0 = Ka + (Kd * Ipoint / dL * N.dot(L.unitVector())) + (Ks * Ipoint / dL * pow(N.dot(H),ns));

        n1 = normalTable[f.n1];
        P = vertexTable[f.v1];
        N = n1.unitVector();
        L = (lightSource - P);
        dL = L.getMagnitude();
        V = (viewer - P);
        H = (L + V).unitVector();
        dL = 1 + dL * dL;
        I1 = Ka + (Kd * Ipoint / dL * N.dot(L.unitVector())) + (Ks * Ipoint / dL * pow(N.dot(H),ns));

        n2 = normalTable[f.n2];
        P = vertexTable[f.v2];
        N = n2.unitVector();
        L = (lightSource - P);
        dL = L.getMagnitude();
        V = (viewer - P);
        H = (L + V).unitVector();
        dL = 1 + dL*100;
        I2 = Ka + (Kd * Ipoint / dL * N.dot(L.unitVector())) + (Ks * Ipoint / dL * pow(N.dot(H),ns));
*/
        bbox = getBoundry(v0,v1,v2);
        yMin = bbox.yMin, yMax = bbox.yMax, xMin = bbox.xMin, xMax = bbox.xMax;

        //iterate over all the pixels (x,y) inside the triangle
        for(py = yMin; py <= yMax; py++)
        {
            //if p(x,y) is outside window boundry ignore it
            if(py <= 0 || py >= windowY)
                continue;
            for(px = xMin; px <= xMax; px++)
            {
                //if p(x,y) is outside window boundry ignore it
                if(px <= 0 || px >= windowX)
                    continue;

                //to check if the point (px,py) inside the triangle
                area = edgeFunction(v0,v1,v2.x,v2.y); //< Area of triangle * 2
                w0 = edgeFunction(v0,v1,px,py);
                w1 = edgeFunction(v1,v2,px,py);
                w2 = edgeFunction(v2,v0,px,py);

                //if point P(x,y) is inside the triangle
                if(w0 >= 0 && w1 >= 0 && w2 >= 0)
                {
                    //parameters for interpolation of vertex attributes : Z-depth, Normal
                    //baricentric coordinates are given as
                    w0 /= area;
                    w1 /= area;
                    w2 /= area;

                    //compute Z-depth of P(x,y)
                    //this is the formula to interpolate Z-value from the 3 vertices of triangle
                    pz = w0/v0.z + w1/v1.z + w2/v2.z;
                    pz = 1/pz;

                    //check if this point is nearer to camera - if yes place its z value in zBuffer
                    if(pz < zBuffer[py*windowX + px])
                    {
                        index = py*windowX + px;
                        //update zBuffer
                        zBuffer[index] = pz;

                        // And Now we are going to calculate INTENSITY AT THE POINT
                        //Let's first find out Interpolated Normal at the point
                        n0 = normalTable[f.n0];
                        n1 = normalTable[f.n1];
                        n2 = normalTable[f.n2];

                        N = n0.multiply(w0/v0.z) + n1.multiply(w1/v1.z) + n2.multiply(w2/v2.z);
                        N = N.multiply(pz); //< this is the normal vector at the point

                        //Calculate the stuffs
                        P = Vector3d(px,py,pz); toView(P);
                       // P.printData(); std::cout<<std::endl;
                        N = N.unitVector();
                        L = (lightSource - P).unitVector();
                        dL = L.getMagnitude();
                        //std::cout<<dL<<std::endl;
                        V = (viewer - P).unitVector();
                        R = N.multiply(2 * N.dot(L)) - L;
                        R = R.unitVector();
                        L = L.unitVector();
                        // H = (L + V).unitVector();
                        dL = 1 + dL * dL;
                        //std::cout<<dL<<std::endl;
                        //And here is the Intensity at the point
                        Ip = Iamb * Ka + (Kd * Ipoint / dL * abs(N.dot(L))) + (Ks * Ipoint / dL * pow(V.dot(R),ns));
                        //std::cout<<"I "<<Ip<<std::endl;
                        //Intensity of RGB
                        r1 = r * Ip;
                        g1 = g * Ip;
                        b1 = b * Ip;

                        // Ip = pz * (I0*w0/v0.z + I1*w1/v1.z + I2*w2/v2.z);
                        // r1 = r * Ip;
                        // g1 = g * Ip;
                        // b1 = b * Ip;
                        //store the color in color buffer
                        colorBuffer[index] = sf::Color(r1,g1,b1);
                    }
                }
            }
        }
    }

}

sf::VertexArray vArray(sf::Points,windowY*windowX);
void Model::drawModel(sf::RenderWindow &window)
{
    int y,x;int index;
    for(y = 0; y < windowY; y++)
    {
        for(x = 0; x < windowX; x++)
        {
            index = y*windowX + x;
            vArray[index].position = sf::Vector2f(x,y);
            vArray[index].color = colorBuffer[y*windowX + x];
        }
    }

    window.draw(vArray);
}

