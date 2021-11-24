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
}

void Game::Start()
{
	EntityManager::Get().CreatePaddle(Side::Left, Input::Human);
	EntityManager::Get().CreatePaddle(Side::Right, Input::Human);
	Entity* ball = EntityManager::Get().CreateBall();
	ball->GetPhysicsComponent()->Body->ApplyLinearImpulse(b2Vec2(-4.0f, 0.0f), ball->GetPhysicsComponent()->Body->GetWorldCenter(), true);
}

void Game::End()
{
	for (auto i : EntityManager::GetEntities())
	{
		if (i != nullptr)
			delete i;
	}
	EntityManager::GetEntities().clear();
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
			lag -= TimeStep;
		}
		Context->RenderOneFrame();
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
void Game::UpdateAllEntities()
{
	for (auto i = EntityManager::GetEntities().begin(); i != EntityManager::GetEntities().end(); i++)
	{
		Entity* e = (*i);

		if (e == nullptr)
			continue;

		e->Advance();

		std::vector<Contact>::iterator position;
		for (position = PhysicsListener->Contacts.begin(); position != PhysicsListener->Contacts.end(); ++position)
		{
			Contact x = *position;
			int mod = 0;

			if ((x.fixtureB == e->GetPhysicsComponent()->Fixture))
			{
				LOGGER_INFO("Intersecting!");
				if (e->Position.x > 0)
					mod = -1;
				else
					mod = 1;

				e->GetPhysicsComponent()->Body->ApplyLinearImpulse(b2Vec2(0.8f * mod, 0.2f * mod), e->GetPhysicsComponent()->Body->GetWorldCenter(), true);
			}

		}
		if (e->BodyType == b2_dynamicBody)
		{
			float xPos = e->Position.x;

			if ( xPos < -25.0f)
			{
				State = GameState::GameOver;
				Game::End();
				break;
			}

			else if (xPos > 25.0f)
			{
				State = GameState::GameOver;
				Game::End();
				break;
			}
		}
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