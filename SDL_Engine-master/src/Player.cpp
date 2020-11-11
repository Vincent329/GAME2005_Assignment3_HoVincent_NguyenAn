#include "Player.h"
#include "Util.h"
#include "TextureManager.h"

Player::Player(): m_currentAnimationState(PLAYER_IDLE_RIGHT)
{
	TextureManager::Instance()->loadSpriteSheet(
		"../Assets/sprites/atlas.txt",
		"../Assets/sprites/atlas.png", 
		"spritesheet");

	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("spritesheet"));
	
	// set frame width
	setWidth(53);

	// set frame height
	setHeight(58);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(PLAYER);

	m_buildAnimations();
}

Player::~Player()
= default;

void Player::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the player according to animation state
	switch(m_currentAnimationState)
	{
	case PLAYER_IDLE_RIGHT:
		TextureManager::Instance()->playAnimation("spritesheet", getAnimation("idle"),
			x, y, 0.12f, 0, 255, true);
		break;
	case PLAYER_IDLE_LEFT:
		TextureManager::Instance()->playAnimation("spritesheet", getAnimation("idle"),
			x, y, 0.12f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;
	case PLAYER_RUN_RIGHT:
		TextureManager::Instance()->playAnimation("spritesheet", getAnimation("run"),
			x, y, 0.25f, 0, 255, true);
		break;
	case PLAYER_RUN_LEFT:
		TextureManager::Instance()->playAnimation("spritesheet", getAnimation("run"),
			x, y, 0.25f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;
	default:
		break;
	}
	
}

void Player::update()
{
	const float deltaTime = 1.0f / 60.f;

	// must normalize
	float dirMagnitude = Util::magnitude(m_direction);
	if (dirMagnitude > 0) {
		// normalize the vector
		getRigidBody()->acceleration = Util::normalize(m_direction) * ACCELERATION; // direction vector multiplied by acceleration when we have a direction input
	}
	else if (Util::magnitude(getRigidBody()->velocity) > 0) { // no input but we need to check if velocity still goes
	 // normalize current velocity when we're not inputting anything, just give us direction vector when magnitude is 1
		getRigidBody()->acceleration = Util::normalize(getRigidBody()->velocity) * -ACCELERATION; // so we make sure that our direction is slowing down every frame by making sure velocity goes opposite of our acceleration
	}

	// once velocity slows down to the point it's less than the magnitude of our acceleration.  Safety net friction
	if (Util::magnitude(getRigidBody()->velocity) < Util::magnitude(getRigidBody()->acceleration) && Util::magnitude(getRigidBody()->velocity) > 0) {
		getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	}

	//if (Util::magnitude(getRigidBody()->velocity) >= MAXVELOCITY)
	//{
	//	getRigidBody()->velocity = MAXVELOCITY;
	//}
	getRigidBody()->velocity += getRigidBody()->acceleration;


	glm::vec2 pos = getTransform()->position;
	pos.x += getRigidBody()->velocity.x * deltaTime;
	pos.y += getRigidBody()->velocity.y * deltaTime;

	//std::cout << getRigidBody()->acceleration.x << std::endl;
	//std::cout << getRigidBody()->acceleration.y << std::endl;

	getTransform()->position = pos;
}

void Player::clean()
{
}

// MOVEMENT CODE
void Player::moveLeft() {
	m_direction.x = -1;
}

void Player::moveRight() {
	m_direction.x = 1;

}

void Player::moveUp() {
	m_direction.y = -1;

}

void Player::moveDown() {
	m_direction.y = 1;
}

void Player::stopMovingX() {
	m_direction.x = 0;
}

void Player::stopMovingY() {
	m_direction.y = 0;
}

bool Player::isColliding(GameObject* pOther) {
	// Works for square sprites only
	float myRadius = getWidth() * 0.5f;
	float otherRadius = pOther->getWidth() * 0.5f;

	return (getDistance(pOther) <= myRadius + otherRadius);
}

float Player::getDistance(GameObject* pOther) {
	glm::vec2 myPos = getTransform()->position;
	glm::vec2 otherPos = pOther->getTransform()->position;

	// Use pythagorean to calculate distance c = sqrt(a^2 + b^2)
	float a = myPos.x - otherPos.x;
	float b = myPos.y - otherPos.y;
	return sqrt(a * a + b * b);
}


void Player::setAnimationState(const PlayerAnimationState new_state)
{
	m_currentAnimationState = new_state;
}

void Player::m_buildAnimations()
{
	Animation idleAnimation = Animation();

	idleAnimation.name = "idle";
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("megaman-idle-0"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("megaman-idle-1"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("megaman-idle-2"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("megaman-idle-3"));

	setAnimation(idleAnimation);

	Animation runAnimation = Animation();

	runAnimation.name = "run";
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("megaman-run-0"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("megaman-run-1"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("megaman-run-2"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("megaman-run-3"));

	setAnimation(runAnimation);
}
