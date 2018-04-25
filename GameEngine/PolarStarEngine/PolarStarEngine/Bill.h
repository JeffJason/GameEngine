#pragma once

#include <SFML/Graphics.hpp>

#include "GameObject.h"
#include "BoxCollider.hpp"
#include "Rigidbody.h"

//typedef void key_function();

class Bill : public GameObject {

public:
	Bill();
	void rotate_left();
	void rotate_right();
	void thrust();
	virtual void update(float delta_time) override;
	static sf::RenderWindow *render_window;

private:
	Rigidbody * rigidbody;
	float move_speed;
	const sf::Vector2f jump_force = sf::Vector2f(0.0f, -0.01f);
	bool isFiring;
};