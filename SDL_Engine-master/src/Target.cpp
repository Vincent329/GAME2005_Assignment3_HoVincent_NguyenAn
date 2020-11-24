#include "Target.h"
#include "TextureManager.h"


Target::Target()
{
	TextureManager::Instance()->load("../Assets/textures/Circle.png","circle");

	const auto size = TextureManager::Instance()->getTextureSize("circle");
	setWidth(size.x);
	setHeight(size.y);
	/*getTransform()->position = glm::vec2(100.0f, 100.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);*/
	m_reset();
	mass = 2.5f;
	getRigidBody()->isColliding = false;
	setType(TARGET);
}

Target::~Target()
= default;

void Target::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("circle", x, y, 0, 255, true);
}

void Target::update()
{
	m_move();
	m_checkBounds();
}

void Target::clean()
{
}


float Target::getMass()
{
	return mass;
}


void Target::setMass(float _mass)
{
	mass = _mass;
}

float Target::getInitialVelocityX()
{
	return initialVelocityX;
}
float Target::getInitialVelocityY()
{
	return initialVelocityY;
}

void Target::m_move()
{
	getTransform()->position = getTransform()->position + getRigidBody()->velocity;
	initialVelocityX = getRigidBody()->velocity.x;
	initialVelocityY = getRigidBody()->velocity.y;
}

void Target::m_checkBounds()
{
	// hits right wall 
	if (getTransform()->position.x >= (800.0f - getWidth()* 0.5f))
	{
		getTransform()->position.x = 800.0f - getWidth() * 0.5f;
		getRigidBody()->velocity.x *= -0.75f;
	}
	// hits left wall
	else if (getTransform()->position.x <= (0.0f + getWidth() * 0.5f))
	{
		getTransform()->position.x = 0 + getWidth() * 0.5f;
		getRigidBody()->velocity.x *= -0.75f;
	}
	// hits bottom wall
	if (getTransform()->position.y >= (600.0f - getWidth()* 0.5f)) 
	{	
		getTransform()->position.y = 600.0f - getWidth() * 0.5f;
		getRigidBody()->velocity.y *= -0.75f;
	}
	// hits top wall
	else if (getTransform()->position.y <= (0.0f + getWidth() * 0.5f))
	{
		getTransform()->position.y = 0 + getWidth() * 0.5f;
		getRigidBody()->velocity.y *= -0.75f;

	}
}

void Target::m_reset()
{
	getTransform()->position = (glm::vec2(Util::RandomRange(0.0f, 800.0f - getWidth()), Util::RandomRange(0.0f, 600.0f - getWidth())));
	getRigidBody()->velocity = (glm::vec2(Util::RandomRange(-1.0f, 1.0f), 3.0f));
}
