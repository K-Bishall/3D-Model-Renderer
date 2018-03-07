#include<iostream>
using namespace std;

#include "Model.h"
#include "Vector3d.h"

#include "SFML/Graphics.hpp"
using namespace sf;

//const int windowX = 1000, windowY = 600;    //< defined in Model.h due to technical difficulty

int main()
{

    Vector3d viewRefPoint(0,0,30);
    Vector3d lookAtPoint(0,0,0);
    Vector3d viewUp(0,1,0);
    float Zvp = 15;
    float Zprp = 0;

    Model model;
    const char filename[] = "model.obj";
 //   model.setViewCoordinate(viewRefPoint, lookAtPoint,viewUp,Zvp, Zprp, scaleFactor);
    model.loadModel(filename);
    model.normalize();
    model.autoFocus();
    model.camera.y = 100;
    model.setViewCoordinate();
    model.scaleFactor = 10;
    model.viewTransform();
/*
    model.lightSource = Vector3d(1,1,1);
    model.Ipoint = 100;
    model.Ka = 5;
    model.Kd = 2.5;
    model.Ks = 10;
    model.ns = 25;
*/
    RenderWindow window;
    window.create(VideoMode(windowX,windowY),"3d Model Renderer");
    window.setFramerateLimit(30);
    window.setKeyRepeatEnabled(false);

 //   model.rotate(60);
    model.renderModel();


    while(window.isOpen())
    {
        Event event;
        while(window.pollEvent(event))
            if(event.type == Event::Closed)
                window.close();


        window.clear(Color::Black);
      //  model.rotate(-2);
        //model.renderModel();
        model.drawModel(window);
        window.display();
    }


return 0;
}
