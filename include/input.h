#pragma once
#include <glfw3.h>

class GameObject;

class InputComponent {
public:
	virtual void Update(GameObject& object);
};

class PlayerInputComponent : public InputComponent {
public:
	void Update(GameObject& object) override;
private:
};