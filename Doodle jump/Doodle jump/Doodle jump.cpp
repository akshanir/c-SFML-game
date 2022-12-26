#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
using namespace sf;

struct point { int x, y; };
struct something {
	float x, y, width, height;
	bool spring;
};
float dx, dy, h = 200;

int main() {
	srand(time(0));

	RenderWindow window(VideoMode(400, 533), "Doodle Jump");
	window.setFramerateLimit(60);
	Texture t1, t2, t3;
	t1.loadFromFile("images/background.png");
	t2.loadFromFile("images/doodle.png");
	t3.loadFromFile("images/platform.png");

	Sprite sbackground(t1), sdoodle(t2), splatform(t3);

	something doodle = { 0, 400, 80, 80 };
	something platform[10];

	int d = 533 / 10;
	for (int i = 0; i < 10; ++i) {
		platform[i].width = 68;
		platform[i].height = 14;
		platform[i].y = i * d + d - 10;
		platform[i].x = rand() % (400-68);
		platform[i].spring = !(rand()%10);
	}
	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}
			
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) doodle.x += 3;
		if (Keyboard::isKeyPressed(Keyboard::Left)) doodle.x -= 3;
		if (doodle.x + doodle.width > 400) {
			doodle.x = 400 - doodle.width;
		}
		if (doodle.x < 0) {
			doodle.x = 0;
		}
		dy += 0.2;
		doodle.y += dy;
		if (doodle.y + doodle.height >= 533)
			dy = -10;
		if (doodle.y < h) {
			for (int i = 0; i < 10; ++i) {
				doodle.y = h;
				platform[i].y -= dy;
				if (platform[i].y + platform[i].height > 533) {
					platform[i].x = rand() % (400 - 68);
					platform[i].y = 0;
					platform[i].spring = !(rand() % 10);
				}
			}
		}
		for (int i = 0; i < 10; ++i) {
			if (dy > 0 && doodle.x + 50 >= platform[i].x && doodle.x + 20 <= platform[i].x + platform[i].width
				&& doodle.y + 70 > platform[i].y && doodle.y + 70  < platform[i].y+14) {
				dy = -10;
				if (platform[i].spring)
					dy = -20;
			}
		}
		sdoodle.setPosition(doodle.x, doodle.y);
		//draw
		window.clear();
		window.draw(sbackground);
		
		for (int i = 0; i < 10; ++i) {
			splatform.setPosition(platform[i].x, platform[i].y);
			if(platform[i].spring)
				splatform.setColor(Color::Black);
			else
				splatform.setColor(Color::White);
			window.draw(splatform);
		}
		window.draw(sdoodle);
		window.display();
	}
}