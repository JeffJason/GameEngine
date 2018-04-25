#include "BoxCollider.hpp"
#include "GameObject.h"

BoxCollider::BoxCollider(float half_width, float half_height) {
	this->half_width = half_width;
	this->half_height = half_height;
}

void BoxCollider::start() {}

void BoxCollider::update(float delta_time) {

	this->bottom_left = this->game_object->transform->getPosition();
	this->bottom_left.x -= this->half_width;
	this->bottom_left.y -= this->half_height;

	this->top_right = this->game_object->transform->getPosition();
	this->top_right.x += this->half_width;
	this->top_right.y += this->half_height;
}