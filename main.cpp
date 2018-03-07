#include<iostream>
using namespace std;

#include "Model.h"
#include "Vector3d.h"

#include "SFML/Graphics.hpp"
using namespace sf;

int main()
{
    Model model;
    const char filename[] = "A380.obj";
    model.loadModel(filename);

    model.normalize();
    model.autoFocus();


    model.setViewCoordinate();
    model.scaleFactor = 10;

    model.viewTransform();

    model.lightSource = Vector3d(-100,100,100);
    model.Ipoint = 3;
    model.Iamb = 1;
    model.Ka = 0.75;
    model.Kd = 0.9;
    model.Ks = 0.3;
    model.ns = 16;

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
        model.rotate(-2);
        model.renderModel();
        model.drawModel(window);
        window.display();
    }


return 0;
}
