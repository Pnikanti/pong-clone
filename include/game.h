#pragma once
#include <vector>
class OpenGLContext;
class GameObject;
class OrthographicCamera;

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
	GameObject* CreatePlayer();
	OrthographicCamera* CreateCamera(float width, float height);
private:
	float MS_PER_UPDATE;
	glm::mat4 viewProjectionMatrix;
	OpenGLContext* context;
	OrthographicCamera* camera;
};
