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

float Game::TimeStep = 1.0f / 60.0f;

Game::Game(const char* appName, int width, int height) :
	Context(nullptr), State(GameState::MainMenu)
{
	Logger::Init(appName);
	EntityManager::Get().Init(3);
	OpenGL::GuiManager::Get().Init(2);

	CreateDebugGui();
	CreateGameGui();

	Physics = new PhysicsWorld();
	PhysicsListener = new ContactListener();
	Input = new GameInputComponent();

	Context = new OpenGL::Context(width, height, appName);
	Context->AddShader(std::string("BasicShader"), std::string("res/shaders/vertex.shader"), std::string("res/shaders/fragment.shader"));
	Context->AddShader(std::string("BallShader"), std::string("res/shaders/vertex.shader"), std::string("res/shaders/ballfragment.shader"));
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
	EntityManager::Get().GetEntities().clear();
}

void Game::Start()
{
	CreateWall(Side::Top);
	CreateWall(Side::Bottom);
	CreatePaddle(Side::Left, Player::Human);
	CreatePaddle(Side::Right, Player::Ai);
	Entity* ball = CreateBall();
	int directionX = 0;
	if (rand() > (RAND_MAX / 2))
		directionX = -1;
	else
		directionX = 1;
	ball->GetPhysicsComponent()->Body->ApplyLinearImpulse(b2Vec2(4.0f * directionX, 0.0f), ball->GetPhysicsComponent()->Body->GetWorldCenter(), true);
}

void Game::End()
{
	for (auto i : EntityManager::Get().GetEntities())
	{
		if (i != nullptr)
			delete i;
	}
	EntityManager::Get().GetEntities().clear();
	EntityManager::Get().GetEntityMap().clear();
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
		while (lag >= TimeStep)
		{
			Physics->Update();
			UpdateAllEntities();
			GameOver = IsBallOutOfBounds();
			lag -= TimeStep;
		}
		Context->RenderOneFrame();
		IsGameOver();
	}
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
			directionX = -1;
		else
			directionX = 1;

		float r = ((float)rand() / (float)RAND_MAX) / 2;
		bodyB->ApplyLinearImpulse(b2Vec2(0.3f * directionX, r * directionY), bodyB->GetWorldCenter(), true);
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

void Game::IsGameOver()
{
	if (GameOver)
	{
		State = GameState::GameOver;
		End();
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
	InputComponent* inputComponent = nullptr;

	switch (side)
	{
		case Side::Left:
			position = glm::vec2(-25.0f, 0.0f);
			break;
		case Side::Right:
			position = glm::vec2(25.0f, 0.0f);
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
		glm::vec3(255.0f, 255.0f, 0.0f),
		b2_kinematicBody,
		1.0f,
		1.0f,
		0.8f
	);
	return x;
}

Entity* Game::CreateBall()
{
	Entity* x = CreateEntity(
		new InputComponent(),
		new PhysicsCircleComponent(),
		new OpenGL::QuadComponent(),
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

OpenGL::GuiContext* Game::CreateDebugGui()
{
	OpenGL::GuiContext* t = new OpenGL::DebugGuiContext();
	OpenGL::GuiManager::Get().GetContexts().push_back(t);
	return t;
}

OpenGL::GuiContext* Game::CreateGameGui()
{
	OpenGL::GuiContext* t = new OpenGL::GameGuiContext();
	OpenGL::GuiManager::Get().GetContexts().push_back(t);
	return t;
}

int main()
{
	auto x = Game("Pong-clone", 1200, 800);
	return 0;
}