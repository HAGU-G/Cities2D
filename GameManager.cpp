#include "pch.h"
#include "GameManager.h"

//TESTCODE : ������� ����
#include "Test/SceneTest.h" 

unsigned int GameManager::titleY = 40;
sf::RenderWindow GameManager::window;
WINDOW_MODE GameManager::currentMode = WINDOW_MODE::WINDOW;
sf::Vector2u GameManager::currentSize;
unsigned int GameManager::ratioX = 16;
unsigned int GameManager::ratioY = 9;

sf::Clock GameManager::globalClock;
float GameManager::globalTimeDelta = 0.f;
float GameManager::globalTimeScale = 1.f;
float GameManager::globalTimer = 0.f;

sf::Vector2i  GameManager::mousePosScreen;
sf::Vector2i  GameManager::mousePosWindow;

void GameManager::Init()
{
	/////////////////////////////
	// 
	//       ������
	// 
	/////////////////////////////
	currentSize = { 1920, 1080 };
	ratioX = 16;
	ratioY = 9;

	window.create(sf::VideoMode(currentSize.x, currentSize.y), "Cities2D", sf::Style::Close);
	SetWindowSize(1280);
	SetWindowPosition(sf::Vector2i((sf::VideoMode::getDesktopMode().width - currentSize.x) / 2,
		(sf::VideoMode::getDesktopMode().height - currentSize.y) / 2)); //��ũ�� �߾ӿ� ��ġ�ϵ���

	/////////////////////////////
	// 
	//       �� & ���ҽ� ����
	// 
	/////////////////////////////
	AddScene();
	
	/////////////////////////////
	// 
	//       �ʱ�ȭ �Ϸ� �۾�
	// 
	/////////////////////////////
	SceneManager::Init();
	globalClock.restart();
}

void GameManager::MainLoop()
{
	while (window.isOpen())
	{
		mousePosWindow = sf::Mouse::getPosition(window);

		globalTimeDelta = globalClock.restart().asSeconds();
		globalTimer += globalTimeDelta;

		/////////////////////////////
		// 
		//       �̺�Ʈ ����
		// 
		/////////////////////////////
		IOManager::Update(globalTimeDelta, globalTimeScale);
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			IOManager::EventUpdate(event);
		}

		/////////////////////////////
		// 
		//       ������Ʈ
		// 
		/////////////////////////////
		SceneManager::PreUpdate(globalTimeDelta);
		SceneManager::Update(globalTimeDelta);

		/////////////////////////////
		// 
		//       ������
		// 
		/////////////////////////////
		window.clear();
		SceneManager::Draw(window);
		window.display();
	}
}

void GameManager::Release()
{
	SceneManager::Release();
}

void GameManager::SetWindowSize(unsigned int x)
{
	currentSize.x = x;
	currentSize.y = x / ratioX * ratioY;
	window.setSize(currentSize);
}
void GameManager::SetWindowRatio(unsigned int x, unsigned int y)
{
	ratioX = x;
	ratioY = y;
	SetWindowSize(currentSize.x);
}
void GameManager::SetWindowPosition(sf::Vector2i position)
{
	window.setPosition(position);
}

void GameManager::AddScene()
{
	SceneManager::AddUse(std::make_shared<SceneTest>("Test"));
}

