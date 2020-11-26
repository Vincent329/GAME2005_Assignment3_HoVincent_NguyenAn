#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	TextureManager::Instance()->draw("background", 400.0f, 300.0f, 0, 255, true, SDL_FLIP_NONE);

	if(EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}

	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	updateDisplayList();
	if (isPlaying)
	{
		if (SDL_GetTicks() - bulletSpawnTimerStart >= bulletSpawnTimerDuration)
		{
			SpawnBullet();
		}
	}

	std::vector<Enemy*>& activeBullets = m_pPool->all;

	for (std::vector<Enemy*>::iterator myiter = activeBullets.begin(); myiter != activeBullets.end(); myiter++)
	{
		Enemy* bullet = *myiter;
		if (bullet->active && bullet->getTransform()->position.y >= 650) // if the bullet is alive and hits the bottom
		{
			m_pPool->despawn(bullet); // despawn the bullet
			break;
		}
		/*if (bullet->active && CollisionManager::circleAABBCheck(bullet, m_pPlayer))*/
		if (bullet->active && (CollisionManager::collisionCheck(bullet->getTransform()->position, m_pPlayer->getBoundingBoxTop()) || CollisionManager::collisionCheck(bullet->getTransform()->position, m_pPlayer->getBoundingBoxBottom())))
		{
			m_pPool->despawn(bullet); // despawn the bullet
			break;
		}
	}
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
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
				m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
				m_playerFacingRight = true;
			}
			else if (EventManager::Instance().getGameController(0)->LEFT_STICK_X < -deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
				m_playerFacingRight = false;
			}
			else
			{
				if (m_playerFacingRight)
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
				}
				else
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
				}
			}
		}
	}


	// handle player movement if no Game Controllers found
	if (SDL_NumJoysticks() < 1)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
			m_playerFacingRight = false;
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
			m_playerFacingRight = true;
		}
		else
		{
			if (m_playerFacingRight)
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
			}
			else
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
			}
		}
	}
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
	{
		m_pPlayer->moveLeft();
	}
	else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
	{
		m_pPlayer->moveRight();
	}
	else
	{
		m_pPlayer->stopMovingX();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_W)) { // separate these if stateme
		m_pPlayer->moveUp();
	}
	else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_S)) {
		m_pPlayer->moveDown();
	}
	else
	{
		m_pPlayer->stopMovingY();
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

void PlayScene::start()
{

	TextureManager::Instance()->load("../Assets/textures/space.jpg", "background");
	// Load a sound
	SoundManager::Instance().load("../Assets/audio/hitsound.ogg", "yay", SOUND_SFX);

	// Set GUI Title
	m_guiTitle = "Play Scene";
	
	// Plane Sprite
	//m_pPlaneSprite = new Plane();
	//addChild(m_pPlaneSprite);

	// Player Sprite
	m_pPlayer = new Player();
	addChild(m_pPlayer);

	m_Enemy = new Enemy();
	addChild(m_Enemy);

	// bullet pool creation
	m_pPool = new BulletPool(10);

	// add each one to the scene
	for (std::vector<Enemy*>::iterator myiter = m_pPool->all.begin(); myiter != m_pPool->all.end(); myiter++) // iterate through all bullets
	{
		Enemy* bullet = *myiter;
		addChild(bullet); // for each bullet, add to the scene
	}

	bulletSpawnTimerStart = SDL_GetTicks(); // the delta time
	

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
	m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas");
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 500.0f);

	addChild(m_pInstructionsLabel);
}

// BULLET SPAWNING
void PlayScene::SpawnBullet()
{
	Enemy* bullet = m_pPool->spawn(); // every half a second when the function is called, we spawn the bullet by setting values and pushing it onto the queue
		if (bullet)
		{
			bullet->getTransform()->position = glm::vec2(50 + rand() % 700, -100 + rand() % 100 );
		}
		bulletSpawnTimerStart = SDL_GetTicks();
}

void PlayScene::GUI_Function()
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);
	if (ImGui::Button("Play Scene"))
	{
		isPlaying = (isPlaying) ? false : true;
		std::cout << isPlaying << std::endl;
	}

	if (ImGui::SliderFloat("Bullet Initial Velocity", &initialBulletVelocity, 1.0f, 500.0f, "%.1f"))
	{
		std::vector<Enemy*>& activeBullets = m_pPool->all;
		for (std::vector<Enemy*>::iterator myiter = activeBullets.begin(); myiter != activeBullets.end(); myiter++)
		{
			Enemy* bullet = *myiter;
			bullet->initialVelocity = initialBulletVelocity;
		}
	}

	if (ImGui::SliderFloat("Bullet Gravity", &gravityFactor, 1.0f, 50.0f, "%.1f"))
	{
		std::vector<Enemy*>& activeBullets = m_pPool->all;
		for (std::vector<Enemy*>::iterator myiter = activeBullets.begin(); myiter != activeBullets.end(); myiter++)
		{
			Enemy* bullet = *myiter;
			bullet->gravity = gravityFactor;
		}
	}

	if (ImGui::SliderFloat("Player Acceleration", &acceleration, 0.0f, 50.0f, "%.1f"))
	{
		m_pPlayer->setAcceleration(acceleration);
	}

	ImGui::SliderFloat("Spawn Rate", &bulletSpawnTimerDuration, 100.0f, 1000.0f, "%.1f");

	if (ImGui::Button("Show Ship Hitbox"))
	{
		if (m_pPlayer->getShowHitbox() == true)
			m_pPlayer->setShowHitbox(false);
		else
			m_pPlayer->setShowHitbox(true);
	}
	if (ImGui::Button("Show Bullet Hitbox"))
	{
		std::vector<Enemy*>& activeBullets = m_pPool->all;
		for (std::vector<Enemy*>::iterator myiter = activeBullets.begin(); myiter != activeBullets.end(); myiter++)
		{
			Enemy* bullet = *myiter;
			bullet->showHitbox = (bullet->showHitbox) ? false : true;
		}
	}
	
	ImGui::Separator();
	
	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}
