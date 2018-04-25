#pragma once
#include <map>
#include <SFML/Window.hpp>

#include "Component.h"

typedef void key_function();
typedef std::map<sf::Keyboard::Key, key_function*> key_binding;

class InputHandler : public Component {

public:
	InputHandler();

	void start() override;
	void update(float delta_time) override;

	void add_binding(sf::Keyboard::Key key, key_function function_address);

private:
	key_binding key_bindings;

};