#pragma once

class Entity;
class Game;

class InputComponent {
public:
	virtual void Update(Entity& entity);
	virtual void Update(Game& game);
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
	float PreviousMovement = 0.0f;
	float CurrentMovement = 0.0f;
};
class GameInputComponent : public InputComponent {
public:
	void Update(Game& game);
};