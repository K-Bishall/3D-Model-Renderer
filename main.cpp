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

    model.scaleFactor = 20;
    model.viewTransform();

    //model.lightSource = Vector3d(8000,0,-11000);
    model.Ipoint = 2;
    model.Iamb = 1;
    model.Ka = 0.3;
    model.Kd = 0.5;
    model.Ks = 0.9;
    model.ns = 10;

    RenderWindow window;
    window.create(VideoMode(windowX,windowY),"3d Model Renderer");
    window.setFramerateLimit(30);
  //  window.setKeyRepeatEnabled(false);

  //  model.project();
   // model.renderModel();

    //model.rotate(0);

    bool r = false;

    while(window.isOpen())
    {
        Event event;
        while(window.pollEvent(event)) {
            if(event.type == Event::Closed)
                window.close();
            if(event.type == Event::KeyPressed) {
                switch(event.key.code) {

                    case Keyboard::Left :
                        model.lightSource.x -= 100;
                        break;

                    case Keyboard::Right:
                        model.lightSource.x += 100;
                        break;

                    case Keyboard::Up:
                        model.lightSource.y += 100;
                        break;

                    case Keyboard::Down:
                        model.lightSource.y -= 100;
                        break;

                    case Keyboard::Z:
                        model.lightSource.z += 100;
                        break;
                    case Keyboard::X:
                        model.lightSource.z -= 100;
                        break;

                    case Keyboard::Add:
                        model.scale(1.05);
                        break;
                    case Keyboard::Subtract:
                        model.scale(0.95);
                        break;

                    case Keyboard::Numpad1:
                        model.rotateX(2);
                        break;

                    case Keyboard::Numpad2:
                        model.rotateX(-2);
                        break;

                    case Keyboard::Numpad4:
                        model.rotateY(2);
                        break;
                    case Keyboard::Numpad5:
                        model.rotateY(-2);
                        break;
                    case Keyboard::Numpad7:
                        model.rotateZ(2);
                        break;
                    case Keyboard::Numpad8:
                        model.rotateZ(-2);
                        break;

                    case Keyboard::Space:
                        if(!r) r = true;
                        else r = false;
                        break;
                }
            }
        }

        window.clear(Color::Black);
    //   model.rotateX(-2);
        if(r) model.rotateY(2);
       //model.rotateZ(2);
        model.project();
        model.renderModel();
        model.drawModel(window);
        window.display();
    }
return 0;
}
