#pragma once
#include <vector>
class OpenGLContext;
class GameObject;
class OrthographicCamera;

enum class GameState
{
	Play = 0, MainMenu = 1, GameOver = 2
};

enum class Side
{
	Left = 0, Right = 1
};

class Game {
public:
	Game::Game();
	Game::~Game();
	void Start();
	void End();
public:
	static std::vector<GameObject*> gameObjects;
	GameState state;
private:
	void GameLoop();
	void Advance();
	void ProcessInput();
	void CreateBall();
	void CreatePaddle();
	OrthographicCamera* CreateCamera(float width, float height);
private:
	float MS_PER_UPDATE;
	glm::mat4 viewProjectionMatrix;
	OpenGLContext* context;
	OrthographicCamera* camera;
	InputComponent* input;
};

