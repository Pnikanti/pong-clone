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

static struct Colors
{
	static constexpr glm::vec3 Yellow = glm::vec3(0.9f, 0.9f, 0.2f);
	static constexpr glm::vec3 Purple = glm::vec3(0.9f, 0.2f, 0.9f);
};

enum class GameState
{
	Play = 0, MainMenu, Start, Pause, GameOver
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
	void Exit();
	void Loop();

	void CheckGameStart();
	void CheckGameOver();
	void ResetScore();

	void ProcessInput();
	void SolvePhysics();
	void UpdateAllEntities();
	void CreateGameGui();

	bool IsBallOutOfBounds();

	Entity* CreateWall(Side side);
	Entity* CreateEntity(InputComponent* input, PhysicsComponent* physics, OpenGL::GraphicsComponent* graphics, glm::vec2 size, glm::vec2 position, float rotation, glm::vec3 color, b2BodyType bodytype, float density, float friction, float restitution);
	Entity* CreatePaddle(Side side, Player player);
	Entity* CreateBall();

public:
	static float TimeStep;
	static GameState State;
	static unsigned int ScoreLeftPlayer;
	static unsigned int ScoreRightPlayer;
	static int StartSequence[3];
	static int SequenceIndex;

	bool BallOutOfBounds;
private:
	OpenGL::Context* Context;
	PhysicsWorld* Physics;
	ContactListener* PhysicsListener;
	GameInputComponent* Input;
	float CurrentTime = 0.0f;
	float PreviousTime = 0.0f;
};