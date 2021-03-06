#include "Target.h"
#include "TextureManager.h"


Target::Target()
{
	TextureManager::Instance()->load("../Assets/textures/energy.png","circle");
	TextureManager::Instance()->load("../Assets/textures/crate.png", "square");

	const auto size = TextureManager::Instance()->getTextureSize("circle");
	setWidth(size.x);
	setHeight(size.y);
	/*getTransform()->position = glm::vec2(100.0f, 100.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);*/
	m_reset();
	mass = 2.5f;
	wallCoefficient = 0.9f;
	showHitbox = false;
	setCollisionType(CIRCLE);
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
	if (collisionType == CIRCLE)
	{
		TextureManager::Instance()->draw("circle", x, y, 0, 255, true);
		if (showHitbox)
		Util::DrawCircle(getTransform()->position, std::max(getWidth() * 0.5f, getHeight() * 0.5f));

	}
	else if (collisionType == RECTANGLE)
	{
		TextureManager::Instance()->draw("square", x, y, 0, 255, true);
		if (showHitbox)
		Util::DrawRect(getTransform()->position - glm::vec2(getWidth() * 0.5, getHeight() * .5f), getWidth(), getHeight());

	}
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

float Target::getWallCoefficient()
{
	return wallCoefficient;
}

void Target::setWallCoefficient(float _wall)
{
	wallCoefficient = _wall;
}

CollisionShape Target::getCollisionType()
{
	return collisionType;
}

void Target::setCollisionType(CollisionShape shape)
{
	collisionType = shape;
	if (collisionType == CIRCLE)
	{
		const auto size = TextureManager::Instance()->getTextureSize("circle");
		setWidth(size.x);
		setHeight(size.y);
	}
	else if (collisionType == RECTANGLE)
	{
		const auto size = TextureManager::Instance()->getTextureSize("square");
		setWidth(size.x);
		setHeight(size.y);
	}
}

bool Target::getShowHitbox()
{
	return showHitbox;
}

void Target::setShowHitbox(bool box)
{
	showHitbox = box;
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
		getRigidBody()->velocity.x *= -wallCoefficient;
	}
	// hits left wall
	else if (getTransform()->position.x <= (0.0f + getWidth() * 0.5f))
	{
		getTransform()->position.x = 0 + getWidth() * 0.5f;
		getRigidBody()->velocity.x *= -wallCoefficient;
	}
	// hits bottom wall
	if (getTransform()->position.y >= (600.0f - getWidth()* 0.5f)) 
	{	
		getTransform()->position.y = 600.0f - getWidth() * 0.5f;
		getRigidBody()->velocity.y *= -wallCoefficient;
	}
	// hits top wall
	else if (getTransform()->position.y <= (0.0f + getWidth() * 0.5f))
	{
		getTransform()->position.y = 0 + getWidth() * 0.5f;
		getRigidBody()->velocity.y *= -wallCoefficient;

	}
}

void Target::m_reset()
{
	getTransform()->position = (glm::vec2(Util::RandomRange(0.0f, 800.0f - getWidth()), Util::RandomRange(0.0f, 600.0f - getWidth())));
	getRigidBody()->velocity = (glm::vec2(Util::RandomRange(-1.0f, 1.0f), 3.0f));
	setCollisionType(CIRCLE);
}
