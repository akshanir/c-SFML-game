#include <iostream>
#include <SFML/Graphics.hpp>
using namespace sf;

bool collides(Sprite &s1, Sprite &s2) {
    return s1.getGlobalBounds().intersects(s2.getGlobalBounds());
}

int main()
{
    RenderWindow window(VideoMode(520, 450), "Arkanoid");
    window.setFramerateLimit(60);
    Texture t1, t2, t3, t4;
    t1.loadFromFile("images/block01.png");
    t2.loadFromFile("images/background.jpg");
    t3.loadFromFile("images/ball.png");
    t4.loadFromFile("images/paddle.png");
    Sprite sbackground(t2), sball(t3), spaddle(t4);
    Sprite block[100];
    spaddle.setPosition(300, 440);
    sball.setPosition(300, 400);
    for (int i = 1, n = 0; i <= 10; ++i) {
        for (int j = 1; j <= 10; ++j, ++n) {
            block[n].setPosition(i*43, j*20);
            block[n].setTexture(t1);
        }
    }

    float dx = 6, dy = 5, x = 300, y = 400;//(x, y) ball coardinates

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }
        bool cx = 0;
        x += dx;
        sball.move(dx, 0);
        for (int i = 0; i < 100; ++i) {
            if (collides(sball, block[i])) {
                cx = 1;
                dx = -dx;
                    block[i].setPosition(-100, 0);
            }
        }
        y += dy;
        sball.move(0, dy);
        for (int i = 0; !cx && i < 100; ++i) {
            if (collides(sball, block[i])) {
                dy = -dy;
                block[i].setPosition(-100, 0);
            }
        }
        if (x <= 0 || x >= 520) dx = -dx;
        if (y <= 0 || y >= 450) dy = -dy;
        if (Keyboard::isKeyPressed(Keyboard::Right)) spaddle.move(6, 0);
        if (Keyboard::isKeyPressed(Keyboard::Left)) spaddle.move(-6, 0);
        if (collides(sball, spaddle)) {
            dy = -dy;
        }
    
        //draw
        window.clear();
        window.draw(sbackground);
        window.draw(sball);
        window.draw(spaddle);
        for (int i = 0; i < 100; ++i) {
            window.draw(block[i]);
        }
        window.display();

    }
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
