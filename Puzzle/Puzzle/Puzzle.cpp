// Puzzle.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

int g[4][4] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0};
int main()
{
    RenderWindow window(VideoMode(256, 256), "15-Puzzle");
    window.setFramerateLimit(64);
    Texture t1;
    t1.loadFromFile("images/15.png");
    Sprite s[16];
    for (int i = 0, n = 1; i < 4; ++i) {
        for (int j = 0; j < 4; ++j, ++n) {
            s[n%16].setTexture(t1);
            s[n%16].setTextureRect(IntRect(j * 64, i * 64, 64, 64));
        }
    }
    int x = 0, y = 0;
    while (window.isOpen()) {
        x = Mouse::getPosition(window).x / 64, y = Mouse::getPosition(window).y / 64;
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::MouseButtonPressed && event.key.code == Mouse::Left) {
                for (int i = 0, yes = 1; yes && i < 4; ++i) {
                    for (int j = 0; j < 4; ++j) {
                        if (g[i][j] == 0 && abs(i-y) + abs(j-x) == 1) { //I'm so fucking lazy
                            //Animations
                            int n = g[y][x], speed = 1, dx = j - x, dy = i - y;
                            s[0].setPosition(x * 64, y * 64);
                            for (int k = 0; k < 64 / 3; ++k) {
                                s[g[y][x]].move(speed * dx, speed * dy);
                                window.draw(s[0]);
                                window.draw(s[g[y][x]]);
                                window.display();// here where the fram goes out
                            }
                            yes = 0;
                            std::swap(g[y][x], g[i][j]);
                            break;
                        }
                    }
                }
            }

        }
        window.clear(Color::White);
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                s[g[i][j]].setPosition(j * 64, i * 64);
                window.draw(s[g[i][j]]);
            }
        }
        window.display();
    }

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
