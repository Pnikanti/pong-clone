#pragma once
#include <glfw3.h>

class GameObject;
class Game;

class InputComponent {
public:
	virtual void Update(GameObject& object);
	virtual void Update(Game& game);
	virtual void Update();
};

class PlayerInputComponent : public InputComponent {
public:
	void Update(GameObject& object) override;
};

class MasterInputComponent : public InputComponent {
public:
	void Update(Game& game) override;
};