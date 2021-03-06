#include "MousePlayer.h"
#include "Util.h"
#include "TextureManager.h"

MousePlayer::MousePlayer() : m_currentAnimationState(PLAYER_IDLE_RIGHT)
{
	TextureManager::Instance()->loadSpriteSheet(
		"../Assets/sprites/atlas.txt",
		"../Assets/sprites/atlas.png",
		"spritesheet");

	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("spritesheet"));

	TextureManager::Instance()->load("../Assets/textures/paddle.png", "paddle");
	auto size = TextureManager::Instance()->getTextureSize("paddle");
	// set frame width
	setWidth(size.x);

	// set frame height
	setHeight(size.y);

	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->isColliding = false;
	velocityX = 0.0f;
	velocityY = 0.0f;
	initialVelocityX = 0.0f;
	initialVelocityY = 0.0f;
	mass = 5.0f;
	isFlipped = false;
	flipAngle = 0.0f; 

	// pixels per meter factor
	m_PPM = 2.0f;
	isPlaying = true;
	showHitbox = false;

	lastUpdateTime = SDL_GetTicks();
	initialPosition = getTransform()->position;
	setType(PLAYER);

	m_buildAnimations();
}

MousePlayer::~MousePlayer()
= default;

void MousePlayer::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	TextureManager::Instance()->draw("paddle", x, y, flipAngle, 255, true);
	if (showHitbox)
	{
		Util::DrawRect(getTransform()->position - glm::vec2(getWidth() * 0.5, getHeight() * .5f), getWidth(), getHeight());
	}
	// draw the Player according to animation state
	/*switch (m_currentAnimationState)
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

void MousePlayer::update()
{
	// Testing velocity check

	const float deltaTime = 1.0f / 60.f; // fixed timestep

	int currentTime = SDL_GetTicks();
	glm::vec2 currentPosition = getTransform()->position;

	float dTime = (currentTime - lastUpdateTime) / 1000.0f; // variable timestep

	//velocityX = (currentPosition.x - initialPosition.x) * dTime * 10.0f;
	//velocityY = (currentPosition.y - initialPosition.y) * dTime * 10.0f;
	//std::cout << "Velocity magnitude: " << Util::magnitude(glm::vec2(velocityX, velocityY)) << std::endl;

	//// Calculate the velocity of the player during runtime
}

void MousePlayer::clean()
{
}

// MOVEMENT CODE
void MousePlayer::moveLeft() {
	m_direction.x = -1;
}

void MousePlayer::moveRight() {
	m_direction.x = 1;

}

void MousePlayer::moveUp() {
	m_direction.y = -1;

}

void MousePlayer::moveDown() {
	m_direction.y = 1;
}

void MousePlayer::stopMovingX() {
	m_direction.x = 0;
}

void MousePlayer::stopMovingY() {
	m_direction.y = 0;
}

void MousePlayer::mouseMovement(int x, int y)
{
	glm::vec2 direction = glm::vec2(x - getTransform()->position.x, y - getTransform()->position.y);
	getTransform()->position += direction;
	setVelocityX(direction.x / 4);
	setVelocityY(direction.y / 4);
}

float MousePlayer::getMass()
{
	return mass;
}

void MousePlayer::setMass(float m_mass)
{
	mass = m_mass;
}

float MousePlayer::getVelocityX()
{
	return velocityX;
}

void MousePlayer::setVelocityX(float _velocityX)
{
	velocityX = _velocityX;
}

float MousePlayer::getVelocityY()
{
	return velocityY;
}

void MousePlayer::setVelocityY(float _velocityY)
{
	velocityY = _velocityY;
}

float MousePlayer::getInitialVelocityX()
{
	return initialVelocityX;
}

void MousePlayer::setInitialVelocityX(float _ivelocityX)
{
	initialVelocityX = _ivelocityX;
}

float MousePlayer::getInitialVelocityY()
{
	return initialVelocityY;
}

void MousePlayer::setInitialVelocityY(float _ivelocityY)
{
	initialVelocityY = _ivelocityY;
}

void MousePlayer::flipPaddle()
{
	auto size = TextureManager::Instance()->getTextureSize("paddle");
	
	isFlipped = !isFlipped;
	if (isFlipped == false)
	{
		flipAngle = 0.0f;
		// set frame width
		setWidth(size.x);

		// set frame height
		setHeight(size.y);
	}
	else {
		flipAngle = 90.0f;
		// set frame width
		setWidth(size.y);

		// set frame height
		setHeight(size.x);
	}
}

bool MousePlayer::getShowHitbox()
{
	return showHitbox;
}

void MousePlayer::setShowHitbox(bool box)
{
	showHitbox = box;
}

bool MousePlayer::isColliding(GameObject * pOther) {
	// Works for square sprites only
	float myRadius = getWidth() * 0.5f;
	float otherRadius = pOther->getWidth() * 0.5f;

	return (getDistance(pOther) <= myRadius + otherRadius);
}

float MousePlayer::getDistance(GameObject * pOther) {
	glm::vec2 myPos = getTransform()->position;
	glm::vec2 otherPos = pOther->getTransform()->position;

	// Use pythagorean to calculate distance c = sqrt(a^2 + b^2)
	float a = myPos.x - otherPos.x;
	float b = myPos.y - otherPos.y;
	return sqrt(a * a + b * b);
}


float MousePlayer::getPPM()
{
	return m_PPM;
}

void MousePlayer::setPPM(float pixels)
{
	m_PPM = pixels;
}

bool MousePlayer::getIsPlaying()
{
	return isPlaying;
}

void MousePlayer::setIsPlaying(bool play)
{
	isPlaying = play;
}

void MousePlayer::setAnimationState(const PlayerAnimationState new_state)
{
	m_currentAnimationState = new_state;
}

void MousePlayer::m_buildAnimations()
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
