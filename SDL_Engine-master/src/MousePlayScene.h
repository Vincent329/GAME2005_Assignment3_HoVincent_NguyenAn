#pragma once
#ifndef __MOUSE_PLAY_SCENE__
#define __MOUSE_PLAY_SCENE__

#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "MousePlayer.h"
#include "Button.h"
#include "Target.h"
#include "Label.h"
#include "Enemy.h"
#include "Util.h"
#include "CollisionManager.h"

class MousePlayScene : public Scene
{
public:
	MousePlayScene();
	~MousePlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;

	float getPPM();
	void setPPM(float _PPM);
	bool getIsPlaying();
	void setIsPlaying(bool _play);
	//// physics calculations
	//float subtractMomentum(float mass, float velocity);
	//float calculateVelocity(float mass, float momentumm);
	//float energyCalculation;

private:
	// IMGUI Function
	void GUI_Function() const;
	std::string m_guiTitle;

	// Mouse Coordinates
	int xMouse, yMouse;
	
	// PPM Scale
	float m_PPM;
	bool isPlaying;

	// Player Items
	Plane* m_pPlaneSprite;
	Player* m_pPlayer;
	MousePlayer* m_pMousePlayer;
	Enemy* m_Enemy;

	// Bouncing Ball Test
	Target* m_pBall;
	bool m_playerFacingRight;

	// physics properties
	float massPlayer = 5.0f;
	float massBall = 2.5f;


	// UI Items
	Button* m_pBackButton;
	Button* m_pNextButton;
	Label* m_pInstructionsLabel;
};

#endif  /* defined (__MOUSE_PLAY_SCENE__) */