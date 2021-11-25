#pragma once
#include <vector>

namespace OpenGL { class GuiContext; class Context; }
class Entity;
class OrthographicCamera;
class PhysicsWorld;
class GameInputComponent;
class ContactListener;

enum class GameState
{
	Play = 0, MainMenu, Pause, GameOver
};

enum class Side
{
	Left = 0, Right
};

enum class Input
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
	void CreateWalls();
	void ProcessInput();
	void SolvePhysics();
	void UpdateAllEntities();
	void CreateDebugGui();
	void CreateGameGui();

	bool CheckOutOfBounds(Entity* e);

public:
	static std::vector<OpenGL::GuiContext*> GuiContexts;
	static float TimeStep;
	GameState State;
private:
	OpenGL::Context* Context;
	PhysicsWorld* Physics;
	ContactListener* PhysicsListener;
	GameInputComponent* Input;
};