#include "Enemy.h"
#include "TextureManager.h"

Enemy::Enemy() {
	TextureManager::Instance()->load("../Assets/textures/bullet.png", "enemy");

	auto size = TextureManager::Instance()->getTextureSize("enemy");
	setWidth(size.x);
	setHeight(size.y);

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

		TextureManager::Instance()->draw("enemy", x, y, 0, 255, true);
	}
}

void Enemy::update() {
	if (active)
	{
		float deltaTime = 1.0f / 60.0f;
		getRigidBody()->velocity = getRigidBody()->velocity + (getRigidBody()->acceleration * deltaTime);
		getTransform()->position = getTransform()->position + getRigidBody()->velocity * deltaTime;
	}
}

void Enemy::clean() {

}


void Enemy::Reset() {
	getRigidBody()->velocity = glm::vec2(0.0f, 50.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 9.8f);
	getRigidBody()->isColliding = false;
}
