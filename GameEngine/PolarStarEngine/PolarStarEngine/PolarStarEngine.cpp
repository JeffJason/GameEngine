// #ifndef

#include <string>
#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>

#include <Windows.h>
#include <WinBase.h>
#include <direct.h>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "PolarStarEngine.h"

#include "GameObject.h"

//splash screen
#include "SplashScreen.h"

//game play gameobjects
#include "Bill.h"
#include "Bullet.h"
#include "Rigidbody.h"
#include "SpriteRenderer.h"

#include <vector>

GameObjectManager PolarStarEngine::objectManager = GameObjectManager();
sf::RenderWindow* PolarStarEngine::render_window;

PolarStarEngine::PolarStarEngine() : is_debug_mode(true), scene_root(), physics_engine() {
	std::cout << "starting app \n";
	engine_current_state = Uninitialized;
	std::thread hardware_check_thread(&PolarStarEngine::check_hardware, this);

	this->render_window = new sf::RenderWindow();
	this->init_graphics();
	this->display_splash_screen();

	hardware_check_thread.join();

	//this->main_loop();
}

void PolarStarEngine::check_hardware() {
	std::cout << "Starting init" << std::endl;

	this->check_enough_disk_space();
	this->get_cpu_speed();
	this->get_cpu_architecture();
	this->check_memory();
	this->check_joypads();

	engine_current_state = Initialized;

	std::cout << "hardware checking . . . \n" << std::endl;

	return;
}


unsigned int PolarStarEngine::check_enough_disk_space() {
	const DWORDLONG disk_bytes_needed = 300000000;

	int const drive = _getdrive();
	//int const drive = 0;
	struct _diskfree_t diskfree;
	_getdiskfree(drive, &diskfree);
	unsigned __int64 const needed_clusters = disk_bytes_needed /
		(diskfree.sectors_per_cluster*diskfree.bytes_per_sector);
	if (diskfree.avail_clusters < needed_clusters) {
		std::cout << "ERROR NOT ENOUGH SPACE" << std::endl;
		assert(false);
	}
	return true;
}

unsigned int PolarStarEngine::get_cpu_speed() {

	DWORD buffer_size = sizeof(DWORD);
	DWORD mhz = 0;
	DWORD value_type = REG_DWORD;
	HKEY hive_key;

	long error_type = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hive_key);
	if (error_type == ERROR_SUCCESS) {
		RegQueryValueEx(hive_key,
			"~MHz", NULL, &value_type, (LPBYTE)&mhz, &buffer_size);
	}
	else {
		std::cout << "Could get CPU speed";
		return 0;
	}

	assert(mhz > 1000);

	return mhz;
}

std::string PolarStarEngine::get_cpu_architecture() {

	TCHAR architecture[1024];
	DWORD architecture_length = sizeof(architecture);
	DWORD value_type = REG_SZ;
	HKEY hive_key;
	long error_type = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		TEXT("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"), 0, KEY_READ, &hive_key);
	if (error_type == ERROR_SUCCESS) {
		RegQueryValueEx(hive_key, TEXT("ProcessorNameString"), NULL, &value_type,
			reinterpret_cast<LPBYTE>(&architecture), &architecture_length);
	}
	std::cout << architecture << std::endl;

	return "CPU WORKS";
}

void PolarStarEngine::check_memory() {

	MEMORYSTATUSEX memoryStatus = { sizeof memoryStatus };
	GlobalMemoryStatusEx(&memoryStatus);

	const DWORDLONG phyiscal_memory = memoryStatus.ullAvailPhys;

	DWORDLONG virtual_memory = memoryStatus.ullAvailVirtual;

	assert(phyiscal_memory > 90555520);
	assert(virtual_memory > 90555520);

	std::cout << "You have " << phyiscal_memory << " available bytes for physical memory." << std::endl;

	std::cout << "You have " << virtual_memory << " available bytes for virtual memory." << std::endl;

}

void PolarStarEngine::check_joypads() {
	int i = 0;
	while (sf::Joystick::isConnected(i)) {
		std::cout << "Joystick " << i << " is connected" << std::endl;
		i++;
	}
}

void PolarStarEngine::init_graphics() {
	this->render_window->create({ 1024, 709 }, "PolarStarEngine");
}

void PolarStarEngine::display_splash_screen() {

	objectManager.splashScreen.setSprite("polarstar.jpg");
	objectManager.splashScreen.isDrawn = true;
	objectManager.splashScreen.drawSprite(render_window);
	objectManager.audioManager.playMusic("gamemusic.wav", 30);


	render_window->display();

	objectManager.splashScreen.showSplash = true;

	while (objectManager.splashScreen.showSplash == true)
	{
		sf::Event event;
		while (render_window->pollEvent(event))
		{
			if (event.type == sf::Event::EventType::KeyPressed || event.type == sf::Event::EventType::MouseButtonPressed || event.type == sf::Event::EventType::Closed)
			{
				std::cout << "Exit Splash Screen" << std::endl;
				objectManager.splashScreen.isDrawn = false;
				return;
			}
		}
	}
}

void PolarStarEngine::setup_scene() {


	/*sf::Texture texture;

	if (!texture.loadFromFile("..\\Assets\\asteroid.png"))
	{
		std::cout << "Image not found" << std::endl;
	}

	sf::Sprite background(texture);

	render_window->draw(background);
	render_window->display();*/

	Bullet* bullet = new Bullet();

	GameObject* asteroidOne = new GameObject("Asteroid1", nullptr);
	SpriteRenderer* sprite_renderer = new SpriteRenderer();
	sprite_renderer->set_sprite("..\\Assets\\asteroid.png");
	asteroidOne->add_component(sprite_renderer);

	BoxCollider *box_collider = new BoxCollider(sprite_renderer->get_sprite().getGlobalBounds().width * 0.5f,
		sprite_renderer->get_sprite().getGlobalBounds().height * 0.5f);
	asteroidOne->add_component(box_collider);

	Rigidbody* rigidbody = new Rigidbody();
	rigidbody->mass = 8.0f;
	rigidbody->bounciness = 0.6f;
	asteroidOne->add_component(rigidbody);
	asteroidOne->transform->setPosition(200.0f, 100.0f);

	GameObject* asteroidTwo = new GameObject("Asteroid2", nullptr);
	SpriteRenderer* sprite_renderer2 = new SpriteRenderer();
	sprite_renderer2->set_sprite("..\\Assets\\asteroid.png");
	asteroidTwo->add_component(sprite_renderer2);

	BoxCollider *box_collider2 = new BoxCollider(sprite_renderer2->get_sprite().getGlobalBounds().width * 0.5f,
		sprite_renderer2->get_sprite().getGlobalBounds().height * 0.5f);
	asteroidTwo->add_component(box_collider2);

	Rigidbody* rigidbody2 = new Rigidbody();
	rigidbody2->mass = 8.0f;
	rigidbody2->bounciness = 0.6f;
	//rigidbody2->gravity.x = 0.0001f;
	//rigidbody2->gravity.y = 0.0f;
	asteroidTwo->add_component(rigidbody2);
	asteroidTwo->transform->setPosition(600.0f, 100.0f);


	GameObject* asteroidThree = new GameObject("Asteroid3", nullptr);
	SpriteRenderer* sprite_renderer3 = new SpriteRenderer();
	sprite_renderer3->set_sprite("..\\Assets\\asteroid.png");
	asteroidThree->add_component(sprite_renderer3);

	BoxCollider *box_collider3 = new BoxCollider(sprite_renderer3->get_sprite().getGlobalBounds().width * 0.5f,
		sprite_renderer3->get_sprite().getGlobalBounds().height * 0.5f);
	asteroidThree->add_component(box_collider3);

	Rigidbody* rigidbody3 = new Rigidbody();
	rigidbody3->mass = 8.0f;
	rigidbody3->bounciness = 0.6f;
	asteroidThree->add_component(rigidbody3);
	asteroidThree->transform->setPosition(300.0f, 80.0f);


	GameObject* asteroidFour = new GameObject("Asteroid4", nullptr);
	SpriteRenderer* sprite_renderer4 = new SpriteRenderer();
	sprite_renderer4->set_sprite("..\\Assets\\asteroid.png");
	asteroidFour->add_component(sprite_renderer4);

	BoxCollider *box_collider4 = new BoxCollider(sprite_renderer4->get_sprite().getGlobalBounds().width * 0.5f,
		sprite_renderer4->get_sprite().getGlobalBounds().height * 0.5f);
	asteroidFour->add_component(box_collider4);

	Rigidbody* rigidbody4 = new Rigidbody();
	rigidbody4->mass = 8.0f;
	rigidbody4->bounciness = 0.6f;
	asteroidFour->add_component(rigidbody4);
	asteroidFour->transform->setPosition(600.0f, 80.0f);


	GameObject* asteroidFive = new GameObject("Asteroid5", nullptr);
	SpriteRenderer* sprite_renderer5 = new SpriteRenderer();
	sprite_renderer5->set_sprite("..\\Assets\\asteroid.png");
	asteroidFive->add_component(sprite_renderer5);

	BoxCollider *box_collider5 = new BoxCollider(sprite_renderer5->get_sprite().getGlobalBounds().width * 0.5f,
		sprite_renderer5->get_sprite().getGlobalBounds().height * 0.5f);
	asteroidFive->add_component(box_collider5);

	Rigidbody* rigidbody5 = new Rigidbody();
	rigidbody5->mass = 8.0f;
	rigidbody5->bounciness = 0.6f;
	asteroidFive->add_component(rigidbody5);
	asteroidFive->transform->setPosition(450.0f, 100.0f);


	GameObject* asteroidSix = new GameObject("Asteroid6", nullptr);
	SpriteRenderer* sprite_renderer6 = new SpriteRenderer();
	sprite_renderer6->set_sprite("..\\Assets\\asteroid.png");
	asteroidSix->add_component(sprite_renderer6);

	BoxCollider *box_collider6 = new BoxCollider(sprite_renderer6->get_sprite().getGlobalBounds().width * 0.5f,
		sprite_renderer6->get_sprite().getGlobalBounds().height * 0.5f);
	asteroidSix->add_component(box_collider6);

	Rigidbody* rigidbody6 = new Rigidbody();
	rigidbody6->mass = 8.0f;
	rigidbody6->bounciness = 0.6f;
	asteroidSix->add_component(rigidbody6);
	asteroidSix->transform->setPosition(800.0f, 100.0f);

	GameObject* asteroidSeven = new GameObject("Asteroid7", nullptr);
	SpriteRenderer* sprite_renderer7 = new SpriteRenderer();
	sprite_renderer7->set_sprite("..\\Assets\\asteroid.png");
	asteroidSeven->add_component(sprite_renderer7);

	BoxCollider *box_collider7 = new BoxCollider(sprite_renderer7->get_sprite().getGlobalBounds().width * 0.5f,
		sprite_renderer7->get_sprite().getGlobalBounds().height * 0.5f);
	asteroidSeven->add_component(box_collider7);

	Rigidbody* rigidbody7 = new Rigidbody();
	rigidbody7->mass = 8.0f;
	rigidbody7->bounciness = 0.6f;
	asteroidSeven->add_component(rigidbody7);
	asteroidSeven->transform->setPosition(800.0f, 250.0f);

	GameObject* asteroidEight = new GameObject("Asteroid8", nullptr);
	SpriteRenderer* sprite_renderer8 = new SpriteRenderer();
	sprite_renderer8->set_sprite("..\\Assets\\asteroid.png");
	asteroidEight->add_component(sprite_renderer8);

	BoxCollider *box_collider8 = new BoxCollider(sprite_renderer8->get_sprite().getGlobalBounds().width * 0.5f,
		sprite_renderer8->get_sprite().getGlobalBounds().height * 0.5f);
	asteroidEight->add_component(box_collider8);

	Rigidbody* rigidbody8 = new Rigidbody();
	rigidbody8->mass = 8.0f;
	rigidbody8->bounciness = 0.6f;
	asteroidEight->add_component(rigidbody8);
	asteroidEight->transform->setPosition(600.0f, 250.0f);

	GameObject* asteroidNine = new GameObject("Asteroid9", nullptr);
	SpriteRenderer* sprite_renderer9 = new SpriteRenderer();
	sprite_renderer9->set_sprite("..\\Assets\\asteroid.png");
	asteroidNine->add_component(sprite_renderer9);

	BoxCollider *box_collider9 = new BoxCollider(sprite_renderer9->get_sprite().getGlobalBounds().width * 0.5f,
		sprite_renderer9->get_sprite().getGlobalBounds().height * 0.5f);
	asteroidNine->add_component(box_collider9);

	Rigidbody* rigidbody9 = new Rigidbody();
	rigidbody9->mass = 8.0f;
	rigidbody9->bounciness = 0.6f;
	asteroidNine->add_component(rigidbody9);
	asteroidNine->transform->setPosition(450.0f, 250.0f);

	GameObject* asteroidTen = new GameObject("Asteroid10", nullptr);
	SpriteRenderer* sprite_renderer10 = new SpriteRenderer();
	sprite_renderer10->set_sprite("..\\Assets\\asteroid.png");
	asteroidTen->add_component(sprite_renderer10);

	BoxCollider *box_collider10 = new BoxCollider(sprite_renderer10->get_sprite().getGlobalBounds().width * 0.5f,
		sprite_renderer10->get_sprite().getGlobalBounds().height * 0.5f);
	asteroidTen->add_component(box_collider10);

	Rigidbody* rigidbody10 = new Rigidbody();
	rigidbody10->mass = 8.0f;
	rigidbody10->bounciness = 0.6f;
	asteroidTen->add_component(rigidbody6);
	asteroidTen->transform->setPosition(400.0f, 250.0f);

	GameObject* asteroidEleven = new GameObject("Asteroid11", nullptr);
	SpriteRenderer* sprite_renderer11 = new SpriteRenderer();
	sprite_renderer11->set_sprite("..\\Assets\\asteroid.png");
	asteroidEleven->add_component(sprite_renderer11);

	BoxCollider *box_collider11 = new BoxCollider(sprite_renderer11->get_sprite().getGlobalBounds().width * 0.5f,
		sprite_renderer11->get_sprite().getGlobalBounds().height * 0.5f);
	asteroidEleven->add_component(box_collider11);

	Rigidbody* rigidbody11 = new Rigidbody();
	rigidbody11->mass = 8.0f;
	rigidbody11->bounciness = 0.6f;
	asteroidEleven->add_component(rigidbody11);
	asteroidEleven->transform->setPosition(225.0f, 250.0f);

	GameObject* asteroidTwelve = new GameObject("Asteroid12", nullptr);
	SpriteRenderer* sprite_renderer12 = new SpriteRenderer();
	sprite_renderer12->set_sprite("..\\Assets\\asteroid.png");
	asteroidTwelve->add_component(sprite_renderer12);

	BoxCollider *box_collider12 = new BoxCollider(sprite_renderer12->get_sprite().getGlobalBounds().width * 0.5f,
		sprite_renderer12->get_sprite().getGlobalBounds().height * 0.5f);
	asteroidTwelve->add_component(box_collider12);

	Rigidbody* rigidbody12 = new Rigidbody();
	rigidbody12->mass = 8.0f;
	rigidbody12->bounciness = 0.6f;
	asteroidTwelve->add_component(rigidbody12);
	asteroidTwelve->transform->setPosition(150.0f, 250.0f);

}

void PolarStarEngine::main_loop() {

	this->setup_scene();
	//this->scene_root.start();

	Bill* bill = new Bill();

	sf::Clock delta_time_clock;

	//PROJECTILE
	sf::CircleShape projectile;
	projectile.setFillColor(sf::Color::Red);
	projectile.setRadius(10.f);
	
	std::vector<sf::CircleShape> projectiles;
	projectiles.push_back(sf::CircleShape(projectile));

	//PLAYER
	sf::CircleShape player;
	player.setFillColor(sf::Color::White);
	player.setRadius(1.f);
	player.setPosition(1010 / 2 - 50.f, 709 - 50.f * 2 - 10.f);
	sf::Vector2f playerCenter;
	
	sf::Texture enemy;
	enemy.loadFromFile("..\\Assets\\asteroid.png");

	sf::Sprite sprite;
	sprite.setTexture(enemy);
	sprite.setPosition((rand() % int(render_window->getSize().x - 50.f)), 0.f);
	sprite.setTextureRect(sf::IntRect(1000, 1000, 120, 120));
	sprite.setColor(sf::Color(255, 255, 255, 200));

	std::vector<sf::Sprite> spritey;
	spritey.push_back(sf::Sprite(sprite));

	int shootTimer = 0;
	int enemySpawnTimer = 0;
	
	while (this->render_window->isOpen())
	{

		sf::Event event;
		

		while (this->render_window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				this->render_window->close();
		}

		playerCenter = sf::Vector2f(player.getPosition().x + player.getRadius(), player.getPosition().y + player.getRadius());

		//player.setPosition(sf::Mouse::getPosition().x, player.getPosition().y);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			player.setPosition(player.getPosition().x - 1, player.getPosition().y);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			player.setPosition(player.getPosition().x + 1, player.getPosition().y);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			player.setPosition(player.getPosition().x, player.getPosition().y - 1);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			player.setPosition(player.getPosition().x, player.getPosition().y + 1);
		}

		if (shootTimer < 5)
			shootTimer++;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && shootTimer >= 5)
		{
			projectile.setPosition(playerCenter);
			projectiles.push_back(sf::CircleShape(projectile));
			shootTimer = 0;
			Bullet* bullet = new Bullet();
			bullet->transform->setPosition(playerCenter);
			
		}

		for (size_t i = 0; i < projectiles.size(); i++)
		{
			projectiles[i].move(0.f, -1.f);

			if (projectiles[i].getPosition().y <= 0)
				projectiles.erase(projectiles.begin() + i);
		}

		//ENEMIES
		if (enemySpawnTimer < 5)
			enemySpawnTimer++;

		if (enemySpawnTimer >= 5)
		{
			sprite.setPosition((rand() % int(render_window->getSize().x - 50.f)), 0.f);
			spritey.push_back(sf::Sprite(sprite));

			enemySpawnTimer = 0;
		}

		for (size_t i = 0; i < spritey.size(); i++)
		{
			spritey[i].move(0.f, 1.f);

			if (spritey[i].getPosition().y > render_window->getSize().y)
				spritey.erase(spritey.begin() + i);
		}

		//COLLISION

		if (!spritey.empty() && !projectiles.empty())
		{
			for (size_t i = 0; i < projectiles.size(); i++)
			{
				for (size_t k = 0; k < spritey.size(); k++)
				{
					if (projectiles[i].getGlobalBounds().intersects(spritey[k].getGlobalBounds()))
					{
						projectiles.erase(projectiles.begin() + i);
						spritey.erase(spritey.begin() + k);
						//break;
					}
				}
			}
		}

		float delta_time = (float)delta_time_clock.restart().asMilliseconds();
		this->render_window->clear();
		this->render_window->draw(player);
		this->render_window->draw(sprite);

		for (size_t i = 0; i < spritey.size(); i++)
		{
			this->render_window->draw(spritey[i]);
		}

		for (size_t i = 0; i < projectiles.size(); i++)
		{
			this->render_window->draw(projectiles[i]);
		}

		this->physics_engine.updates_physics(delta_time);
		this->scene_root.update(delta_time);
		this->render_window->display();
	}
}