#include<iostream>
using namespace std;

#include "Model.h"
#include "Vector3d.h"

#include "SFML/Graphics.hpp"
using namespace sf;

int main()
{
    Model model;
    const char filename[] = "dragon1.obj";
    model.loadModel(filename);
  //  model.printData();

    model.normalize();
//    model.printData();
    model.autoFocus();
  //  model.camera.y += 100;
  //  model.printData();
 //   model.camera = Vector3d(100,100,100);

    model.setViewCoordinate();
 //   model.printData();
    model.scaleFactor = 20;

    model.viewTransform();
  //  model.printData();

    model.lightSource = Vector3d(100,50,100);
    model.Ipoint = 5;
    model.Iamb = 1;
    model.Ka = 0.5;
    model.Kd = 1;
    model.Ks = 0.9;
    model.ns = 10;

  //  model.rotate(30);
  //  model.printData();

    RenderWindow window;
    window.create(VideoMode(windowX,windowY),"3d Model Renderer");
    window.setFramerateLimit(10);
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
        model.rotate(-2);
        model.renderModel();
        model.drawModel(window);
        window.display();
    }


return 0;
}
