#pragma once

class Entity;
class Game;

class InputComponent {
public:
	virtual void Update(Entity& entity);
	virtual void Update(Game& gameInstance);
	virtual void Update();
};

class PlayerInputComponent : public InputComponent {
public:
	void Update(Entity& entity) override;
};
class ComputerInputComponent : public InputComponent {
public:
	void Update(Entity& entity) override;
private:
	float PreviousTime = 0.0f;
	float CurrentTime = 0.0f;
};
class GameInputComponent : public InputComponent {
public:
	void Update(Game& gameInstance);
private:
	float PreviousTime = 0.0f;
	float CurrentTime = 0.0f;
};