#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Button.h"
#include "BulletPool.h"
#include "Label.h"
#include "Enemy.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;


private:
	// IMGUI Function
	void GUI_Function() const;
	std::string m_guiTitle;
	
	// bullet manager items
	const float bulletSpawnTimerDuration = 500.0f; // 3 seconds = 3000 milliseconds
	float bulletSpawnTimerStart;
	
	glm::vec2 m_mousePosition;

	Plane* m_pPlaneSprite;
	Player* m_pPlayer;
	Enemy* m_Enemy;
	BulletPool* m_pPool;
	//BulletManager* m_pBulletManager;

	bool m_playerFacingRight;

	// UI Items
	Button* m_pBackButton;
	Button* m_pNextButton;
	Label* m_pInstructionsLabel;

	void SpawnBullet();

	//void SpawnBullet();
};

#endif /* defined (__PLAY_SCENE__) */