#include "InputHandler.hpp"
#include <iostream>

InputHandler::InputHandler() : key_bindings() {}

void InputHandler::start() {
	//
}

void InputHandler::update(float delta_time) {
	std::cout << "Helping input..." << std::endl;
	for (key_binding::iterator itorator = this->key_bindings.begin(); itorator != this->key_bindings.end(); itorator++) {
		if (sf::Keyboard::isKeyPressed(itorator->first)) {
			itorator->second();
		}
	}
}

void InputHandler::add_binding(sf::Keyboard::Key key, key_function function_address) {

	this->key_bindings.insert(std::pair<sf::Keyboard::Key, key_function*>(key, function_address));

}