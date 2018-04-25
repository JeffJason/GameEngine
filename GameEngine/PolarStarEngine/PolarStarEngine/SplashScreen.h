#pragma once
#include "SFML\Graphics.hpp"
#include <iostream>
#include <string>
#include "Component.h"

class Splash : public Component {

public:
	//Splash();

	void setSprite(std::string filename);
	void drawSprite(sf::RenderWindow *window);

	static sf::Texture SplashImage;
	static sf::Sprite SplashSprite;

	bool isDrawn = false;
	bool showSplash = false;
};