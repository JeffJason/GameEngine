#include "SplashScreen.h"


sf::Sprite Splash::SplashSprite;
sf::Texture Splash::SplashImage;


void Splash::setSprite(std::string filename)
{

	if (!SplashImage.loadFromFile(filename))
	{
		std::cout << "Error could not load splash screen image" << std::endl;
		return;
	}


	SplashImage.setSmooth(true);
	SplashSprite.setTexture(SplashImage);
	SplashSprite.setPosition(sf::Vector2f(125, 15));
}

void Splash::drawSprite(sf::RenderWindow *render_window)
{

	render_window->draw(SplashSprite);

}

