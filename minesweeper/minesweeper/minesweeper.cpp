#include <iostream>
#include <SFML/Graphics.hpp>
#include <time.h>
using namespace std;
using namespace sf;

const int w = 32, mines = 90, width = 40, height = 24;
int g[height][width], sg[height][width];
bool AI_Active = true;
//AI
double prop[height][width];

void generate_grid();
void reset_showgrid();
void reveal_grid();
void set_AI();
void apply_AI(int &x, int &y);

int main()
{
    srand(time(0));
    RenderWindow window(VideoMode(w * width, w * height), "minesweeper");
    Texture t;
    t.loadFromFile("tiles.jpg");
    Sprite s(t);
    //AI timer
    Clock clock;
    float time = 0, delay = 0.01;

    generate_grid();
    reset_showgrid();
    set_AI();

    while (window.isOpen()) {
        Vector2i pos = Mouse::getPosition(window);
        int x = pos.x / w, y = pos.y/w;
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (!AI_Active && event.type == Event::MouseButtonPressed) {
                if (event.key.code == Mouse::Left) {
                    if(sg[y][x] == 10)
                        sg[y][x] = g[y][x];
                    if (g[y][x] == 9) {
                        reveal_grid();
                    }
                }
                else if (event.key.code == Mouse::Right) {
                    if (sg[y][x] == 10)
                        sg[y][x] = 11;
                    else if (sg[y][x] == 11)
                        sg[y][x] = 10;
                }
            }
        }
        if (AI_Active)
        {

            //AI timer
            float timer = clock.getElapsedTime().asSeconds();
            clock.restart();
            time += timer;
            if (time < delay) {
                continue;
            }
            time = 0;

            x = rand() % width, y = rand() % height;
            apply_AI(x, y);
            //in AI we only need Left mouse clicks
            if (sg[y][x] == 10 && prop[y][x] < 99.9) {
                sg[y][x] = g[y][x];
                prop[y][x] = 0;
            }
        }
        //printing
        window.clear();
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                s.setTextureRect(IntRect(sg[i][j] * w, 0, w, w));
                s.setPosition(j*w, i*w);//x , w fuckthisshit
                window.draw(s);
            }
        }
        window.display();
        if (sg[y][x] == 9) {
            while (window.isOpen()) {
                while (window.pollEvent(event)) {
                    if (event.type == Event::Closed) {
                        window.close();
                    }
                }
            }
            reveal_grid();
        }
    }
}

void generate_grid() {
    //mines
    for (int i = 0; i < mines; ++i) {
        int x = rand() % width, y = rand() % height;
        while (g[y][x] == 9) {
            x = rand() % width, y = rand() % height;
        }
        g[y][x] = 9;
    }
    //counting
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (g[y][x] == 9)
                continue;
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    if (y + i < 0 || y + i == height || x + j < 0 || x + j == width)
                        continue;
                    g[y][x] += g[y + i][x + j] == 9;
                }
            }
        }
    }
}

void reset_showgrid() {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            sg[i][j] = 10;
        }
    }
}

void reveal_grid() {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            sg[i][j] = g[i][j];
        }
    }
}

void set_AI() {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            prop[i][j] = 50; //50%
        }
    }
}
void apply_AI(int &x, int &y) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (sg[i][j] >= 9)
                continue;
            if (sg[i][j] == 0) {
                for (int k = -1; k <= 1; ++k) {
                    for (int l = -1; l <= 1; ++l) {
                        if (i+k < 0 || i+k == height || j+l < 0 || j+l == width)
                            continue;
                        prop[i + k][j + l] = 0.0;
                    }
                }
                continue;
            }
            int cnt = 0;
            for (int k = -1; k <= 1; ++k) {
                for (int l = -1; l <= 1; ++l) {
                    if (i + k < 0 || i + k == height || j + l < 0 || j + l == width)
                        continue;
                    cnt += sg[i + k][j + l] == 10;
                }
            }
            if (cnt == sg[i][j]) {
                for (int k = -1; k <= 1; ++k) {
                    for (int l = -1; l <= 1; ++l) {
                        if (i + k < 0 || i + k == height || j + l < 0 || j + l == width)
                            continue;
                        if (sg[i + k][j + l] == 10)
                            prop[i + k][j + l] = 100;
                    }
                }
                continue;
            }
            int bcnt = 0;
            for (int k = -1; k <= 1; ++k) {
                for (int l = -1; l <= 1; ++l) {
                    if (i + k < 0 || i + k == height || j + l < 0 || j + l == width)
                        continue;
                    bcnt += prop[i + k][j + l] > 99.9;
                }
            }
            if (bcnt == sg[i][j]) {
                for (int k = -1; k <= 1; ++k) {
                    for (int l = -1; l <= 1; ++l) {
                        if (i + k < 0 || i + k == height || j + l < 0 || j + l == width)
                            continue;
                        if (prop[i + k][j + l] < 99.9) {
                            prop[i + k][j + l] = 0;
                        }
                    }
                }
                continue;
            }
        }
    }
    double mn = 100;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (prop[i][j] < mn && sg[i][j] == 10) {
                y = i;
                x = j;
                mn = prop[i][j];
            }
        }
        
    }
}