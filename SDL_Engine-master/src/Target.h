#pragma once
#ifndef __TARGET__
#define __TARGET__

#include "DisplayObject.h"
#include "Util.h"

class Target final : public DisplayObject {
public:
	Target();
	~Target();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	//void bounceWall;

private:
	void m_move();
	void m_checkBounds();
	void m_reset();

	// Physics components
	float mass;
	float velocityMagnitude;
	glm::vec2 velocity;
};


#endif /* defined (__TARGET__) */