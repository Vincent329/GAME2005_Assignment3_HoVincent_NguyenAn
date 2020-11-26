#pragma once
#ifndef __TARGET__
#define __TARGET__

#include "DisplayObject.h"
#include "CollisionShape.h"
#include "Util.h"

class Target final : public DisplayObject {
public:
	Target();
	~Target();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	float getMass();
	void setMass(float _nass);
	float getInitialVelocityX();
	float getInitialVelocityY();
	float getWallCoefficient();
	void setWallCoefficient(float _wall);
	CollisionShape getCollisionType();
	void setCollisionType(CollisionShape shape);
	bool getShowHitbox();
	void setShowHitbox(bool box);

	CollisionShape collisionType;

private:
	void m_move();
	void m_checkBounds();
	void m_reset();

	// Physics components
	float mass;
	float initialVelocityX;
	float initialVelocityY;
	float wallCoefficient;
	bool showHitbox;
};


#endif /* defined (__TARGET__) */