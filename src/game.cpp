#include <glfw/glfw3.h>
#include <box2d/b2_body.h>
#include <string.h>
#include "context.h"
#include "gui.h"
#include "game.h"
#include "log.h"
#include "camera.h"
#include "graphics.h"
#include "physics.h"
#include "entity.h"
#include "entitymanager.h"
#include "input.h"


std::vector<OpenGL::GuiContext*> Game::GuiContexts(std::vector<OpenGL::GuiContext*>(2));
float Game::TimeStep = 1.0f / 60.0f;

Game::Game(const char* appName, int width, int height) :
	Context(nullptr), State(GameState::MainMenu)
{
	Logger::Init(appName);
	EntityManager::Init(3);

	CreateDebugGui();
	CreateGameGui();

	Physics = new PhysicsWorld();
	PhysicsListener = new ContactListener();
	Input = new GameInputComponent();

	Context = new OpenGL::Context(width, height, appName);
	Context->AddShader(std::string("BasicShader"), std::string("res/shaders/vertex.shader"), std::string("res/shaders/fragment.shader"));
	Context->Start();
	Loop();
}

Game::~Game()
{
	for (auto i : GuiContexts)
	{
		if (i != nullptr)
			delete i;
	}
	GuiContexts.clear();

	for (auto i : EntityManager::GetEntities())
	{
		if (i != nullptr)
			delete i;
	}
	EntityManager::GetEntities().clear();
	EntityManager::GetEntityMap().clear();
}

void Game::Start()
{
	CreateWalls();
	EntityManager::Get().CreatePaddle(Side::Left, Input::Human);
	EntityManager::Get().CreatePaddle(Side::Right, Input::Human);
	Entity* ball = EntityManager::Get().CreateBall();
	int directionX = 0;
	if (rand() > (RAND_MAX / 2))
		directionX = -1;
	else
		directionX = 1;
	ball->GetPhysicsComponent()->Body->ApplyLinearImpulse(b2Vec2(4.0f * directionX, 0.0f), ball->GetPhysicsComponent()->Body->GetWorldCenter(), true);
}

void Game::End()
{
	for (auto i : EntityManager::GetEntities())
	{
		if (i != nullptr)
			delete i;
	}
	EntityManager::GetEntities().clear();
	EntityManager::GetEntityMap().clear();
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

	for (auto i : EntityManager::GetEntities())
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
	auto it = EntityManager::GetEntityMap().find("Ball");

	if (it == EntityManager::GetEntityMap().end())
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
void Game::CreateWalls()
{
	EntityManager::Get().CreateEntity(
		new InputComponent(),
		new PhysicsPolygonComponent(),
		new OpenGL::QuadComponent(),
		glm::vec2(30.0f, 0.5f),
		glm::vec2(0.0f, 20.5f),
		0.0f,
		glm::vec3(0.0f),
		b2_staticBody,
		0.0f,
		0.0f,
		1.0f
	);
	EntityManager::Get().CreateEntity(
		new InputComponent(),
		new PhysicsPolygonComponent(),
		new OpenGL::QuadComponent(),
		glm::vec2(30.0f, 0.5f),
		glm::vec2(0.0f, -20.5f),
		0.0f,
		glm::vec3(0.0f),
		b2_staticBody,
		0.0f,
		0.0f,
		1.0f
	);
}

void Game::UpdateAllEntities()
{
	for (auto i = EntityManager::GetEntities().begin(); i != EntityManager::GetEntities().end(); i++)
	{
		Entity* e = (*i);

		if (e == nullptr)
			continue;

		e->Advance();
		SolvePhysics();
	}
}

void Game::CreateDebugGui()
{
	OpenGL::GuiContext* t = new OpenGL::DebugGuiContext();
	GuiContexts.push_back(t);
}

void Game::CreateGameGui()
{
	OpenGL::GuiContext* t = new OpenGL::GameGuiContext();
	GuiContexts.push_back(t);
}

int main()
{
	auto x = Game("Pong-clone", 1200, 800);
	return 0;
}