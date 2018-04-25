#include "Component.h"
#include <SFML\Audio.hpp>
#include <iostream>
#include <string>


class AudioComponent {
public:
	AudioComponent() {};

	void playSound(std::string filename, int vol);
	void playMusic(std::string filename, int vol);

	static sf::SoundBuffer buffer;
	static sf::Sound sound;
	static sf::Music music;
};