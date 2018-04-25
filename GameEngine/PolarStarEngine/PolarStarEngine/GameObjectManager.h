#pragma once

#include "GameObject.h"
#include "AudioComponent.h"
#include "SplashScreen.h"

class GameObjectManager {
public:
	GameObjectManager();

	static GameObjectManager* instance;

	void add_to_graph(GameObject* game_object);
	void start();
	void update(float delta_time);
	void render();

	static AudioComponent audioManager;
	static Splash splashScreen;

private:
	std::vector<GameObject*> scene_graph;
};