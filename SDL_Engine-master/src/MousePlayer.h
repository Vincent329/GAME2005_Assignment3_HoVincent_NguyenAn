#pragma once
#ifndef __MOUSE_PLAYER__
#define __MOUSE_PLAYER__

#include "PlayerAnimationState.h"
#include "Sprite.h"
#include "SceneState.h"

class MousePlayer final : public Sprite
{
public:
	MousePlayer();
	~MousePlayer();

	// Life Cycle Methods
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	// MOVEMENT LOGIC
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void stopMovingX();
	void stopMovingY();

	// Mouse logic
	int xMouse, yMouse;
	void mouseMovement(int x, int y);

	// physics getters and setters
	float getMass();
	void setMass(float m_mass);
	float getVelocityX();
	void setVelocityX(float _velocityX);
	float getVelocityY();
	void setVelocityY(float _velocityY);
	float getInitialVelocityX();
	void setInitialVelocityX(float _ivelocityX);
	float getInitialVelocityY();
	void setInitialVelocityY(float _ivelocityY);
	void flipPaddle();

	SceneState getCurrentState;

	bool isColliding(GameObject*);
	float getDistance(GameObject*);

	// Physics components

	// setters
	void setAnimationState(PlayerAnimationState new_state);

private:
	void m_buildAnimations();
	PlayerAnimationState m_currentAnimationState;

	const float ACCELERATION = 10.0F;
	glm::vec2 m_direction;

	// asset properties
	bool isFlipped;
	float flipAngle;

	//Physics properties
	float mass;
	int lastUpdateTime;
	glm::vec2 initialPosition;

	// for momentum calculations
	float velocityX, velocityY, initialVelocityX, initialVelocityY;


	// Physics Components for momentum
};

#endif /* defined (__PLAYER__) */