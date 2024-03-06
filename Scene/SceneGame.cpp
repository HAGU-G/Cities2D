#include "pch.h"
#include "SceneGame.h"
#include "Test/ObjectTest.h"
#include "ObjectTile.h"
#include "ObjectTileMap.h"

SceneGame::SceneGame(const std::string& name)
	:Scene(name)
{
}

SceneGame::~SceneGame()
{
}

void SceneGame::Init()
{
	//선택된 타일 표시용 (임시)
	AddObject(std::make_shared<ObjectTest>(This(), GAME_OBJECT_TYPE::BUILDING));

	//바닥을 그려줄 타일 맵
	groundTileMap = ObjectTileMap::Create(This());

	Scene::Init();
	Reset();
}

void SceneGame::PreUpdate(float timeDelta, float timeScale)
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
			DeleteObjectTile(mousePosGrid);
	}

}

void SceneGame::Update(float timeDelta, float timeScale)
{
	Scene::Update(timeDelta, timeScale);
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

bool SceneGame::CreateObjectTile(GAME_OBJECT_TYPE type, const sf::Vector2i& gridCoord)
{
	if (type != GAME_OBJECT_TYPE::NONE && gridInfo[gridCoord.x][gridCoord.y].first == GAME_OBJECT_TYPE::NONE)
	{
		gridInfo[gridCoord.x][gridCoord.y].first = type;
		gridInfo[gridCoord.x][gridCoord.y].second = ObjectTile::Create(This(), type, gridCoord);
		groundTileMap->UpdateTile(gridCoord);
		TileUpdate(); //사용하지 않을 것으로 예상됨.
		return true;
	}
	else
	{
		return false;
	}
}

bool SceneGame::DeleteObject(const std::string& key)
{
	bool ret = Scene::DeleteObject(key);
	TileUpdate();
	return ret;
}

void SceneGame::DeleteObjectTile(const sf::Vector2i& gridCoord)
{
	DeleteObject(gridInfo[gridCoord.x][gridCoord.y].second->GetKey());
	gridInfo[gridCoord.x][gridCoord.y].first = GAME_OBJECT_TYPE::NONE;
	gridInfo[gridCoord.x][gridCoord.y].second.reset();

	groundTileMap->UpdateTile(gridCoord);
}

void SceneGame::TileUpdate()
{
	
}

void SceneGame::SetMousePosGrid()
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