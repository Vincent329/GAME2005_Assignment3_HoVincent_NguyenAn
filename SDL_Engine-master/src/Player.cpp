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
	
	TextureManager::Instance()->load("../Assets/textures/galaga.png", "galaga");
	auto size = TextureManager::Instance()->getTextureSize("galaga");
	// set frame width
	setWidth(size.x);

	// set frame height
	setHeight(size.y);

	// set frame width
	setWidth(53);

	// set frame height
	setHeight(58);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	showHitbox = false;
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
	glm::vec2 startPos = glm::vec2(x - getWidth() / 2, y - getHeight() / 2);

	TextureManager::Instance()->draw("galaga", x, y, 0, 255, true);

	//Util::DrawRect(startPos, getWidth(), getHeight());
	updateBoundingBoxTop();
	updateBoundingBoxBottom();
	// draw the player according to animation state
	/*switch(m_currentAnimationState)
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
	}*/
	
}

void Player::update()
{
	// Testing velocity check

	const float deltaTime = 1.0f / 60.f;

	// must normalize
	float dirMagnitude = Util::magnitude(m_direction);
	if (dirMagnitude > 0) {
		// normalize the vector
		getRigidBody()->acceleration = Util::normalize(m_direction) * acceleration; // direction vector multiplied by acceleration when we have a direction input
	}
	else if (Util::magnitude(getRigidBody()->velocity) > 0) { // no input but we need to check if velocity still goes
	 // normalize current velocity when we're not inputting anything, just give us direction vector when magnitude is 1
		getRigidBody()->acceleration = Util::normalize(getRigidBody()->velocity) * -acceleration; // so we make sure that our direction is slowing down every frame by making sure velocity goes opposite of our acceleration
	}

	// once velocity slows down to the point it's less than the magnitude of our acceleration.  Safety net friction
	if (Util::magnitude(getRigidBody()->velocity) < Util::magnitude(getRigidBody()->acceleration) && Util::magnitude(getRigidBody()->velocity) > 0) {
		getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	}
	getRigidBody()->velocity += getRigidBody()->acceleration;

	glm::vec2 pos = getTransform()->position;
	pos.x += getRigidBody()->velocity.x * deltaTime;
	pos.y += getRigidBody()->velocity.y * deltaTime;
	getTransform()->position = pos;

	checkBounds();
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

void Player::mouseMovement(int x, int y)
{
	getTransform()->position.x = x;
	getTransform()->position.y = y;
}

void Player::checkBounds()
{
	// hits right wall 
	if (getTransform()->position.x >= (800.0f - getWidth() * 0.5f))
	{
		getTransform()->position.x = 800.0f - getWidth() * 0.5f;
	}
	// hits left wall
	else if (getTransform()->position.x <= (0.0f + getWidth() * 0.5f))
	{
		getTransform()->position.x = 0 + getWidth() * 0.5f;
	}
	// hits bottom wall
	if (getTransform()->position.y >= (600.0f - getWidth() * 0.5f))
	{
		getTransform()->position.y = 600.0f - getWidth() * 0.5f;
	}
	// hits top wall
	else if (getTransform()->position.y <= (0.0f + getWidth() * 0.5f))
	{
		getTransform()->position.y = 0 + getWidth() * 0.5f;

	}
}

float Player::getAcceleration()
{
	return acceleration;
}

void Player::setAcceleration(float accel)
{
	acceleration = accel;
}

SDL_Rect Player::getBoundingBoxTop()
{
	return boundingBoxTop;
}

SDL_Rect Player::getBoundingBoxBottom()
{
	return boundingBoxBottom;
}

void Player::updateBoundingBoxTop()
{
	boundingBoxTop.w = getWidth() * 0.5;
	boundingBoxTop.h = getHeight() * 0.5;
	boundingBoxTop.x = getTransform()->position.x - boundingBoxTop.w * 0.5;
	boundingBoxTop.y = getTransform()->position.y - boundingBoxTop.h;
	if (showHitbox)
	Util::DrawRect(glm::vec2(boundingBoxTop.x, boundingBoxTop.y), boundingBoxTop.w, boundingBoxTop.h);
}

void Player::updateBoundingBoxBottom()
{
	boundingBoxBottom.w = getWidth() + 20;
	boundingBoxBottom.h = getHeight() * 0.5;
	boundingBoxBottom.x = getTransform()->position.x - boundingBoxTop.w - 10;
	boundingBoxBottom.y = getTransform()->position.y;
	if (showHitbox)
	Util::DrawRect(glm::vec2(boundingBoxBottom.x, boundingBoxBottom.y), boundingBoxBottom.w, boundingBoxBottom.h);
}

bool Player::getShowHitbox()
{
	return showHitbox;
}

void Player::setShowHitbox(bool box)
{
	showHitbox = box;
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
