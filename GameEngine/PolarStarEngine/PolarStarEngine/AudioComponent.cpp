#include "AudioComponent.h"

sf::SoundBuffer AudioComponent::buffer;
sf::Sound AudioComponent::sound;
sf::Music AudioComponent::music;

void AudioComponent::playSound(std::string filename, int vol)
{
	if (!buffer.loadFromFile(filename))
	{
		std::cout << "cannot play sound" << std::endl;
		return;
	}

	sound.setBuffer(buffer);
	sound.setVolume(vol);
	sound.play();
}

////////////////////////////////////////////////////////////
/// Play an music
///
////////////////////////////////////////////////////////////
void AudioComponent::playMusic(std::string filename, int vol)
{
	if (!music.openFromFile(filename))
	{
		std::cout << "cannot play music" << std::endl;
		return;
	}

	music.setVolume(vol);
	music.setLoop(true);
	music.play();
}