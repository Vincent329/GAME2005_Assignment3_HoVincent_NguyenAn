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

void Target::m_move()
{
	getTransform()->position = getTransform()->position + getRigidBody()->velocity * 5.0f;
	// movement logic
}

void Target::m_checkBounds()
{
	// hits right wall 
	if (getTransform()->position.x >= (800.0f - getWidth()* 0.5f))
	{
		getTransform()->position.x = 800.0f - getWidth() * 0.5f;
		getRigidBody()->velocity.x *= -0.9f;
	}
	// hits left wall
	else if (getTransform()->position.x <= (0.0f + getWidth() * 0.5f))
	{
		getTransform()->position.x = 0 + getWidth() * 0.5f;
		getRigidBody()->velocity.x *= -0.9f;
	}
	// hits bottom wall
	if (getTransform()->position.y >= (600.0f - getWidth()* 0.5f)) 
	{	
		getTransform()->position.y = 600.0f - getWidth() * 0.5f;
		getRigidBody()->velocity.y *= -0.9f;
	}
	// hits top wall
	else if (getTransform()->position.y <= (0.0f + getWidth() * 0.5f))
	{
		getTransform()->position.y = 0 + getWidth() * 0.5f;
		getRigidBody()->velocity.y *= -0.9f;

	}
}

void Target::m_reset()
{
	getTransform()->position = (glm::vec2(Util::RandomRange(0.0f, 800.0f - getWidth()), Util::RandomRange(0.0f, 600.0f - getWidth())));
	getRigidBody()->velocity = (glm::vec2(Util::RandomRange(-1.0f, 1.0f), 3.0f));
}
