#include "MousePlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"

MousePlayScene::MousePlayScene()
{
	MousePlayScene::start();
}

MousePlayScene::~MousePlayScene()
= default;

static bool isPlaying = true;

void MousePlayScene::draw()
{
	// draw the background
	TextureManager::Instance()->draw("background", 400.0f, 300.0f, 0, 255, true, SDL_FLIP_NONE);
	if (EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
		if (isPlaying)
		{
			isPlaying = !isPlaying;
			m_pBall->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		}
	}
	else {
		isPlaying = true;
	}

	drawDisplayList();
	
	
	m_playerMass->setText("Player Mass: " + std::to_string(m_pMousePlayer->getMass()));
	m_ballMass->setText("Ball Mass: " + std::to_string(m_pBall->getMass()));
	m_playerVelocity->setText("Player Velocity X:" + std::to_string(m_pMousePlayer->getVelocityX() * m_pMousePlayer->getPPM()) + " Y: " + std::to_string(m_pMousePlayer->getVelocityY() * m_pMousePlayer->getPPM()));
	m_ballVelocity->setText("Ball Velocity X:" + std::to_string(m_pBall->getInitialVelocityX() * m_pMousePlayer->getPPM()) + " Y: " + std::to_string(m_pBall->getInitialVelocityX() * m_pMousePlayer->getPPM()));
	m_coefficient->setText("Wall Friction Coefficient: " + std::to_string(m_pBall->getWallCoefficient()));
	m_PixelsPerMeter->setText("Pixels/Meter: " + std::to_string(m_pMousePlayer->getPPM()));

	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

static float normalX = 0.0f, normalY = 0.0f;

void MousePlayScene::update()
{
	updateDisplayList();
	if (isPlaying)
	{
		SDL_GetMouseState(&xMouse, &yMouse);
		m_pMousePlayer->mouseMovement(xMouse, yMouse);

		if (m_pBall->getCollisionType() == CIRCLE)
		{
			CollisionManager::circleAABBCheck(m_pBall, m_pMousePlayer);
		}
		else if (m_pBall->getCollisionType() == RECTANGLE)
		{
			CollisionManager::AABBCheck(m_pBall, m_pMousePlayer);
		}
	}
	SDL_ShowCursor(1);
	// figure out velocity response
}

void MousePlayScene::clean()
{
	removeAllChildren();
}

void MousePlayScene::handleEvents()
{
	EventManager::Instance().update();

	// handle player movement with GameController
	if (SDL_NumJoysticks() > 0)
	{
		if (EventManager::Instance().getGameController(0) != nullptr)
		{
			const auto deadZone = 10000;
			if (EventManager::Instance().getGameController(0)->LEFT_STICK_X > deadZone)
			{
				m_pMousePlayer->setAnimationState(PLAYER_RUN_RIGHT);
				m_playerFacingRight = true;
			}
			else if (EventManager::Instance().getGameController(0)->LEFT_STICK_X < -deadZone)
			{
				m_pMousePlayer->setAnimationState(PLAYER_RUN_LEFT);
				m_playerFacingRight = false;
			}
			else
			{
				if (m_playerFacingRight)
				{
					m_pMousePlayer->setAnimationState(PLAYER_IDLE_RIGHT);
				}
				else
				{
					m_pMousePlayer->setAnimationState(PLAYER_IDLE_LEFT);
				}
			}
		}
	}

	// handle player movement if no Game Controllers found
	if (SDL_NumJoysticks() < 1)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
		{
			m_pMousePlayer->setAnimationState(PLAYER_RUN_LEFT);
			m_playerFacingRight = false;
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{
			m_pMousePlayer->setAnimationState(PLAYER_RUN_RIGHT);
			m_playerFacingRight = true;
		}
		else
		{
			if (m_playerFacingRight)
			{
				m_pMousePlayer->setAnimationState(PLAYER_IDLE_RIGHT);
			}
			else
			{
				m_pMousePlayer->setAnimationState(PLAYER_IDLE_LEFT);
			}
		}
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}
}

void MousePlayScene::start()
{
	TextureManager::Instance()->load("../Assets/textures/background.jpg", "background");

	// Load a sound
	SoundManager::Instance().load("../Assets/audio/hitsound.ogg", "yay", SOUND_SFX);
	
	const SDL_Color white = { 1,1,1,1 };
	const SDL_Color blue = { 0, 0, 255, 255 };
	const SDL_Color black = { 0, 0, 0 , 255 };
	// Set GUI Title
		m_guiTitle = "Play Scene";

	//// Plane Sprite
	//m_pPlaneSprite = new Plane();
	//addChild(m_pPlaneSprite);

	// Player Sprite
	m_pMousePlayer = new MousePlayer();
	addChild(m_pMousePlayer);

	// Instantiate the target
	m_pBall = new Target();
	m_pBall->getTransform()->position = glm::vec2(300.0f, 300.0f);
	addChild(m_pBall);

	m_playerFacingRight = true;

	// Back Button
	m_pBackButton = new Button("../Assets/textures/backButton.png", "backButton", BACK_BUTTON);
	m_pBackButton->getTransform()->position = glm::vec2(100.0f, 550.0f);
	m_pBackButton->addEventListener(CLICK, [&]()-> void
		{
			m_pBackButton->setActive(false);
			TheGame::Instance()->changeSceneState(START_SCENE);
		});

	m_pBackButton->addEventListener(MOUSE_OVER, [&]()->void
		{
			m_pBackButton->setAlpha(128);
		});

	m_pBackButton->addEventListener(MOUSE_OUT, [&]()->void
		{
			m_pBackButton->setAlpha(255);
		});
	addChild(m_pBackButton);

	// Next Button
	m_pNextButton = new Button("../Assets/textures/nextButton.png", "nextButton", NEXT_BUTTON);
	m_pNextButton->getTransform()->position = glm::vec2(700.0f, 550.0f);
	m_pNextButton->addEventListener(CLICK, [&]()-> void
		{
			m_pNextButton->setActive(false);
			TheGame::Instance()->changeSceneState(END_SCENE);
		});

	m_pNextButton->addEventListener(MOUSE_OVER, [&]()->void
		{
			m_pNextButton->setAlpha(128);
		});

	m_pNextButton->addEventListener(MOUSE_OUT, [&]()->void
		{
			m_pNextButton->setAlpha(255);
		});

	addChild(m_pNextButton);

	/* Instructions Label */
	m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Pause and Alter Sim", "Consolas");
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 500.0f);
	addChild(m_pInstructionsLabel);

	m_playerMass = new Label(" ", "Consolas", 15, black, glm::vec2(650.0f, 10.0f));
	addChild(m_playerMass);

	m_ballMass = new Label(" ", "Consolas", 15, black, glm::vec2(650.0f, 30.0f));
	addChild(m_ballMass);

	m_playerVelocity = new Label(" ", "Consolas", 15, black, glm::vec2(650.0f, 50.0f));
	addChild(m_playerVelocity);

	m_ballVelocity = new Label(" ", "Consolas", 15, black, glm::vec2(650.0f, 70.0f));
	addChild(m_ballVelocity);

	m_coefficient = new Label(" ", "Consolas", 15, black, glm::vec2(650.0f, 90.0f));
	addChild(m_coefficient);

	m_PixelsPerMeter = new Label(" ", "Consolas", 15, black, glm::vec2(650.0f, 110.0f));
	addChild(m_PixelsPerMeter);
}

bool MousePlayScene::getIsPlaying()
{
	return isPlaying;
}

void MousePlayScene::setIsPlaying(bool _play)
{
	isPlaying = _play;
}

void MousePlayScene::GUI_Function() const
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();

	ImGui::Begin("Physics Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	// Reset Button
	if (ImGui::Button("Reset"))
	{
		m_pBall->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		m_pBall->getTransform()->position = glm::vec2(400.0f, 300.0f);
	}

	ImGui::Separator();
	// Shape Buttons
	
	ImGui::Text("Shape Selection");
	if (ImGui::Button("Circle"))
	{
		m_pBall->setCollisionType(CIRCLE);
	}
	ImGui::SameLine();
	if (ImGui::Button("Square"))
	{
		m_pBall->setCollisionType(RECTANGLE);
	}
	
	if (ImGui::Button("Flip Paddle"))
	{
		m_pMousePlayer->flipPaddle();
	}

	if (ImGui::Button("Toggle Player Hitbox")) {
		if (m_pMousePlayer->getShowHitbox() == false)
			m_pMousePlayer->setShowHitbox(true);
		else
			m_pMousePlayer->setShowHitbox(false);
	}
	if (ImGui::Button("Toggle Ball Hitbox")) {
		if (m_pBall->getShowHitbox() == false)
			m_pBall->setShowHitbox(true);
		else
			m_pBall->setShowHitbox(false);
	}
	// --------------------Parameter changes-------------------------
	static float m_massPlayer = 5.0f;
	static float m_massBall = 2.5f;
	static float m_wallCoefficient = 0.9;
	static float m_pixels = 2.0f;

	if (ImGui::SliderFloat("Pixels Per Meter", &m_pixels, 1.0f, 10.0f, "%.1f"))	{	
		m_pMousePlayer->setPPM(m_pixels);
		std::cout << "Pixels: " << m_pMousePlayer->getPPM() << std::endl;
	}

	if (ImGui::SliderFloat("Player Mass", &m_massPlayer, 1.0f, 10.0f, "%.1f"))
	{
		m_pMousePlayer->setMass(m_massPlayer);
		std::cout << "Mass Player: " << m_pMousePlayer->getMass() << std::endl;
		if (m_massBall > m_massPlayer)
		{
			m_massBall = m_massPlayer;
		}

	}

	if (ImGui::SliderFloat("Ball Mass", &m_massBall, 1.0f, 10.0f, "%.1f"))
	{
		
		m_pBall->setMass(m_massBall);
		std::cout << "Mass Ball: " << m_pBall->getMass() << std::endl;
		if (m_massBall > m_massPlayer)
		{
			m_massPlayer = m_massBall;
		}
	}

	if (ImGui::SliderFloat("Wall Coefficient of Momentum", &m_wallCoefficient, 0.0f, 1.0f))
	{
		m_pBall->setWallCoefficient(m_wallCoefficient);
	}

	ImGui::Separator();


	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}
