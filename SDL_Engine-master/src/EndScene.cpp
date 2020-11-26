#include "EndScene.h"
#include <algorithm>
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"

EndScene::EndScene()
{
	EndScene::start();
}

EndScene::~EndScene()
= default;

void EndScene::draw()
{
	drawDisplayList();
}

void EndScene::update()
{
	updateDisplayList();
}

void EndScene::clean()
{
	removeAllChildren();
}

void EndScene::handleEvents()
{
	EventManager::Instance().update();

	// Button Events
	m_pRestartButton->update();

	// Keyboard Events
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	}
}

void EndScene::start()
{
	const SDL_Color blue = { 0, 0, 255, 255 };
	const SDL_Color black = { 0, 0, 0 , 255 };

	m_label = new Label("END SCENE", "Dock51", 80, blue, glm::vec2(400.0f, 40.0f));
	m_label->setParent(this);
	addChild(m_label);
	m_Scene1 = new Label("Click here for Scene 1", "Consolas", 15, black, glm::vec2(200.0f, 350.0f));
	m_Scene1->setParent(this);
	addChild(m_Scene1);

	m_Scene2 = new Label("Click here for Scene 2", "Consolas", 15, black, glm::vec2(600.0f, 350.0f));
	m_Scene2->setParent(this);
	addChild(m_Scene2);

	// Restart Button
	m_pRestartButton = new Button("../Assets/textures/restartButton.png", "restartButton", RESTART_BUTTON);
	m_pRestartButton->getTransform()->position = glm::vec2(200.0f, 400.0f);
	m_pRestartButton->addEventListener(CLICK, [&]()-> void
	{
		m_pRestartButton->setActive(false);
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	});

	m_pRestartButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pRestartButton->setAlpha(128);
	});

	m_pRestartButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pRestartButton->setAlpha(255);
	});

	addChild(m_pRestartButton);

	m_pRestartMouseButton = new Button("../Assets/textures/restartButton.png", "restartButton", RESTART_BUTTON);
	m_pRestartMouseButton->getTransform()->position = glm::vec2(600.0f, 400.0f);
	m_pRestartMouseButton->addEventListener(CLICK, [&]()-> void
		{
			m_pRestartMouseButton->setActive(false);
			TheGame::Instance()->changeSceneState(PLAY_MOUSE_SCENE);
		});

	m_pRestartMouseButton->addEventListener(MOUSE_OVER, [&]()->void
		{
			m_pRestartMouseButton->setAlpha(128);
		});

	m_pRestartMouseButton->addEventListener(MOUSE_OUT, [&]()->void
		{
			m_pRestartMouseButton->setAlpha(255);
		});

	addChild(m_pRestartMouseButton);
}
