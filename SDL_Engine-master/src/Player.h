#pragma once
#ifndef __PLAYER__
#define __PLAYER__

#include "PlayerAnimationState.h"
#include "SDL.h"
#include "Sprite.h"
#include "SceneState.h"

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

	// Mouse logic
	int xMouse, yMouse;
	void mouseMovement(int x, int y);
	void checkBounds();
	float getAcceleration();
	void setAcceleration(float accel);

	// Hitbox implementation
	SDL_Rect getBoundingBoxTop();
	SDL_Rect getBoundingBoxBottom();

	void updateBoundingBoxTop();
	void updateBoundingBoxBottom();
	bool getShowHitbox();
	void setShowHitbox(bool box);

	bool isColliding(GameObject*);
	float getDistance(GameObject*);

	// Physics components
	
	// setters
	void setAnimationState(PlayerAnimationState new_state);

private:
	void m_buildAnimations();

	PlayerAnimationState m_currentAnimationState;

	float acceleration = 5.0F;
	glm::vec2 m_direction;
	bool showHitbox;

	//testing
	int lastUpdateTime;
	glm::vec2 initialPosition;
	float velocityX, velocityY;
	SDL_Rect boundingBoxTop;
	SDL_Rect boundingBoxBottom;
	// Physics Components for momentum
};

#endif /* defined (__PLAYER__) */