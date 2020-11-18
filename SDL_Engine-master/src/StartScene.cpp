#include "StartScene.h"
#include <algorithm>
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"

StartScene::StartScene()
{
	StartScene::start();
}

StartScene::~StartScene()
= default;

void StartScene::draw()
{
	drawDisplayList();
}

void StartScene::update()
{
	updateDisplayList();
}

void StartScene::clean()
{
	removeAllChildren();
}

void StartScene::handleEvents()
{
	EventManager::Instance().update();

	// Keyboard Events
	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	}
}

void StartScene::start()
{
	const SDL_Color blue = { 0, 0, 255, 255 };
	const SDL_Color black = { 0, 0, 0 , 255 };
	m_pStartLabel = new Label("ASSIGNMENT 3", "Consolas", 80, blue, glm::vec2(400.0f, 40.0f));
	m_pStartLabel->setParent(this);
	addChild(m_pStartLabel);

	m_pInstructionsLabel = new Label("Press 1 to Play", "Consolas", 40, blue, glm::vec2(400.0f, 120.0f));
	m_pInstructionsLabel->setParent(this);
	addChild(m_pInstructionsLabel);

	m_Vincent = new Label("Vincent Ho - 101334300", "Consolas", 20, black, glm::vec2(400.0F, 200.0f));
	m_Vincent->setParent(this);
	addChild(m_Vincent);

	m_An = new Label("An Nguyen - 101166265", "Consolas", 20, black, glm::vec2(400.0f, 230.0f));
	m_An->setParent(this);
	addChild(m_An);

	m_pShip = new Ship();
	m_pShip->getTransform()->position = glm::vec2(400.0f, 300.0f); 
	addChild(m_pShip); 

	// Start Button for SCENE 1
	m_pStartButton = new Button();
	m_pStartButton->getTransform()->position = glm::vec2(200.0f, 400.0f); 

	m_pStartButton->addEventListener(CLICK, [&]()-> void
	{
		m_pStartButton->setActive(false);
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	});
	
	m_pStartButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pStartButton->setAlpha(128);
	});

	m_pStartButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pStartButton->setAlpha(255);
	});
	addChild(m_pStartButton);

	// Start Button for SCENE 2

	m_pMouseStartButton = new Button();
	m_pMouseStartButton->getTransform()->position = glm::vec2(600.0f, 400.0f);

	m_pMouseStartButton->addEventListener(CLICK, [&]()-> void
		{
			m_pMouseStartButton->setActive(false);
			TheGame::Instance()->changeSceneState(PLAY_MOUSE_SCENE);
		});

	m_pMouseStartButton->addEventListener(MOUSE_OVER, [&]()->void
		{
			m_pMouseStartButton->setAlpha(128);
		});

	m_pMouseStartButton->addEventListener(MOUSE_OUT, [&]()->void
		{
			m_pMouseStartButton->setAlpha(255);
		});
	addChild(m_pMouseStartButton);


	
}

