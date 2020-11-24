#include "CollisionManager.h"
#include "Util.h"
#include <algorithm>
#include <limits>



int CollisionManager::squaredDistance(const glm::vec2 p1, const glm::vec2 p2)
{
	const int diffOfXs = p2.x - p1.x;
	const int diffOfYs = p2.y - p1.y;
	const auto result = diffOfXs * diffOfXs + diffOfYs * diffOfYs;

	return result;
}

bool CollisionManager::squaredRadiusCheck(GameObject* object1, GameObject* object2)
{
	glm::vec2 P1 = object1->getTransform()->position;
	glm::vec2 P2 = object2->getTransform()->position;
	const int halfHeights = (object1->getHeight() + object2->getHeight()) * 0.5f;

	//if (glm::distance(P1, P2) < halfHeights) {

	if (CollisionManager::squaredDistance(P1, P2) < (halfHeights * halfHeights)) {
		if (!object2->getRigidBody()->isColliding) {

			object2->getRigidBody()->isColliding = true;

			switch (object2->getType()) {
			case TARGET:	// change this for the bullet pool too.  Use Target for now for the sake of consistency
							// add own sound files should you wish 
				std::cout << "Collision with Target!" << std::endl;
				SoundManager::Instance().playSound("yay", 0);
				break;
			default:
				
				break;
			}

			return true;
		}
		return false;
	}
	else
	{
		object2->getRigidBody()->isColliding = false;
		return false;
	}
}

bool CollisionManager::AABBCheck(GameObject* object1, GameObject* object2)
{
	// prepare relevant variables
	const auto p1 = object1->getTransform()->position;
	const auto p2 = object2->getTransform()->position;
	const float p1Width = object1->getWidth();
	const float p1Height = object1->getHeight();
	const float p2Width = object2->getWidth();
	const float p2Height = object2->getHeight();

	if (
		p1.x < p2.x + p2Width &&
		p1.x + p1Width > p2.x&&
		p1.y < p2.y + p2Height &&
		p1.y + p1Height > p2.y
		)
	{
		if (!object2->getRigidBody()->isColliding) {

			object2->getRigidBody()->isColliding = true;

			switch (object2->getType()) {
			case TARGET:
				std::cout << "Collision with Target!" << std::endl;
				SoundManager::Instance().playSound("yay", 0);
				break;
			case PLAYER:
				std::cout << "Collision with Target!" << std::endl;
				SoundManager::Instance().playSound("yay", 0);
				break;
			default:
				
				break;
			}

			return true;
		}
		return false;
	}
	else
	{
		object2->getRigidBody()->isColliding = false;
		return false;
	}

	return false;
}

bool CollisionManager::lineLineCheck(const glm::vec2 line1_start, const glm::vec2 line1_end, const glm::vec2 line2_start, const glm::vec2 line2_end)
{
	const auto x1 = line1_start.x;
	const auto x2 = line1_end.x;
	const auto x3 = line2_start.x;
	const auto x4 = line2_end.x;
	const auto y1 = line1_start.y;
	const auto y2 = line1_end.y;
	const auto y3 = line2_start.y;
	const auto y4 = line2_end.y;

	// calculate the distance to intersection point
	const auto uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
	const auto uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

	// if uA and uB are between 0-1, lines are colliding
	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1)
	{
		return true;
	}

	return false;
}

bool CollisionManager::lineRectCheck(const glm::vec2 line1_start, const glm::vec2 line1_end, const glm::vec2 rec_start, const float rect_width, const float rect_height)
{
	const auto x1 = line1_start.x;
	const auto x2 = line1_end.x;
	const auto y1 = line1_start.y;
	const auto y2 = line1_end.y;
	const auto rx = rec_start.x;
	const auto ry = rec_start.y;
	const auto rw = rect_width;
	const auto rh = rect_height;

	// check if the line has hit any of the rectangle's sides
	// uses the Line/Line function below
	const auto left = lineLineCheck(glm::vec2(x1, y1), glm::vec2(x2, y2), glm::vec2(rx, ry), glm::vec2(rx, ry + rh));
	const auto right = lineLineCheck(glm::vec2(x1, y1), glm::vec2(x2, y2), glm::vec2(rx + rw, ry), glm::vec2(rx + rw, ry + rh));
	const auto top = lineLineCheck(glm::vec2(x1, y1), glm::vec2(x2, y2), glm::vec2(rx, ry), glm::vec2(rx + rw, ry));
	const auto bottom = lineLineCheck(glm::vec2(x1, y1), glm::vec2(x2, y2), glm::vec2(rx, ry + rh), glm::vec2(rx + rw, ry + rh));

	// if ANY of the above are true, the line
	// has hit the rectangle
	if (left || right || top || bottom) {
		return true;
	}

	return false;
}

int CollisionManager::minSquaredDistanceLineLine(glm::vec2 line1_start, glm::vec2 line1_end, glm::vec2 line2_start, glm::vec2 line2_end)
{
	auto u = line1_end - line1_start;
	auto v = line2_end - line2_start;
	auto w = line1_start - line2_start;
	auto a = Util::dot(u, u);         // always >= 0
	auto b = Util::dot(u, v);
	auto c = Util::dot(v, v);         // always >= 0
	auto d = Util::dot(u, w);
	auto e = Util::dot(v, w);
	auto D = a * c - b * b;					// always >= 0
	float sc, tc;

	// compute the line parameters of the two closest points
	if (D < Util::EPSILON) {					// the lines are almost parallel
		sc = 0.0;
		tc = (b > c ? d / b : e / c);			// use the largest denominator
	}
	else {
		sc = (b * e - c * d) / D;
		tc = (a * e - b * d) / D;
	}

	// get the difference of the two closest points
	auto dP = w + (sc * u) - (tc * v);  // =  L1(sc) - L2(tc)

	auto norm = Util::dot(dP, dP);
	return norm;
}

bool CollisionManager::lineAABBCheck(Ship* object1, GameObject* object2)
{
	const auto lineStart = object1->getTransform()->position;
	const auto lineEnd = object1->getTransform()->position + object1->getCurrentDirection() * 100.0f;
	// aabb
	const auto boxWidth = object2->getWidth();
	const int halfBoxWidth = boxWidth * 0.5f;
	const auto boxHeight = object2->getHeight();
	const int halfBoxHeight = boxHeight * 0.5f;
	const auto boxStart = object2->getTransform()->position - glm::vec2(halfBoxWidth, halfBoxHeight);

	if (lineRectCheck(lineStart, lineEnd, boxStart, boxWidth, boxHeight))
	{
		switch (object2->getType()) {
		case TARGET:
			std::cout << "Collision with Obstacle!" << std::endl;
			SoundManager::Instance().playSound("yay", 0);

			break;

		case PLAYER:
			break;
		default:
			
			break;
		}

		return true;
	}

	return false;
}

int CollisionManager::circleAABBsquaredDistance(const glm::vec2 circle_centre, int circle_radius, const glm::vec2 box_start, const int box_width, const int box_height)
{
	auto dx = std::max(box_start.x - circle_centre.x, 0.0f); 
	dx = std::max(dx, circle_centre.x - (box_start.x + box_width));
	auto dy = std::max(box_start.y - circle_centre.y, 0.0f);
	dy = std::max(dy, circle_centre.y - (box_start.y + box_height));

	return (dx * dx) + (dy * dy);
}

bool CollisionManager::circleAABBCheck(GameObject* object1, GameObject* object2)
{
	// circle
	const auto circleCentre = object1->getTransform()->position;
	const int circleRadius = std::max(object1->getWidth() * 0.5f, object1->getHeight() * 0.5f);
	// aabb
	const auto boxWidth = object2->getWidth();
	int halfBoxWidth = boxWidth * 0.5f; // just so we don't calculate this over again
	const auto boxHeight = object2->getHeight();
	int halfBoxHeight = boxHeight * 0.5f;

	const auto boxStart = object2->getTransform()->position - glm::vec2(boxWidth * 0.5f, boxHeight * 0.5f);

	if (circleAABBsquaredDistance(circleCentre, circleRadius, boxStart, boxWidth, boxHeight) <= (circleRadius * circleRadius)) // squared for a more accurate calculation and no loss of dataa
	{
		if (!object2->getRigidBody()->isColliding) {

			object2->getRigidBody()->isColliding = true;

			const auto attackVector = object1->getTransform()->position - object2->getTransform()->position; // this is a glm::vec2, difference in position
			const auto normal = glm::vec2(0.0f, -1.0f);

			std::cout << "Attack X: " << attackVector.x << std::endl;
			std::cout << "Attack Y: " << attackVector.y << std::endl;

			const auto dot = Util::dot(attackVector, normal); // a float value, this is the angle to calculate the hit
			const auto angle = acos(dot / Util::magnitude(attackVector)) * Util::Rad2Deg; // normalized

			std::cout << "Angle: " << angle << std::endl;

			//switch (object2->getType()) {
			//case TARGET:
			//	std::cout << "Collision with Planet!" << std::endl;
			//	SoundManager::Instance().playSound("yay", 0);
			//	break;

			//case PLAYER:
			//{
			//	std::cout << "COLLISION DETECTED" << std::endl;

			//	SoundManager::Instance().playSound("yay", 0);

			//	auto velocityX = object1->getRigidBody()->velocity.x;
			//	auto velocityY = object1->getRigidBody()->velocity.y;

			//	if ((attackVector.x > 0 && attackVector.y < 0) || (attackVector.x < 0 && attackVector.y < 0))
			//		// top right or top left
			//	{

			//		if (angle <= 45)
			//		{
			//			object1->getRigidBody()->velocity = glm::vec2(velocityX, -velocityY);
			//			object1->getRigidBody()->velocity +=
			//				(glm::vec2(static_cast<MousePlayer*>(object2)->getVelocityX(), static_cast<MousePlayer*>(object2)->getVelocityY()) * 10.0f);

			//		}
			//		else
			//		{
			//			object1->getRigidBody()->velocity = glm::vec2(-velocityX, velocityY);
			//			object1->getRigidBody()->velocity +=
			//				(glm::vec2(static_cast<MousePlayer*>(object2)->getVelocityX(), static_cast<MousePlayer*>(object2)->getVelocityY()) * 10.0f);
			//		}
			//	}

			//	if ((attackVector.x > 0 && attackVector.y > 0) || (attackVector.x < 0 && attackVector.y > 0))
			//		// bottom right or bottom left
			//	{
			//		if (angle <= 135)
			//		{
			//			object1->getRigidBody()->velocity = glm::vec2(-velocityX, velocityY);
			//			object1->getRigidBody()->velocity +=
			//				(glm::vec2(static_cast<MousePlayer*>(object2)->getVelocityX(), static_cast<MousePlayer*>(object2)->getVelocityY()) * 10.0f);
			//		}
			//		else
			//		{
			//			object1->getRigidBody()->velocity = glm::vec2(velocityX, -velocityY);
			//			object1->getRigidBody()->velocity +=
			//				(glm::vec2(static_cast<MousePlayer*>(object2)->getVelocityX(), static_cast<MousePlayer*>(object2)->getVelocityY()) * 10.0f);
			//		}
			//	}
			//}
			//break;
			//
			//case SHIP:
			//{
			//	SoundManager::Instance().playSound("thunder", 0);
			//	auto velocityX = object1->getRigidBody()->velocity.x;
			//	auto velocityY = object1->getRigidBody()->velocity.y;

			//	if ((attackVector.x > 0 && attackVector.y < 0) || (attackVector.x < 0 && attackVector.y < 0))
			//		// top right or top left
			//	{

			//		if (angle <= 45)
			//		{
			//			object1->getRigidBody()->velocity = glm::vec2(velocityX, -velocityY);
			//		}
			//		else
			//		{
			//			object1->getRigidBody()->velocity = glm::vec2(-velocityX, velocityY);
			//		}
			//	}

			//	if ((attackVector.x > 0 && attackVector.y > 0) || (attackVector.x < 0 && attackVector.y > 0))
			//		// bottom right or bottom left
			//	{
			//		if (angle <= 135)
			//		{
			//			object1->getRigidBody()->velocity = glm::vec2(-velocityX, velocityY);
			//		}
			//		else
			//		{
			//			object1->getRigidBody()->velocity = glm::vec2(velocityX, -velocityY);
			//		}
			//	}
			//}
			//break;
			//default:

			//	break;
			//}

			return true;
		}
		return false;
	}
	else
	{
	object2->getRigidBody()->isColliding = false;
	return false;
	}

	return false;
}


bool CollisionManager::collisionCheck(GameObject* object1, GameObject* object2)
{
	return 0;
}

// returns entry time based on inverse time, and the float factor will move the ball out of the way
float CollisionManager::sweptAABB(GameObject* object1, GameObject* object2, float& normalx, float& normaly)
{
	// inverse time
	float xInvEntry, yInvEntry; // how far the closest edges of the objects are from each other
	float xInvExit, yInvExit;   // distance to the far side of the object

	// moving particle
	const auto boxWidth = object1->getWidth();
	const int halfBoxWidth = boxWidth * 0.5f;
	const auto boxHeight = object1->getHeight();
	const int halfBoxHeight = boxHeight * 0.5f;
	const auto targetBoxStart = object1->getTransform()->position - glm::vec2(halfBoxWidth, halfBoxHeight);

	// the ship
	const auto playerBoxWidth = object2->getWidth();
	const int halfPlayerBoxWidth = playerBoxWidth * 0.5f;
	const auto playerBoxHeight = object2->getHeight();
	const int halfPlayerBoxHeight = playerBoxHeight * 0.5f;
	const auto playerBoxStart = object2->getTransform()->position - glm::vec2(playerBoxWidth, playerBoxHeight);
	
	// distance between the near AND far sides for both x and y
	// going right
	if (object1->getRigidBody()->velocity.x > 0.0f)
	{
		xInvEntry = playerBoxStart.x - (targetBoxStart.x + object1->getWidth());
		xInvExit = (playerBoxStart.x + object2->getWidth()) - targetBoxStart.x;
		/*std::cout << "xInvEntry: " << xInvEntry << std::endl;
		std::cout << "xInvExit: " << xInvExit << std::endl;*/
		//std::cout << "Going Right" << std::endl;
	}
	else {
		xInvEntry = (playerBoxStart.x + object2->getWidth()) - targetBoxStart.x;
		xInvExit = playerBoxStart.x - (targetBoxStart.x + object1->getWidth());
		//std::cout << "xInvEntry: " << xInvEntry << std::endl;
		//std::cout << "xInvExit: " << xInvExit << std::endl;
		//std::cout << "Going Left" << std::endl;
	}
	if (object1->getRigidBody()->velocity.y < 0.0f)
	{
		yInvEntry = playerBoxStart.y - (targetBoxStart.y + object1->getHeight());
		yInvExit = (playerBoxStart.y + object2->getHeight()) - targetBoxStart.y;
		/*	std::cout << "yInvEntry: " << yInvEntry << std::endl;
			std::cout << "yInvExit: " << yInvExit << std::endl;*/
			//std::cout << "Going Up" << std::endl;
	}
	else {
		yInvEntry = (playerBoxStart.y + object2->getHeight()) - targetBoxStart.y;
		yInvExit = playerBoxStart.y - (targetBoxStart.y + object1->getHeight());
		/*std::cout << "yInvEntry: " << yInvEntry << std::endl;
		std::cout << "yInvExit: " << yInvExit << std::endl;*/
		//std::cout << "Going Down" << std::endl;
	}

	float xEntry, yEntry;
	float xExit, yExit;

	if (object1->getRigidBody()->velocity.x == 0.0f)
	{
		xEntry = -std::numeric_limits<float>::infinity();
		xExit = std::numeric_limits<float>::infinity();
	}
	else {
		xEntry = xInvEntry / object1->getRigidBody()->velocity.x;
		xExit = xInvExit / object1->getRigidBody()->velocity.x;
		//std::cout << "X Entry: " << xEntry << std::endl;
	}
	if (object1->getRigidBody()->velocity.y == 0.0f)
	{
		yEntry = -std::numeric_limits<float>::infinity();
		yExit = std::numeric_limits<float>::infinity();
	}
	else {
		yEntry = yInvEntry / object1->getRigidBody()->velocity.y;
		yExit = yInvExit / object1->getRigidBody()->velocity.y;
		//std::cout << "Y Entry: " << yEntry << std::endl;
	}

	// find earliest and latest times of collision
	float entryTime = std::max(xEntry, yEntry);
	float exitTime = std::max(xExit, yExit);
	
	//std::cout << entryTime << std::endl;

	// NO COLLISION DETECTED, want to find a value between 0 and 1
	if (entryTime > exitTime || xEntry < 0.0f && yEntry < 0.0f || xEntry > 1.0f || yEntry > 1.0f)
	{
		normalx = 0.0f;
		normaly = 0.0f;
		std::cout << "No collision" << std::endl;
		return 1.0f;
	}
	// if there is a collision
	else {
		if (xEntry > yEntry)
		{
			if (xInvEntry < 0.0f)
			{
				normalx = 1.0f;
				normaly = 0.0f;
				std::cout << "Coming from right" << std::endl;
			}
			else
			{
				normalx = -1.0f;
				normaly = 0.0f;
				std::cout << "Coming from left" << std::endl;
			}
		}
		else {
			if (yInvEntry > 0.0f)
			{
				normalx = 0.0f;
				normaly = 1.0f;
				std::cout << "Coming from top" << std::endl;
			}
			else {
				normalx = 0.0f;
				normaly = -1.0f;
				std::cout << "Coming from bottom" << std::endl;
			}
		}
		return entryTime;
	}
}

bool CollisionManager::pointRectCheck(const glm::vec2 point, const glm::vec2 rect_start, const float rect_width, const float rect_height)
{
	const float topLeftX = rect_start.x - rect_width * 0.5;
	const float topLeftY = rect_start.y - rect_height * 0.5;
	const auto width = rect_width;
	const auto height = rect_height;

	if (point.x > topLeftX&&
		point.x < topLeftX + width &&
		point.y > topLeftY&&
		point.y < topLeftY + height)
	{
		return true;
	}
	return false;
}

// Collision Response (Elastic Collision)
// if mass of puck is less than mass of player

// m1v1(initial) + m2v2(initial) = m1v1(final) + m2v2(final) 
// (FOR BOTH X AND Y COMPONENTS)

// if mass of player is much greater than ball
// player continues in motion unaltered but the ball continues with a speed of about TWICE the initial speed of player (IF AT REST)
// If the light ball hits the heavy particle at rest (mouse doesn't move), light particle has its velocity REVERSED (it's already in the code here)


CollisionManager::CollisionManager()
= default;


CollisionManager::~CollisionManager()
= default;
