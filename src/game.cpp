#include <glfw/glfw3.h>
#include <box2d/b2_body.h>
#include <string.h>
#include "context.h"
#include "game.h"
#include "log.h"
#include "camera.h"
#include "graphics.h"
#include "physics.h"
#include "entity.h"
#include "entitymanager.h"
#include "gui.h"
#include "guimanager.h"
#include "input.h"

#define length(array) ((sizeof(array)) / (sizeof(array[0])))

float Game::TimeStep = 1.0f / 60.0f;
GameState Game::State = GameState::MainMenu;
unsigned int Game::ScoreLeftPlayer = 0;
unsigned int Game::ScoreRightPlayer = 0;
int Game::StartSequence[3] = { 3, 2, 1 };
int Game::SequenceIndex = 0;

Game::Game(const char* appName, int width, int height) :
	Context(nullptr), BallOutOfBounds(false)
{
	Logger::Init(appName);
	EntityManager::Get().Init(3);
	OpenGL::GuiManager::Get().Init(2);

	CreateGameGui();

	Physics = new PhysicsWorld();
	PhysicsListener = new ContactListener();
	Input = new GameInputComponent();

	Context = new OpenGL::Context(width, height, appName);
	Context->AddShader(std::string("BasicShader"), std::string("res/shaders/vertex.shader"), std::string("res/shaders/fragment.shader"));
	Context->AddShader(std::string("TexturedShader"), std::string("res/shaders/texturedvertex.shader"), std::string("res/shaders/texturedfragment.shader"));
	Context->Start();
	Loop();
}

Game::~Game()
{
	for (auto i : OpenGL::GuiManager::Get().GetContexts())
	{
		if (i != nullptr)
			delete i;
	}
	OpenGL::GuiManager::Get().GetContexts().clear();

	for (auto i : EntityManager::Get().GetEntities())
	{
		if (i != nullptr)
			delete i;
	}

	EntityManager::Get().GetEntities().clear();
	EntityManager::Get().GetEntityMap().clear();
}

void Game::Start()
{
	CreateWall(Side::Top);
	CreateWall(Side::Bottom);
	CreatePaddle(Side::Left, Player::Human);
	CreatePaddle(Side::Right, Player::Ai);
	CreateBall();
}

void Game::End()
{
	for (auto i : EntityManager::Get().GetEntities())
	{
		if (i != nullptr)
			delete i;
	}
	LOGGER_INFO("Entities size: {0}", EntityManager::Get().GetEntities().size());
	LOGGER_INFO("EntityMap size: {0}", EntityManager::Get().GetEntityMap().size());
	LOGGER_INFO("PhysicsListener size: {0}", PhysicsListener->Contacts.size());
	EntityManager::Get().GetEntities().clear();
	EntityManager::Get().GetEntityMap().clear();
	LOGGER_INFO("Entities size: {0}", EntityManager::Get().GetEntities().size());
	LOGGER_INFO("EntityMap size: {0}", EntityManager::Get().GetEntityMap().size());
	LOGGER_INFO("PhysicsListener size: {0}", PhysicsListener->Contacts.size());
}

void Game::Exit()
{
	glfwSetWindowShouldClose(OpenGL::Context::Window, true);
}

void Game::Loop()
{
	double previous, current, elapsed, lag;
	previous = current = elapsed = lag = 0.0;

	while (Context->Alive)
	{
		current = glfwGetTime();
		elapsed = current - previous;
		previous = current;
		lag += elapsed;

		ProcessInput();
		CheckGameStart();
		while (lag >= TimeStep)
		{
			if (State == GameState::Play)
			{
				Physics->Update();
				UpdateAllEntities();
				BallOutOfBounds = IsBallOutOfBounds();
			}
			lag -= TimeStep;
		}
		Context->RenderOneFrame();
		CheckGameOver();
	}
}

void Game::ResetScore()
{
	ScoreLeftPlayer = 0;
	ScoreRightPlayer = 0;
}

void Game::ProcessInput()
{
	Input->Update(*this);

	for (auto i : EntityManager::Get().GetEntities())
	{
		if (i != nullptr)
			i->ProcessInput();
	}
}

void Game::SolvePhysics()
{
	std::vector<Contact>::iterator position;
	for (position = PhysicsListener->Contacts.begin(); position != PhysicsListener->Contacts.end(); ++position)
	{
		Contact x = *position;
		glm::vec3 color = glm::vec3(0.0f);

		if (x.fixtureA->GetBody()->GetType() == b2_staticBody)
			continue;

		int directionX = 0;
		int directionY = 0;

		b2Body* bodyA = x.fixtureA->GetBody();
		b2Body* bodyB = x.fixtureB->GetBody();
		 
		b2Vec2 worldpoints = x.worldManifold.points[0];

		b2Vec2 intersectionA = bodyA->GetWorldPoint(bodyA->GetLocalPoint(bodyB->GetWorldCenter()));
		b2Vec2 intersectionB = bodyB->GetWorldPoint(bodyB->GetLocalPoint(bodyA->GetWorldCenter()));

		if (intersectionA.y > intersectionB.y)
			directionY = 1;
		else
			directionY = -1;

		if (worldpoints.x > 0)
		{
			directionX = -1;
			color = Colors::Purple;
		}
		else
		{
			directionX = 1;
			color = Colors::Yellow;
		}

		float r = ((float)rand() / (float)RAND_MAX) / 2;

		// Apply linear pulse to ball
		bodyB->ApplyLinearImpulse(b2Vec2(0.3f * directionX, r * directionY), bodyB->GetWorldCenter(), true);

		// Set ball color
		auto it = EntityManager::Get().GetEntityMap().find("Ball");

		if (it == EntityManager::Get().GetEntityMap().end())
			return;

		Entity* e = it->second;
		e->Color = color;
	}
}

void Game::CheckGameStart()
{
	if (State == GameState::Start)
	{
		CurrentTime = (float)glfwGetTime();
		if (CurrentTime > PreviousTime + 1.0f)
		{
			PreviousTime = CurrentTime;
			SequenceIndex++;
		}

		if (SequenceIndex < int(length(StartSequence)))
			return;

		SequenceIndex = 0;

		auto it = EntityManager::Get().GetEntityMap().find("Ball");
		if (it == EntityManager::Get().GetEntityMap().end())
			return;

		int directionX = 0;
		Entity* e = it->second;

		if (rand() > (RAND_MAX / 2))
			directionX = -1;
		else
			directionX = 1;

		e->GetPhysicsComponent()->Body->ApplyLinearImpulse(b2Vec2(4.0f * directionX, 0.0f), e->GetPhysicsComponent()->Body->GetWorldCenter(), true);
		State = GameState::Play;
	}
}

bool Game::IsBallOutOfBounds()
{
	auto it = EntityManager::Get().GetEntityMap().find("Ball");

	if (it == EntityManager::Get().GetEntityMap().end())
		return false;

	Entity* e = it->second;
	float positionX = e->Position.x;

	if (positionX < -30.0f)
		return true;

	else if (positionX > 30.0f)
		return true;
	return false;
}

void Game::CheckGameOver()
{
	if (BallOutOfBounds && State != GameState::GameOver)
	{
		auto it = EntityManager::Get().GetEntityMap().find("Ball");

		if (it == EntityManager::Get().GetEntityMap().end())
			return;

		Entity* ball = it->second;

		if (ball->GetPosition().x > 0)
			ScoreLeftPlayer++;
		else
			ScoreRightPlayer++;

		State = GameState::GameOver;
		BallOutOfBounds = false;
	}
}

Entity* Game::CreateWall(Side side)
{
	glm::vec2 position = glm::vec2(0.0f);

	switch (side)
	{
	case Side::Top:
		position = glm::vec2(0.0f, 20.5f);
		break;
	case Side::Bottom:
		position = glm::vec2(0.0f, -20.5f);
		break;
	}

	Entity* e = CreateEntity(
		new InputComponent(),
		new PhysicsPolygonComponent(),
		new OpenGL::QuadComponent(),
		glm::vec2(30.0f, 0.5f),
		position,
		0.0f,
		glm::vec3(0.0f),
		b2_staticBody,
		0.0f,
		0.0f,
		1.0f
	);
	return e;
}

Entity* Game::CreateEntity(InputComponent* input, PhysicsComponent* physics, OpenGL::GraphicsComponent* graphics, glm::vec2 size, glm::vec2 position, float rotation, glm::vec3 color, b2BodyType bodytype, float density, float friction, float restitution)
{
	Entity* x = new Entity(input, physics, graphics, position, size, rotation, color, bodytype, density, friction, restitution);
	EntityManager::Get().GetEntities().emplace_back(x);
	return x;
}

Entity* Game::CreatePaddle(Side side, Player player)
{
	glm::vec2 position = glm::vec2(0.0f);
	glm::vec3 color = glm::vec3(0.0f);
	InputComponent* inputComponent = nullptr;
	switch (side)
	{
		case Side::Left:
			position = glm::vec2(-25.0f, 0.0f);
			color = Colors::Yellow;
			break;
		case Side::Right:
			position = glm::vec2(25.0f, 0.0f);
			color = Colors::Purple;
			break;
	}
	switch (player)
	{
		case Player::Human:
			inputComponent = new PlayerInputComponent();
			break;
		case Player::Ai:
			inputComponent = new ComputerInputComponent();
			break;
	}
	Entity* x = CreateEntity(
		inputComponent,
		new PhysicsPolygonComponent(),
		new OpenGL::QuadComponent(),
		glm::vec2(0.5f, 3.0f),
		position,
		0.0f,
		color,
		b2_kinematicBody,
		1.0f,
		1.0f,
		0.8f
	);
	return x;
}

Entity* Game::CreateBall()
{
	InputComponent* inputComponent = new InputComponent();

	Entity* x = CreateEntity(
		inputComponent,
		new PhysicsCircleComponent(),
		new OpenGL::TexturedQuadComponent("res/textures/circle-800.png"),
		glm::vec2(0.5f),
		glm::vec2(0.0f, 4.0f),
		30.0f,
		glm::vec3(255.0f, 255.0f, 255.0f),
		b2_dynamicBody,
		1.0f,
		1.0f,
		0.8f
	);
	EntityManager::Get().GetEntityMap().emplace(std::string("Ball"), x);
	return x;
}

void Game::UpdateAllEntities()
{
	for (auto i = EntityManager::Get().GetEntities().begin(); i != EntityManager::Get().GetEntities().end(); i++)
	{
		Entity* e = (*i);

		if (e == nullptr)
			continue;

		e->Advance();
		SolvePhysics();
	}
}

void Game::CreateGameGui()
{
	OpenGL::GuiManager::Get().GetContexts().push_back(new OpenGL::GameGuiContext());
#ifdef DEBUG
	OpenGL::GuiManager::Get().GetContexts().push_back(new OpenGL::DebugGuiContext());
#endif
}

int main()
{
	auto x = Game("Pong-clone", 1200, 800);
	return 0;
}