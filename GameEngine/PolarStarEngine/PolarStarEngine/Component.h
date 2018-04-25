#pragma once

class GameObject;

class Component {
public:
	GameObject * game_object = nullptr;
	virtual void start() {};
	virtual void update(float delta_time) {};
};