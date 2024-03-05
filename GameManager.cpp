#include "pch.h"
#include "GameManager.h"

//TESTCODE : ������� ����
#include "Test/SceneTest.h" 

sf::RenderWindow GameManager::debugWindow;
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

	sf::ContextSettings setting;
	setting.antialiasingLevel = 0;

	window.create(sf::VideoMode(currentSize.x, currentSize.y), "Cities2D", sf::Style::Close, setting);
	SetWindowSize(1280);
	SetWindowPosition(sf::Vector2i((sf::VideoMode::getDesktopMode().width - currentSize.x) / 2,
		(sf::VideoMode::getDesktopMode().height - currentSize.y) / 2)); //��ũ�� �߾ӿ� ��ġ�ϵ���

	debugWindow.create(sf::VideoMode(200.f, 400.f), "Cities2D : Debug", sf::Style::Close);
	debugWindow.setPosition(sf::Vector2i(window.getPosition().x - debugWindow.getSize().x, window.getPosition().y));
	
	window.requestFocus();

	/////////////////////////////
	// 
	//       �� & ���ҽ� ����
	// 
	/////////////////////////////
	AddScene();
	SFGM_FONT.Load("BMHANNAPro.ttf");
	
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

		/////////////////////////////
		// 
		//       ����� ������
		// 
		/////////////////////////////
		DebugUpdate();
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








/////////////////////////////
// 
//       ����� ������
// 
/////////////////////////////
void GameManager::DebugUpdate()
{
	sf::Event event;
	while (debugWindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			debugWindow.close();
	}

	//�ؽ�Ʈ
	sf::Text text;
	text.setCharacterSize(20);
	text.setFillColor(sf::Color::White);
	text.setFont(SFGM_FONT.Get("BMHANNAPro.ttf"));
	std::shared_ptr<SceneTest> sceneTest = std::dynamic_pointer_cast<SceneTest>(SceneManager::Get("Test"));


	debugWindow.clear();

	text.setString(
		"[WolrdPos]\n" + std::to_string(sceneTest->GetMousePosWolrd().x) + "\n" + std::to_string(sceneTest->GetMousePosWolrd().y)
	);
	debugWindow.draw(text);

	text.setPosition(0.f, 60.f);
	text.setString(
		"\n[GridPos]\n" + std::to_string(sceneTest->GetMousePosGrid().x) + "\n" + std::to_string(sceneTest->GetMousePosGrid().y)
	);
	debugWindow.draw(text);

	text.setPosition(0.f, 140.f);
	text.setString(
		"\n[ObjectCount]\n" + std::to_string(GameObject::GetObjectsTotalCount()) + "\n" + std::to_string(GameObject::GetObjectsCount())
	);
	debugWindow.draw(text);

	debugWindow.display();
}

