#pragma once
#include <vector>
#include <box2d/b2_body.h>

namespace OpenGL { class Context; class GuiContext; class GraphicsComponent; }
class Entity;
class OrthographicCamera;
class PhysicsWorld;
class PhysicsComponent;
class GameInputComponent;
class ContactListener;
class InputComponent;

enum class GameState
{
	Play = 0, MainMenu, Pause, GameOver
};

enum class Side
{
	Left = 0, Bottom = 0,
	Right = 1, Top = 1
};

enum class Player
{
	Human = 0, Ai
};

class Game {
public:
	Game(const char* appName, int width, int height);
	~Game();

	void Start();
	void End();
	void Loop();
	void ProcessInput();
	void SolvePhysics();
	void UpdateAllEntities();

	void IsGameOver();
	bool IsBallOutOfBounds();

	OpenGL::GuiContext* CreateDebugGui();
	OpenGL::GuiContext* CreateGameGui();
	Entity* CreateWall(Side side);
	Entity* CreateEntity(InputComponent* input, PhysicsComponent* physics, OpenGL::GraphicsComponent* graphics, glm::vec2 size, glm::vec2 position, float rotation, glm::vec3 color, b2BodyType bodytype, float density, float friction, float restitution);
	Entity* CreatePaddle(Side side, Player player);
	Entity* CreateBall();

public:
	static float TimeStep;
	static GameState State;
	bool GameOver;
private:
	OpenGL::Context* Context;
	PhysicsWorld* Physics;
	ContactListener* PhysicsListener;
	GameInputComponent* Input;
};