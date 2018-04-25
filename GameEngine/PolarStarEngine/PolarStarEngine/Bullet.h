
#pragma once

#include <SFML/Graphics.hpp>

#include "GameObject.h"
#include "BoxCollider.hpp"
#include "Rigidbody.h"

//typedef void key_function();

class Bullet : public GameObject {

public:
	Bullet();
	void rotate_left();
	void rotate_right();
	void thrust();
	virtual void update(float delta_time) override;


private:
	Rigidbody * rigidbody;
	float move_speed;
	const sf::Vector2f jump_force = sf::Vector2f(0.0f, -0.01f);

};