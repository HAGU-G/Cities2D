#include "pch.h"
#include "GameManager.h"
#include "SceneGame.h" 
#include "SceneGameUI.h"
#include "SceneTitle.h"
#include <SceneMenu.h>

sf::RenderWindow GameManager::window;
sf::RenderWindow GameManager::debugWindow;
sf::VertexArray GameManager::fpsGraph(sf::LinesStrip, 300);

WINDOW_MODE GameManager::currentMode = WINDOW_MODE::WINDOW;
sf::Vector2f GameManager::defaultSize = { 1920, 1080 };
sf::Vector2u GameManager::currentSize = sf::Vector2u(GameManager::defaultSize);
sf::Vector2f GameManager::windowRatio = GameManager::defaultSize;

sf::Clock GameManager::globalClock;
float GameManager::globalTimeDelta = 0.f;
float GameManager::globalTimeScale = 1.f;
float GameManager::globalTimer = 0.f;

sf::Vector2i  GameManager::mousePosScreen;
sf::Vector2i  GameManager::mousePosWindow;

std::random_device* GameManager::rd = nullptr; // randomDevice
std::mt19937* GameManager::rg = nullptr; //randomGenerator


void GameManager::Init()
{
	/////////////////////////////
	// 
	//       윈도우
	// 
	/////////////////////////////
	//currentSize = {sf::VideoMode::getDesktopMode().width ,sf::VideoMode::getDesktopMode().height };
	currentSize = { 1920,1080 };
	windowRatio = sf::Vector2f(currentSize);

	sf::ContextSettings setting;
	setting.antialiasingLevel = 8;

	window.create(sf::VideoMode(currentSize.x, currentSize.y), "Cities2D", sf::Style::Close, setting);
	currentMode = WINDOW_MODE::WINDOW;
	SetWindowSize(1440);
	SetWindowPosition(sf::Vector2i((sf::VideoMode::getDesktopMode().width - currentSize.x) / 2,
		(sf::VideoMode::getDesktopMode().height - currentSize.y) / 2)); //스크린 중앙에 위치하도록
	//window.setFramerateLimit(60);
	window.display();

	/////////////////////////////
	//	
	//       디버그 윈도우
	// 
	/////////////////////////////
	debugWindow.create(sf::VideoMode(200, 400), "Cities2D : Debug", sf::Style::Close);
	debugWindow.setPosition(sf::Vector2i(window.getPosition().x - debugWindow.getSize().x, window.getPosition().y));
	debugWindow.display();
	window.requestFocus(); //메인 윈도우 포커싱
	for (int i = 0; i <= fpsGraph.getVertexCount() - 4; i++)
	{
		fpsGraph[i + 2].position = { 1.f + i * 198.f / (fpsGraph.getVertexCount() - 4) ,1.f };
	}



	/////////////////////////////
	// 
	//       씬
	// 
	/////////////////////////////
	AddScene();
	window.clear();
	SceneManager::Draw(window);
	window.display();

	/////////////////////////////
	// 
	//       초기화 완료 작업
	// 
	/////////////////////////////	
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
		//       이벤트 루프
		// 
		/////////////////////////////
		IOManager::Update(globalTimeDelta, globalTimeScale);
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				debugWindow.close();
				window.close();
			}
			IOManager::EventUpdate(event);
		}

		/////////////////////////////
		// 
		//       업데이트
		// 
		/////////////////////////////
		SceneManager::PreUpdate(globalTimeDelta);
		SceneManager::Update(globalTimeDelta);
		SceneManager::PostUpdate(globalTimeDelta);

		/////////////////////////////
		// 
		//       렌더링
		// 
		/////////////////////////////
		window.clear();
		SceneManager::Draw(window);
		window.display();

		/////////////////////////////
		// 
		//       디버그 윈도우
		// 
		/////////////////////////////
		DebugUpdate();
	}
}

void GameManager::Release()
{
	while (debugWindow.isOpen()) {}
	SceneManager::Release();
}

void GameManager::SetWindowSize(unsigned int x)
{
	currentSize.x = x;
	currentSize.y = x / windowRatio.x * windowRatio.y;
	window.setSize(currentSize);
}
void GameManager::SetWindowRatio(unsigned int x, unsigned int y)
{
	windowRatio.x = x;
	windowRatio.y = y;
	SetWindowSize(currentSize.x);
}
void GameManager::SetWindowPosition(sf::Vector2i position)
{
	window.setPosition(position);
}

void GameManager::AddScene()
{
	SceneManager::Add(std::make_shared<SceneTitle>("SceneTitle"));
	SceneManager::Add(std::make_shared<SceneGame>("SceneGame"));
	SceneManager::Add(std::make_shared<SceneGameUI>("SceneGameUI"));
	SceneManager::Add(std::make_shared<SceneMenu>("SceneMenu"));
	SceneManager::canChange = false;
	SceneManager::Use("SceneTitle", true);
	SceneManager::canChange = true;
}








/////////////////////////////
// 
//       디버그 윈도우
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

	//텍스트
	sf::Text text;
	int fontSize = 24;
	float graphRatio = fontSize / 165.f;
	float infoY = 0.f;
	text.setCharacterSize(fontSize);
	text.setFont(SFGM_FONT.Load("resource/font/ROKAF Sans Medium.ttf"));
	std::shared_ptr<SceneGame> sceneGame = std::dynamic_pointer_cast<SceneGame>(SceneManager::Get("SceneGame"));

	//렌더링
	debugWindow.clear();
	{
		fpsGraph[1].position = sf::Vector2f(1.f, fontSize);
		fpsGraph[1].color = sf::Color::Green;
		fpsGraph[0].position = sf::Vector2f(199.f, fontSize);
		fpsGraph[0].color = sf::Color::Green;
		fpsGraph[fpsGraph.getVertexCount() - 1].position = fpsGraph[0].position;
		fpsGraph[fpsGraph.getVertexCount() - 1].color = sf::Color::Green;
		fpsGraph[fpsGraph.getVertexCount() - 2].position = sf::Vector2f(199.f, fontSize - (1.f / globalTimeDelta) * graphRatio);
		if (fpsGraph[fpsGraph.getVertexCount() - 2].position.y >= fontSize - 30.f * graphRatio)
			fpsGraph[fpsGraph.getVertexCount() - 2].color = sf::Color::Red;
		else if (fpsGraph[fpsGraph.getVertexCount() - 2].position.y >= fontSize - 60.f * graphRatio)
			fpsGraph[fpsGraph.getVertexCount() - 2].color = sf::Color::Yellow;
		else
			fpsGraph[fpsGraph.getVertexCount() - 2].color = sf::Color::Green;
		debugWindow.draw(fpsGraph);
		for (int i = 0; i <= fpsGraph.getVertexCount() - 5; i++)
		{
			fpsGraph[i + 2].position = { 1.f + i * 198.f / (fpsGraph.getVertexCount() - 4) ,fpsGraph[i + 3].position.y };
			if (fpsGraph[i + 2].position.y >= fontSize - 30.f * graphRatio)
				fpsGraph[i + 2].color = sf::Color::Red;
			else if (fpsGraph[i + 2].position.y >= fontSize - 60.f * graphRatio)
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
		"[WorldPos]\n" + std::to_string(sceneGame->GetMousePosWorld().x) + "\n" + std::to_string(sceneGame->GetMousePosWorld().y)
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

void GameManager::Exit()
{
	debugWindow.close();
	window.close();
}

float GameManager::RandomRange(float a, float b)
{
	std::uniform_real_distribution<float> urf(std::min(a, b), std::max(a, b)); //uniformed random float
	return urf((*rg));
}

int GameManager::RandomRange(int a, int b)
{
	std::uniform_int_distribution<int> uri(std::min(a, b), std::max(a, b)); //uniformed random float
	return uri((*rg));
}

