#pragma once
#ifndef __PLAYER__
#define __PLAYER__

#include "PlayerAnimationState.h"
#include "Sprite.h"

class Player final : public Sprite
{
public:
	Player();
	~Player();

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

	bool isColliding(GameObject*);
	float getDistance(GameObject*);


	// setters
	void setAnimationState(PlayerAnimationState new_state);

private:
	void m_buildAnimations();

	PlayerAnimationState m_currentAnimationState;

	const float ACCELERATION = 10.0F;
	glm::vec2 m_direction;
};

#endif /* defined (__PLAYER__) */