#include "pch.h"
#include "GameManager.h"
#include "SceneGame.h" 
#include <SceneGameUI.h>

sf::RenderWindow GameManager::debugWindow;
sf::VertexArray GameManager::fpsGraph = sf::VertexArray(sf::LinesStrip, 300);
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
	setting.antialiasingLevel = 8;

	window.create(sf::VideoMode(currentSize.x, currentSize.y), "Cities2D", sf::Style::Close, setting);
	SetWindowSize(1280);
	SetWindowPosition(sf::Vector2i((sf::VideoMode::getDesktopMode().width - currentSize.x) / 2,
		(sf::VideoMode::getDesktopMode().height - currentSize.y) / 2)); //��ũ�� �߾ӿ� ��ġ�ϵ���
	//window.setFramerateLimit(60);

	/////////////////////////////
	// 
	//       ����� ������
	// 
	/////////////////////////////
	debugWindow.create(sf::VideoMode(200.f, 400.f), "Cities2D : Debug", sf::Style::Close);
	debugWindow.setPosition(sf::Vector2i(window.getPosition().x - debugWindow.getSize().x, window.getPosition().y));
	window.requestFocus(); //���� ������ ��Ŀ��
	for (int i = 0; i <= fpsGraph.getVertexCount() - 4; i++)
	{
		fpsGraph[i + 2].position = { 1.f + i * 198.f / (fpsGraph.getVertexCount() - 4) ,30.f };
	}

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
			{
				window.close();
				debugWindow.close();
			}
			IOManager::EventUpdate(event);
		}

		/////////////////////////////
		// 
		//       ������Ʈ
		// 
		/////////////////////////////
		SceneManager::PreUpdate(globalTimeDelta);
		SceneManager::Update(globalTimeDelta);
		SceneManager::PostUpdate(globalTimeDelta);

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
	SceneManager::AddUse(std::make_shared<SceneGame>("SceneGame"));
	SceneManager::AddUse(std::make_shared<SceneGameUI>("SceneGameUI"));
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
	int fontSize = 30;
	float infoY = 0.f;
	text.setCharacterSize(fontSize);
	text.setFont(SFGM_FONT.Get("BMHANNAPro.ttf"));
	std::shared_ptr<SceneGame> sceneGame = std::dynamic_pointer_cast<SceneGame>(SceneManager::Get("SceneGame"));

	//������
	debugWindow.clear();
	{
		fpsGraph[1].position = sf::Vector2f(1.f, fontSize);
		fpsGraph[1].color = sf::Color::Green;
		fpsGraph[0].position = sf::Vector2f(199.f, fontSize);
		fpsGraph[0].color = sf::Color::Green;
		fpsGraph[fpsGraph.getVertexCount() - 1].position = fpsGraph[0].position;
		fpsGraph[fpsGraph.getVertexCount() - 1].color = sf::Color::Green;
		fpsGraph[fpsGraph.getVertexCount() - 2].position = sf::Vector2f(199.f, fontSize - (1.f / globalTimeDelta) * 0.15f);
		if (fpsGraph[fpsGraph.getVertexCount() - 2].position.y >= fontSize - 30.f * 0.15f)
			fpsGraph[fpsGraph.getVertexCount() - 2].color = sf::Color::Red;
		else if (fpsGraph[fpsGraph.getVertexCount() - 2].position.y >= fontSize - 60.f * 0.15f)
			fpsGraph[fpsGraph.getVertexCount() - 2].color = sf::Color::Yellow;
		else
			fpsGraph[fpsGraph.getVertexCount() - 2].color = sf::Color::Green;
		debugWindow.draw(fpsGraph);
		for (int i = 0; i <= fpsGraph.getVertexCount() - 5; i++)
		{
			fpsGraph[i + 2].position = { 1.f + i * 198.f / (fpsGraph.getVertexCount() - 4) ,fpsGraph[i + 3].position.y };
			if (fpsGraph[i + 2].position.y >= fontSize - 30.f * 0.15f)
				fpsGraph[i + 2].color = sf::Color::Red;
			else if (fpsGraph[i + 2].position.y >= fontSize - 60.f * 0.15f)
				fpsGraph[i + 2].color = sf::Color::Yellow;
			else
				fpsGraph[i + 2].color = sf::Color::Green;
		}

	}
	infoY += fontSize * 1;
	text.setPosition(0.f, infoY);

	text.setFillColor(sf::Color::Green);
	text.setString(std::to_string(1 / globalTimeDelta));
	debugWindow.draw(text);
	infoY += fontSize * 1;
	text.setPosition(0.f, infoY);

	text.setFillColor(sf::Color::White);
	text.setString(
		"[WolrdPos]\n" + std::to_string(sceneGame->GetMousePosWorld().x) + "\n" + std::to_string(sceneGame->GetMousePosWorld().y)
	);
	debugWindow.draw(text);
	infoY += fontSize * 4;
	text.setPosition(0.f, infoY);

	text.setString(
		"[GridPos]\n" + std::to_string(sceneGame->GetMousePosGrid().x) + "\n" + std::to_string(sceneGame->GetMousePosGrid().y)
	);
	debugWindow.draw(text);
	infoY += fontSize * 4;
	text.setPosition(0.f, infoY);

	text.setString(
		"[ObjectCount]\n" + std::to_string(GameObject::GetObjectsTotalCount()) + "\n" + std::to_string(GameObject::GetObjectsCount())
	);
	debugWindow.draw(text);
	infoY += fontSize * 4;
	text.setPosition(0.f, infoY);

	debugWindow.display();
}

