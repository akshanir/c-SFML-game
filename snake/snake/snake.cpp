// snake.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
#include <deque>

using namespace std;
using namespace sf;

int main()
{
    srand(time(0));
    RenderWindow window(VideoMode(960, 480), "Snake");

    Clock clk;
    float timer = 0, delay = 10;

    double T = 0, T_p_wave, T_p_QSeg, T_p_QInt, T_QRS, T_Q_TInt, T_T_Wave, T_S_TSeg;
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        float time = clk.getElapsedTime().asSeconds();
        timer += time;
        clk.restart();

        if (timer > delay) {
            timer = 0;
            //play
            T_p_wave = 0.37 * sqrt(T) - 0.22 * T - 0.06;
            T_p_QSeg = 0.33 * sqrt(T) - 0.18 * T - 0.08;
            T_p_QInt = 0.69 * sqrt(T) - 0.39 * T - 0.14;
            T_QRS    = 0.25 * sqrt(T) - 0.16 * T - 0.02;
            T_Q_TInt = 1.21 * sqrt(T) - 0.53 * T - 0.31;
            T_T_Wave = 1.06 * sqrt(T) - 0.51 * T - 0.33;
            T_S_TSeg = -0.09 * sqrt(T) + 0.13 * T + 0.04;
            cout << T << ' ' << T_p_wave << ' '
                << T_p_QSeg << ' '
                << T_p_QInt << ' '
                << T_QRS << ' '
                << T_Q_TInt << ' '
                << T_T_Wave << ' '
                << T_S_TSeg << '\n';
            T += 0.1;
        }
                

        //draw
        window.clear();
        window.display();
    }
    std::cout << "Hello World!\n";
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
