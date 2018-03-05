#include "Model.h"
#include<thread>

void Model::renderModel()
{
    initializeBuffers();
    int fcount = faceCount;
    std::thread t1(&render,this,0,fcount/2);
    std::thread t2(&render,this,fcount/2,fcount);

    t1.join();
    t2.join();
}

void Model::render(int fcountMin, int fcountMax)
{

    int i,py,px; //< loop counters
    int index;
    Vector3d v0,v1,v2; //< vertices of a triangle
    Face f;
    BoundingBox bbox;
    float yMin, yMax, xMin, xMax;

    float area, w0, w1, w2;
    float pz; //< depth of point P(x,y)

//    int r=255,g=255,b=255; //< color values
    sf::Color color;
     sf::Color c0(139,0,139),c1(139,0,139),c2(139,0,139);
    // sf::Color c0,c1,c2;

    //For each triangle in the model
    for(i = fcountMin; i < fcountMax; i++)
    {
        f = faceTable[i];
        v0 = viewTransform(vertexTable[f.v0]);
        v1 = viewTransform(vertexTable[f.v1]);
        v2 = viewTransform(vertexTable[f.v2]);

        bbox = getBoundry(v0,v1,v2);
        yMin = bbox.yMin, yMax = bbox.yMax, xMin = bbox.xMin, xMax = bbox.xMax;

        //iterate over all the pixels (x,y) inside the triangle
        for(py = yMin; py <= yMax; py++)
        {
            if(py <= 0 || py >= windowY)
                continue;
            for(px = xMin; px <= xMax; px++)
            {
                //if p(x,y) is outside window boundry ignore it
                if(px <= 0 || px >= windowX)
                    continue;

                area = edgeFunction(v0,v1,v2.x,v2.y); //< Area of triangle * 2
                w0 = edgeFunction(v0,v1,px , py);
                w1 = edgeFunction(v1,v2,px , py);
                w2 = edgeFunction(v2,v0,px , py);

                //if point P(x,y) is inside the triangle
                if(w0 >= 0 && w1 >= 0 && w2 >= 0)
                {
                    //baricentric coordinates are given as
                    w0 /= area;
                    w1 /= area;
                    w2 /= area;

                    //compute Z-depth of P(x,y)
                    //this is the formula to interpolate Z-value from the 3 vertices of triangle
                    pz = w0/v0.z + w1/v1.z + w2/v2.z;
                    pz = 1/pz;

                    //check if this point is nearer to camera
                    if(pz < zBuffer[py*windowX + px])
                    {
                        index = py*windowX + px;
                        //update zBuffer and colorBuffer
                        zBuffer[index] = pz;

                        // if(w0 == 0 || w1 == 0 || w2 ==0)
                        color.r = pz * (w0 * c0.r/v0.z + w1 * c1.r/v1.z + w2 * c2.r/v2.z);
                        color.g = pz * (w0 * c0.g/v0.z + w1 * c1.g/v1.z + w2 * c2.g/v2.z);
                        color.b = pz * (w0 * c0.b/v0.z + w1 * c1.b/v1.z + w2 * c2.b/v2.z);
                        colorBuffer[index] = color;
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
