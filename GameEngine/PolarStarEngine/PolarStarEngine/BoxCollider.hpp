#pragma once
#include <SFML/Graphics.hpp>

#include "Component.h"


class BoxCollider : public Component {

public:
	BoxCollider(float half_width, float half_height);
	void start() override;
	void update(float delta_time) override;


public:
	sf::Vector2f bottom_left;
	sf::Vector2f top_right;

	float half_width;
	float half_height;
};