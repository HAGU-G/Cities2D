#include "pch.h"
#include "SceneTest.h"
#include "Test/ObjectTest.h"
#include "ObjectTile.h"

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
	Scene::PreUpdate(timeDelta, timeScale);
	SetMousePosGrid();

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


	if (IOManager::IsKeyDown(sf::Keyboard::Space))
	{
		if (gridInfo[mousePosGrid.x][mousePosGrid.y].second == nullptr)
			CreateObjectTile(GAME_OBJECT_TYPE::BUILDING, mousePosGrid);
		else
			DeleteObject(mousePosGrid);
	}

}

void SceneTest::Update(float timeDelta, float timeScale)
{
	Scene::Update(timeDelta, timeScale);
}

void SceneTest::Draw(sf::RenderWindow& window)
{
	test.setSize(gridSize * 5.f);
	test.setFillColor(sf::Color::White);

	window.draw(test);
	Scene::Draw(window);
}

bool SceneTest::CreateObjectTile(GAME_OBJECT_TYPE type, const sf::Vector2i& gridCoord)
{
	if (type != GAME_OBJECT_TYPE::NONE && gridInfo[gridCoord.x][gridCoord.y].first == GAME_OBJECT_TYPE::NONE)
	{
		gridInfo[gridCoord.x][gridCoord.y].first = type;
		gridInfo[gridCoord.x][gridCoord.y].second = ObjectTile::Create(This(), type, gridCoord);
		TileUpdate();
		return true;
	}
	else
	{
		return false;
	}
}

bool SceneTest::DeleteObject(const std::string& key)
{
	bool ret = Scene::DeleteObject(key);
	TileUpdate();
	return ret;
}

void SceneTest::DeleteObject(const sf::Vector2i& gridCoord)
{
	DeleteObject(gridInfo[gridCoord.x][gridCoord.y].second->GetKey());
	gridInfo[gridCoord.x][gridCoord.y].first = GAME_OBJECT_TYPE::NONE;
	gridInfo[gridCoord.x][gridCoord.y].second.reset();
}

void SceneTest::TileUpdate()
{

}

void SceneTest::SetMousePosGrid()
{
	if (mousePosWorld.x >= 0)
		mousePosGrid.x = mousePosWorld.x / gridSize.x;
	else
		mousePosGrid.x = floor(mousePosWorld.x / gridSize.x);

	if (mousePosWorld.y >= 0)
		mousePosGrid.y = mousePosWorld.y / gridSize.y;
	else
		mousePosGrid.y = floor(mousePosWorld.y / gridSize.y);
}