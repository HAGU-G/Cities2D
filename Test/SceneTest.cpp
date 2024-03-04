#include "pch.h"
#include "SceneTest.h"
#include "ObjectTest.h"

SceneTest::SceneTest(const std::string& name)
	:Scene(name)
{
}

SceneTest::~SceneTest()
{
}

void SceneTest::Init()
{
	AddObject(std::make_shared<ObjectTest>(This(), GAME_OBJECT_TYPE::BUILDING));

	Scene::Init();
}

void SceneTest::PreUpdate(float timeDelta, float timeScale)
{
	if (IOManager::IsKeyPress(sf::Keyboard::Q))
	{
		view.rotate(45.f * timeDelta);
	}
	if (IOManager::IsKeyPress(sf::Keyboard::E))
	{
		view.rotate(-45.f * timeDelta);
	}
	if (IOManager::IsKeyPress(sf::Keyboard::W))
	{
		view.move(sf::Transform().rotate(view.getRotation()) * sf::Vector2f(0.f, -200.f * timeDelta));
	}
	if (IOManager::IsKeyPress(sf::Keyboard::S))
	{
		view.move(sf::Transform().rotate(view.getRotation()) * sf::Vector2f(0.f, 200.f * timeDelta));
	}
	if (IOManager::IsKeyPress(sf::Keyboard::A))
	{
		view.move(sf::Transform().rotate(view.getRotation()) * sf::Vector2f(-200.f * timeDelta, 0.f));
	}
	if (IOManager::IsKeyPress(sf::Keyboard::D))
	{
		view.move(sf::Transform().rotate(view.getRotation()) * sf::Vector2f(200.f * timeDelta, 0.f));
	}


	Scene::PreUpdate(timeDelta, timeScale);
	SetMousePosGrid();
}

void SceneTest::Update(float timeDelta, float timeScale)
{
	Scene::Update(timeDelta, timeScale);

	std::cout << "Window: " << GameManager::GetMousePosWindow().x << " " << GameManager::GetMousePosWindow().y << std::endl;
	std::cout << "World: " << mousePosWorld.x << " " << mousePosWorld.y << std::endl;
	std::cout << "Greed: " << GetMousePosGrid().x << " " << GetMousePosGrid().y << std::endl;



}

void SceneTest::Draw(sf::RenderWindow& window)
{

	test.setSize({ gridSize * 5.f, gridSize * 5.f });
	test.setFillColor(sf::Color::White);

	window.draw(test);
	Scene::Draw(window);
}

void SceneTest::SetMousePosGrid()
{
	if (mousePosWorld.x >= 0)
		mousePosGrid.x = mousePosWorld.x / gridSize;
	else
		mousePosGrid.x = floor(mousePosWorld.x / gridSize);

	if (mousePosWorld.y >= 0)
		mousePosGrid.y = mousePosWorld.y / gridSize;
	else
		mousePosGrid.y = floor(mousePosWorld.y / gridSize);
}