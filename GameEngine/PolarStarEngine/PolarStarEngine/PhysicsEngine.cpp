#include "PhysicsEngine.h"
#include <cmath>
#include <algorithm>
#include <numeric>
#include <SFML\System\Vector2.hpp>
#include <iostream>


PhysicsEngine* PhysicsEngine::physics_engine_instance;

PhysicsEngine::PhysicsEngine() {
	PhysicsEngine::physics_engine_instance = this;
}

void PhysicsEngine::add_rigidBody(Rigidbody* object_rigidbody) {
	rigidbodies_list.push_back(object_rigidbody);
}

void PhysicsEngine::update_gameobjects_physics(float delta_time)
{
	for each (Rigidbody *rigidbody in rigidbodies_list)
	{
		rigidbody->update_phyisics(delta_time);
	}
}

bool PhysicsEngine::is_grounded(Rigidbody* object_rigidbody)
{
	for each (Rigidbody *rigidbody in rigidbodies_list)
	{
		if (rigidbody != object_rigidbody)
		{
			if (object_rigidbody->aabb.bottom_left.x < rigidbody->aabb.top_right.x
				&& object_rigidbody->aabb.top_right.x > rigidbody->aabb.bottom_left.x
				&& abs(object_rigidbody->aabb.bottom_left.y - rigidbody->aabb.top_right.y) <= groundedTol)
			{
				if (abs(object_rigidbody->current_velocity.y) < groundedTol)
					return true;
			}
		}
	}
	return false;
}

void PhysicsEngine::check_collisions()
{
	for (std::list<Rigidbody*>::iterator rigidbody_a = rigidbodies_list.begin(); rigidbody_a != rigidbodies_list.end(); rigidbody_a++) {
		for (std::list<Rigidbody*>::iterator rigidbody_b = rigidbody_a; rigidbody_b != rigidbodies_list.end(); rigidbody_b++) {

			if (rigidbody_a == rigidbody_b)
				continue;
			else
			{
				CollisionPair *pair = new CollisionPair();
				CollisionInfo *info = new CollisionInfo();

				pair->object_rigidbody_a = *rigidbody_a;
				pair->object_rigidbody_b = *rigidbody_b;

				sf::Vector2f distance = (*rigidbody_b)->game_object->transform->getPosition() - (*rigidbody_a)->game_object->transform->getPosition();

				sf::Vector2f half_size_a = ((*rigidbody_a)->aabb.top_right - (*rigidbody_a)->aabb.bottom_left) * 0.5f;
				sf::Vector2f half_size_b = ((*rigidbody_b)->aabb.top_right - (*rigidbody_b)->aabb.bottom_left) * 0.5f;

				sf::Vector2f gap = sf::Vector2f(abs(distance.x), abs(distance.y)) - (half_size_a + half_size_b);

				if (gap.x < 0 && gap.y < 0) {

					if (gap.x > gap.y) {
						if (distance.x > 0) {
							info->collisonNormal = sf::Vector2f(1, 0);
						}
						else {
							info->collisonNormal = sf::Vector2f(-1, 0);
						}
						info->collide = gap.x;
					}
					else {
						if (distance.y > 0) {
							info->collisonNormal = sf::Vector2f(0, 1);
						}
						else {
							info->collisonNormal = sf::Vector2f(0, -1);
						}
						info->collide = gap.y;
					}
					collision_list.insert(std::pair<CollisionPair*, CollisionInfo*>(pair, info));
				}
			}
		}
	}
}
/*
void PhysicsEngine::check_collisions()
{
for (std::list<Rigidbody*>::iterator rigidbody_a = rigidbodies_list.begin(); rigidbody_a != rigidbodies_list.end(); rigidbody_a++) {
for (std::list<Rigidbody*>::iterator rigidbody_b = rigidbody_a; rigidbody_b != rigidbodies_list.end(); rigidbody_b++) {

if (rigidbody_a == rigidbody_b)
continue;
else
{
pair->object_rigidbody_a = *rigidbody_a;
pair->object_rigidbody_b = *rigidbody_b;

sf::Vector2f distance = (*rigidbody_b)->game_object->transform->getPosition() - (*rigidbody_a)->game_object->transform->getPosition();

sf::Vector2f half_size_a = ((*rigidbody_a)->aabb.top_right - (*rigidbody_a)->aabb.bottom_left) * 0.5f;
sf::Vector2f half_size_b = ((*rigidbody_b)->aabb.top_right - (*rigidbody_b)->aabb.bottom_left) * 0.5f;

sf::Vector2f gap = sf::Vector2f(abs(distance.x), abs(distance.y)) - (half_size_a + half_size_b);

std::map<CollisionPair*, CollisionInfo*>::iterator it = collision_list.find(pair);

if (gap.x < 0 && gap.y < 0) {
if (it != collision_list.end())
{
//std::map<CollisionPair*, CollisionInfo*>::iterator it = collision_list.find(pair);
collision_list.erase(it);
}

if (gap.x > gap.y) {
if (distance.x > 0) {
info->collisonNormal = sf::Vector2f(1, 0);
}
else {
info->collisonNormal = sf::Vector2f(-1, 0);
}
info->penatration = gap.x;
}
else {
if (distance.y > 0) {
info->collisonNormal = sf::Vector2f(0, 1);
}
else {
info->collisonNormal = sf::Vector2f(0, -1);
}
info->penatration = gap.y;
}
collision_list.insert(std::pair<CollisionPair*, CollisionInfo*>(pair, info));
}
else if(it != collision_list.end())
{
collision_list.erase(it);
}
}
}
}
}
*/


void PhysicsEngine::resolve_collisons()
{
	for (std::map<CollisionPair*, CollisionInfo*>::iterator it = collision_list.begin(); it != collision_list.end(); it++)
	{
		float min_bounce = std::min(it->first->object_rigidbody_a->bounciness, it->first->object_rigidbody_b->bounciness);

		sf::Vector2f temp = it->first->object_rigidbody_b->current_velocity - it->first->object_rigidbody_a->current_velocity;
		float vel_along_normal = (temp.x * it->second->collisonNormal.x) + (temp.y * it->second->collisonNormal.y);

		if (vel_along_normal > 0)
			continue;

		float j = -(1 + min_bounce) * vel_along_normal;

		float inv_mass_a, inv_mass_b;

		//Handle inv mass a
		if (it->first->object_rigidbody_a->mass == 0)
			inv_mass_a = 0;
		else
			inv_mass_a = 1 / it->first->object_rigidbody_a->mass;

		//Handle inv mass b
		if (it->first->object_rigidbody_b->mass == 0)
			inv_mass_b = 0;
		else
			inv_mass_b = 1 / it->first->object_rigidbody_b->mass;

		j /= inv_mass_a + inv_mass_b;

		sf::Vector2f impulse = j * it->second->collisonNormal;

		if (it->first->object_rigidbody_a->mass != 0)
			it->first->object_rigidbody_a->current_velocity -= 1 / it->first->object_rigidbody_a->mass * impulse;

		if (it->first->object_rigidbody_b->mass != 0)
			it->first->object_rigidbody_b->current_velocity += 1 / it->first->object_rigidbody_b->mass * impulse;

		if (abs(it->second->collide) > 0.01f)
			correct_positions(it->first);
	}
}

void PhysicsEngine::correct_positions(CollisionPair *pair)
{
	const float percent = 0.2f;

	float inv_mass_a, inv_mass_b;

	//Handle inv mass a
	if (pair->object_rigidbody_a->mass == 0)
		inv_mass_a = 0;
	else
		inv_mass_a = 1 / pair->object_rigidbody_a->mass;

	//Handle inv mass b
	if (pair->object_rigidbody_b->mass == 0)
		inv_mass_b = 0;
	else
		inv_mass_b = 1 / pair->object_rigidbody_b->mass;

	sf::Vector2f correction = ((collision_list.find(pair)->second->collide / (inv_mass_a + inv_mass_b)) * percent) * -collision_list.find(pair)->second->collisonNormal;

	sf::Vector2f temp = pair->object_rigidbody_a->game_object->transform->getPosition();
	temp -= inv_mass_a * correction;
	pair->object_rigidbody_a->game_object->transform->setPosition(temp);

	temp = sf::Vector2f(pair->object_rigidbody_b->game_object->transform->getPosition());
	temp += inv_mass_b * correction;
	pair->object_rigidbody_b->game_object->transform->setPosition(temp);
}

void PhysicsEngine::updates_physics(float delta_time)
{
	update_gameobjects_physics(delta_time);
	check_collisions();
	resolve_collisons();
	collision_list.clear();
}