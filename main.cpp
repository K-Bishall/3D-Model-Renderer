#include<iostream>
using namespace std;

#include "Model.h"
#include "Vector3d.h"

#include "SFML/Graphics.hpp"
using namespace sf;

//const int windowX = 1000, windowY = 600;    //< defined in Model.h due to technical difficulty

int main()
{

    Vector3d viewRefPoint(0,0,5);
    Vector3d lookAtPoint(1,1,1);
    Vector3d viewUp(0,1,0);
    float Zvp = 10;
    float Zprp = 3;
    float scaleFactor = 12;

    Model model;
    const char filename[] = "model.obj";
    model.setViewCoordinate(viewRefPoint, lookAtPoint,viewUp,Zvp, Zprp, scaleFactor);
    model.loadModel(filename);

    model.lightSource = Vector3d(-100,100,100);
    model.Iamb = INF;
    model.Ipoint = INF;
    model.Ka = 0.75;
    model.Kd = 0.95;
    model.Ks = 0.3;
    model.ns = 25;

    RenderWindow window;
    window.create(VideoMode(windowX,windowY),"3d Model Renderer");
    window.setFramerateLimit(30);
    window.setKeyRepeatEnabled(false);

    model.renderModel();

    while(window.isOpen())
    {
        Event event;
        while(window.pollEvent(event))
            if(event.type == Event::Closed)
                window.close();


        window.clear(Color::Black);
     //   model.rotate(2);
       // model.renderModel();
        model.drawModel(window);
        window.display();
    }

return 0;
}
