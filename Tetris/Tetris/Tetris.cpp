#include <iostream>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <string>
using namespace sf;

const int N = 20, M = 10, B = 18;// N -> height M -> width B blocksize

int g[N][M];

struct point { int x, y; };

class piece{
    public:
    point a[4];//every piece has 4 squares
    int type;
        piece() {
            Reset(rand() % 7);
        }
        piece(piece &another) {
            type = another.type;
            for (int i = 0; i < 4; ++i) {
                a[i] = another.a[i];
            }
        }
        void operator =(piece &another) {
            type = another.type;
            for (int i = 0; i < 4; ++i) {
                a[i] = another.a[i];
            }
        }
        void Move_Right() {
            bool can = 1;
            for (int i = 0; i < 4; ++i) {
                if (a[i].x + 1 == M || g[a[i].y][a[i].x + 1] < 10) {
                    can = 0;
                }
            }
            for (int i = 0; can && i < 4; ++i) {
                ++ a[i].x;
            }
        }

        void Move_Left() {
            bool can = 1;
            for (int i = 0; i < 4; ++i) {
                if (a[i].x - 1 < 0 || g[a[i].y][a[i].x - 1] < 10) {
                    can = 0;
                }
            }
            for (int i = 0; can && i < 4; ++i) {
                -- a[i].x;
            }
        }

        void Move_Down() {
            bool can = 1;
            for (int i = 0; i < 4; ++i) {
                if (a[i].y + 1 == N || a[i].y >= 0 && g[a[i].y + 1][a[i].x] < 10) {
                    can = 0;
                }
            }
            for (int i = 0; can && i < 4; ++i) {
                ++ a[i].y;
            }
        }

        void Clear_from_grid() {
            for (int i = 0; i < 4; ++i) {
                g[a[i].y][a[i].x] = 0;
            }
        }

        void Add_to_grid() {
            for (int i = 0; i < 4; ++i) {
                g[a[i].y][a[i].x] = type;
            }
        }

        bool Touch_the_ground() {
            for (int i = 0; i < 4; ++i) {
                if (g[a[i].y + 1][a[i].x] < 10 || a[i].y + 1 == N)
                    return 1;
            }
            return 0;
        }

        void Rotate() {//I had alot of ideas but this seems the easiest and am lazy
            //note that a[1] is the center of rotation
            point p = a[1];
            bool can = 1;
            for (int i = 0; i < 4; ++i) {
                int x = a[i].y - p.y;
                int y = a[i].x - p.x;
                x = p.x - x;
                y = p.y + y;
                if (x == M || y == N || x < 0 || y < 0 || g[y][x] < 10)
                    can = false;
            }

            for (int i = 0; can && i < 4; ++i) {
                int x = a[i].y - p.y;
                int y = a[i].x - p.x;
                a[i].x = p.x - x;
                a[i].y = p.y + y;
            }
        }
        void Reset(int t) {
            type = t;
            switch (type) {
            case 0://I
                a[0] = { 3, -2 };
                a[1] = { 4, -2 };
                a[2] = { 5, -2 };
                a[3] = { 6, -2 };
                break;
            case 1://J
                a[0] = { 3, -2 };
                a[1] = { 4, -2 };
                a[2] = { 5, -2 };
                a[3] = { 3, -1 };
                break;
            case 2://L
                a[0] = { 3, -1 };
                a[1] = { 4, -1 };
                a[2] = { 5, -1 };
                a[3] = { 3, -2 };
                break;
            case 3://O
                a[0] = { 4, -2 };
                a[1] = { 4, -1 };
                a[2] = { 5, -2 };
                a[3] = { 5, -1 };
                break;
            case 4://S
                a[0] = { 3, -2 };
                a[1] = { 4, -2 };
                a[2] = { 4, -1 };
                a[3] = { 5, -1 };
                break;
            case 5://Z
                a[0] = { 3, -1 };
                a[1] = { 4, -1 };
                a[2] = { 4, -2 };
                a[3] = { 5, -2 };
                break;
            case 6://T
                a[0] = { 3, -2 };
                a[1] = { 4, -2 };
                a[2] = { 5, -2 };
                a[3] = { 4, -1 };
                break;
            }
        }
};

void Reset_Grid();
void Fix_Lines(int &Score);

int main()
{
    srand(time(0));
    RenderWindow window(VideoMode(M * 2 * B + 6 * B/*for the score*/, N * 2 * B), "Tetris");

    Font font;
    font.loadFromFile("images/CloisterBlack.ttf");
    Text Score_Text, Next_Text, Hold_Text;
    Score_Text.setFont(font);
    Next_Text.setFont(font);
    Hold_Text.setFont(font);
    Score_Text.setFillColor(Color::Black);
    Next_Text.setFillColor(Color::Black);
    Hold_Text.setFillColor(Color::Black);
    Next_Text.setString("Next: ");
    Hold_Text.setString("Hold: ");
    Next_Text.setPosition(M * 2 * B + B, 2 * 2 * B);
    Hold_Text.setPosition(M * 2 * B + B, 7 * 2 * B);

    Texture t1, t2;
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/white.png");
    Sprite s1(t1), s2(t2);
    s1.setScale(Vector2f(2, 2));// the size is bigger than the image size
    s2.setScale(Vector2f(2.0 * B / 16, 2.0 * B / 16));

    Clock clock;
    float time = 0, delay = 0.3;

    bool Rotate = 0;
    int dx = 0;

    Reset_Grid();
    piece test, next, hold;

    int Score = 0;

    while (window.isOpen()) {
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                window.close();
            }
            if (e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::Right) {
                    dx = 1;
                }
                else if (e.key.code == Keyboard::Left) {
                    dx = -1;
                }
                else if (e.key.code == Keyboard::Up) {
                    Rotate = 1;
                }
                else if (e.key.code == Keyboard::Down) {
                    delay = 0.05;
                }
                else if (e.key.code == Keyboard::P) {

                }
            }
        }

        float timer = clock.restart().asSeconds();
        time += timer;
        if (time > delay) {
            time = 0;
            delay = 0.3;
            if (test.Touch_the_ground()) {
                test.Add_to_grid();
                test = next;//Remember to edit this later so the player can move during the fram it hits the ground
                next.Reset(rand()%7);
                Fix_Lines(Score);
            }
            test.Move_Down();
        }
        //moving left or right time is 0
        if (Rotate) {
            test.Rotate();
        }
        else if (dx == 1) {
            test.Move_Right();
        }
        else if (dx == -1) {
            test.Move_Left();
        }  
        //moves are instant
        dx = 0;
        Rotate = 0;
        /// Draw
        window.clear(Color::White);
        std::string score = "Score: \n  " + std::to_string(Score);
        Score_Text.setString(score);
        Score_Text.setPosition(M*2*B+B,0);
        window.draw(Score_Text);
        window.draw(Next_Text);
        window.draw(Hold_Text);

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < M; ++j) {
                if (g[i][j] == 10) {
                    s2.setPosition(j * 2 * B, i * 2 * B);
                    window.draw(s2);
                    continue;
                }
                int colour = g[i][j];
                s1.setTextureRect(IntRect(colour * B, 0, B, B));
                s1.setPosition(j * 2 * B, i * 2 * B);
                window.draw(s1);
            }
        }
        //the grid is constant so we would have to draw the active piece separately
        for (int i = 0; i < 4; ++i) {
            int colour = test.type;
            s1.setTextureRect(IntRect(colour * B, 0, B, B));
            s1.setPosition(test.a[i].x * 2 * B, test.a[i].y * 2 * B);
            window.draw(s1);
        }
        //now we draw the next piece
        for (int i = 0; i < 4; ++i) {
            int colour = next.type;
            s1.setTextureRect(IntRect(colour * B, 0, B, B));
            s1.setPosition(next.a[i].x * 2 * B + 7 * 2 * B, next.a[i].y * 2 * B + 6 * 2 * B);
            window.draw(s1);
        }
        //now we draw the hold piece
        for (int i = 0; i < 4; ++i) {
            int colour = hold.type;
            s1.setTextureRect(IntRect(colour * B, 0, B, B));
            s1.setPosition(hold.a[i].x * 2 * B + 7 * 2 * B, hold.a[i].y * 2 * B + 11 * 2 * B);
            window.draw(s1);
        }
        window.display();
    }
    
}

void Reset_Grid() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            g[i][j] = 10; 
        }
    }
}

void Fix_Lines(int &Score) {
//this is a lazy implementation fix it if you need to speed things up
    
    for (int i = 0; i < N; ++i) {
        bool need_fix = 1;
        for (int j = 0; j < M; ++j) {
            if (g[i][j] == 10)
                need_fix = 0;
        }
        for (int k = i; need_fix && k >= 0; --k) {
            for (int j = 0; j < M; ++j) {
                if (k == 0)
                    g[k][j] = 10;
                else
                    g[k][j] = g[k - 1][j];
            }
        }
        Score += need_fix;
    }
}










