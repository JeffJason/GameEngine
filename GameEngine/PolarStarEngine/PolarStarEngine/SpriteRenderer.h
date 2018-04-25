#pragma once

#include <SFML/Graphics.hpp>
#include "Component.h"

class SpriteRenderer : public Component {


public:
	void start() override;
	void update(float delta_time) override;
	void set_sprite(std::string image_path);
	sf::Sprite get_sprite();

private:
	sf::Sprite sprite;
	sf::Texture sprite_texture;
};