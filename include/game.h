#pragma once
#include <vector>
class OpenGLContext;

class Game {
public:
	Game::Game();
	Game::~Game();
	void Start();
public:
	static std::vector<GameObject*> gameObjects;
private:
	void GameLoop();
	void Advance();
	void ProcessInput();
	GameObject* CreateTestGameObject();
private:
	OpenGLContext* context;
	GameObject* test;
	float MS_PER_UPDATE;
};
