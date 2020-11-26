#include "Enemy.h"
#include "TextureManager.h"
#include "Util.h"

Enemy::Enemy() {
	TextureManager::Instance()->load("../Assets/textures/bullet.png", "enemy");

	auto size = TextureManager::Instance()->getTextureSize("enemy");
	setWidth(size.x);
	setHeight(size.y);
	initialVelocity = 50.0f;
	gravity = 9.8f;
	getTransform()->position = glm::vec2(700.0f, 300.0f);
	setType(ENEMY);
	Reset();
}

Enemy::~Enemy()
= default;

void Enemy::draw() {
	// alias for x and y
	if (active)
	{
		const auto x = getTransform()->position.x;
		const auto y = getTransform()->position.y;
		const auto width = getWidth() / 2;
		const auto height = getHeight() / 2;

		glm::vec2 ballPos[4];
		ballPos[0] = glm::vec2(x - width, y - height);
		ballPos[1] = glm::vec2(x + width, y - height);
		ballPos[2] = glm::vec2(x - width, y + height);
		ballPos[3] = glm::vec2(x + width, y + height);

		if (showHitbox)
		{
			Util::DrawLine(ballPos[0], ballPos[1]);
			Util::DrawLine(ballPos[2], ballPos[3]);
			Util::DrawLine(ballPos[0], ballPos[2]);
			Util::DrawLine(ballPos[1], ballPos[3]);
		}


		TextureManager::Instance()->draw("enemy", x, y, 0, 255, true);
	}
}

void Enemy::update() {
	if (active)
	{
		float deltaTime = 1.0f / 60.0f;
		lastPosition = getTransform()->position;
		getRigidBody()->velocity = getRigidBody()->velocity + (getRigidBody()->acceleration * deltaTime);
		getTransform()->position = getTransform()->position + getRigidBody()->velocity * deltaTime;
	}
}

void Enemy::clean() {

}


void Enemy::Reset() {
	getRigidBody()->velocity = glm::vec2(0.0f, initialVelocity);
	getRigidBody()->acceleration = glm::vec2(0.0f, gravity);
	getRigidBody()->isColliding = false;
}
