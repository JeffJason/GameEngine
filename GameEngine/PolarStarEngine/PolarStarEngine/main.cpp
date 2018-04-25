#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "PolarStarEngine.h"

#include<vector>

int main()
{
	PolarStarEngine application;

	//application.display_splash_screen();
	application.main_loop();

	system("pause");

	sf::RenderWindow window({ 1024,709 }, "Hello there");
	window.setFramerateLimit(30);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.display();
	}
}