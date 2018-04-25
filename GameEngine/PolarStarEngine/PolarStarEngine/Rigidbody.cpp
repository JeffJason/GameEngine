#include "Rigidbody.h"
#include "PhysicsEngine.h"
#include "iostream"
#include "SpriteRenderer.h"

Rigidbody::Rigidbody() {}

void Rigidbody::start() {
	engine = PhysicsEngine::physics_engine_instance;
	engine->add_rigidBody(this);
	this->box_collider = this->game_object->get_component<BoxCollider>();
}

bool Rigidbody::is_grounded() {
	grounded = engine->is_grounded(this);
	return(grounded);
}

void Rigidbody::set_aabb() {

	if (box_collider != nullptr)
	{
		aabb.bottom_left = box_collider->bottom_left;
		aabb.top_right = box_collider->top_right;
	}
}

void Rigidbody::stop(bool stop_x_axis, bool stop_y_axis) {

	if (stop_x_axis)
	{
		current_velocity.x = 0.0f;
		total_forces.x = 0.0f;
	}
	if (stop_y_axis)
	{
		current_velocity.y = 0.0f;
		total_forces.y = 0.0f;
	}

}

void Rigidbody::add_force(sf::Vector2f force) {
	total_forces += force;
}

void Rigidbody::update(float delta_time) {
}

void Rigidbody::update_phyisics(float delta_time) {

	if (obeys_gravity && !grounded)
		add_force(gravity);
	else
	{
		if (abs(current_velocity.y) < 0.05f)
			current_velocity.y = 0;
	}

	if (mass != 0)
	{
		sf::Vector2f accleration = total_forces / mass;

		if (mass == 0)
			sf::Vector2f accleration = sf::Vector2f(0.0f, 0.0f);
		else
			sf::Vector2f accleration = total_forces / mass;

		current_velocity += accleration * delta_time;

		//Handle max velocity
		if (current_velocity.x > 0)
			current_velocity.x = std::min(current_velocity.x, max_velocity.x);
		else if (current_velocity.x < 0)
			current_velocity.x = std::max(current_velocity.x, -max_velocity.x);

		if (current_velocity.y > 0)
			current_velocity.y = std::min(current_velocity.y, max_velocity.y);
		else if (current_velocity.y < 0)
			current_velocity.y = std::max(current_velocity.y, -max_velocity.y);

		//std::cout << "X: " << current_velocity.x << "Y: " << current_velocity.y << std::endl;

		sf::Vector2f temp = this->game_object->transform->getPosition();
		temp += current_velocity * delta_time;

		this->game_object->transform->setPosition(temp);
	}
	//std::cout << this->game_object->transform->getPosition().x << std::endl;
	set_aabb();

	total_forces = sf::Vector2f(0.0f, 0.0f);
}