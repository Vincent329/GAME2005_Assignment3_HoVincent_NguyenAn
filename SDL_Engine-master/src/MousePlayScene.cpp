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

void MousePlayScene::draw()
{
	if (EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}

	drawDisplayList();
	Util::DrawCircle(m_pBall->getTransform()->position, std::max(m_pBall->getWidth() * 0.5f, m_pBall->getHeight() * 0.5f));
	Util::DrawRect(m_pMousePlayer->getTransform()->position - glm::vec2(m_pMousePlayer->getWidth()*0.5, m_pMousePlayer->getHeight() *.5f), m_pMousePlayer->getWidth(), m_pMousePlayer->getHeight());

	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

static float normalX = 0.0f, normalY = 0.0f;

void MousePlayScene::update()
{
	updateDisplayList();
	SDL_GetMouseState(&xMouse, &yMouse);
	m_pMousePlayer->mouseMovement(xMouse, yMouse);
	std::cout << "Velocity X: " << m_pMousePlayer->getVelocityX() << std::endl;

	CollisionManager::circleAABBCheck(m_pBall, m_pMousePlayer); // figure out velocity response
	//float collisionTime = CollisionManager::sweptAABB(m_pBall, m_pMousePlayer, normalX, normalY);
	//CollisionManager::AABBCheck(m_pBall, m_pPlayer);
	//std::cout << collisionTime << std::endl;
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
	// Load a sound
	SoundManager::Instance().load("../Assets/audio/yay.ogg", "yay", SOUND_SFX);
	
	// Set GUI Title
		m_guiTitle = "Play Scene";

	// Plane Sprite
	m_pPlaneSprite = new Plane();
	addChild(m_pPlaneSprite);

	// Player Sprite
	m_pMousePlayer = new MousePlayer();
	addChild(m_pMousePlayer);

	// Instantiate the target
	m_pBall = new Target();
	addChild(m_pBall);

	m_playerFacingRight = true;

	// Back Button
	m_pBackButton = new Button("../Assets/textures/backButton.png", "backButton", BACK_BUTTON);
	m_pBackButton->getTransform()->position = glm::vec2(300.0f, 400.0f);
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
	m_pNextButton->getTransform()->position = glm::vec2(500.0f, 400.0f);
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
	m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas");
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 500.0f);

	addChild(m_pInstructionsLabel);
}

void MousePlayScene::GUI_Function() const
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();

	ImGui::Begin("Your Window Title Goes Here", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	if (ImGui::Button("My Button"))
	{
		std::cout << "My Button Pressed" << std::endl;
	}

	// Play Button
	if (ImGui::Button("Play"))
	{

	}

	// Reset Button
	if (ImGui::Button("Reset"))
	{

	}
	ImGui::Separator();
	// Shape Buttons
	ImGui::Text("Shape Selection");
	if (ImGui::Button("Circle"))
	{

	}
	ImGui::SameLine();
	if (ImGui::Button("Square"))
	{

	}
	if (ImGui::Button("Triangle"))
	{

	}
	// --------------------Parameter changes-------------------------
	static float m_massPlayer = 5.0f;
	static float m_massBall = 2.5f;

	if (ImGui::SliderFloat("Player Mass", &m_massPlayer, 0.0f, 10.0f, "%.1f"))
	{
		m_pMousePlayer->setMass(m_massPlayer);
		std::cout << "Mass Player: " << m_pMousePlayer->getMass() << std::endl;
		if (m_massBall > m_massPlayer)
		{
			m_massBall = m_massPlayer;
		}

	}

	if (ImGui::SliderFloat("Ball Mass", &m_massBall, 0.0f, 10.0f))
	{
		
		m_pBall->setMass(m_massBall);
		std::cout << "Mass Ball: " << m_pBall->getMass() << std::endl;
		if (m_massBall > m_massPlayer)
		{
			m_massPlayer = m_massBall;
		}
	}

	ImGui::Separator();

	static float float3[3] = { 0.0f, 1.0f, 1.5f };
	if (ImGui::SliderFloat3("My Slider", float3, 0.0f, 2.0f))
	{
		std::cout << float3[0] << std::endl;
		std::cout << float3[1] << std::endl;
		std::cout << float3[2] << std::endl;
		std::cout << "---------------------------\n";
	}

	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}
