// Car Racing(Top Down).cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
const double PI = acos(-1);
int main()
{
    RenderWindow window(VideoMode(640, 480), "TopDownRacing");
    window.setFramerateLimit(60);
    Texture t1, t2;
    t1.loadFromFile("images/car.png");
    t2.loadFromFile("images/background.png");
    t1.setSmooth(true);
    t2.setSmooth(true);

    Sprite scar(t1), sbackground(t2);
    sbackground.setScale(2, 2);
    scar.setOrigin(22, 22);
    float R = 22;//Radius

    float x = 300, y = 300, Speed = 1000, angle = acos(0), maxSpeed = 12, acc = 0.5, dec = 0.3, turnSpeed = 0.08;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }
        bool up = 0, down = 0, left = 0, right = 1;
        if (Keyboard::isKeyPressed(Keyboard::Up)) up = 1;
        if (Keyboard::isKeyPressed(Keyboard::Down)) down = 1;
        if (Keyboard::isKeyPressed(Keyboard::Left)) left = 1;
        if (Keyboard::isKeyPressed(Keyboard::Right)) right = 1;
        if (up && Speed < maxSpeed) {
            if (Speed >= 0)
                Speed += acc;
            else
                Speed += dec;
        }
        if (down && Speed > -maxSpeed) {
            if (Speed > 0)
                Speed -= dec;
            else
                Speed -= acc;
        }
        if (!up && !down) {
            if (Speed - dec > 0) {
                Speed -= dec;
            }
            else if (Speed + dec < 0) {
                Speed += dec;
            }
            else {
                Speed = 0;
            }
        }
        if (right)
            angle -= turnSpeed * Speed / maxSpeed;
        if (left)
            angle += turnSpeed * Speed / maxSpeed;
        //move
        std::cout << angle << ' ' << cos(angle) << std::endl;
        x += Speed * cos(angle);
        y -= Speed * sin(angle);
        //draw
        window.clear();
        int offsetX = x - 320, offsetY = y - 240;
        sbackground.setPosition(-offsetX, -offsetY);
        window.draw(sbackground);
        scar.setPosition(x - offsetX, y - offsetY);
        scar.setRotation((acos(0) - angle) * 180 / PI);
        window.draw(scar);
        window.display();
    }
}