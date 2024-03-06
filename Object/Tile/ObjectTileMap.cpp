#include "pch.h"
#include "ObjectTileMap.h"
#include "ObjectTile.h"
#include "SceneGame.h"

ObjectTileMap::ObjectTileMap(std::weak_ptr<Scene> scene, GAME_OBJECT_TYPE objectType)
	:GameObject(scene, objectType)
{
}

ObjectTileMap::~ObjectTileMap()
{
	Release();
}

std::shared_ptr<ObjectTileMap> ObjectTileMap::Create(std::weak_ptr<Scene> scene)
{
	std::shared_ptr<ObjectTileMap> objectTileMap = std::make_shared<ObjectTileMap>(scene);
	objectTileMap->Init();
	scene.lock()->AddObject(objectTileMap);
	return objectTileMap;
}

void ObjectTileMap::UpdateTile(int x, int y)
{
	//color를 texCoord로 변환할 필요가 있음.
	// 
	//타일이 비어있을 때
	sf::Color color;

	switch (sceneGame.lock()->GetTileInfo(x, y).first)
	{
	case GAME_OBJECT_TYPE::GROUND:
	case GAME_OBJECT_TYPE::NONE:
		ResetTile(x, y);
		return;
	case GAME_OBJECT_TYPE::ROAD:
		color = {100,100,100,255};
		break;
	case GAME_OBJECT_TYPE::BUILDING:
		color = { 255,255,0,255 };
		break;
	default:
		break;
	}

	//타일에 무언가 있을 때
	sf::Vector2f gridSize = sceneGame.lock()->GetGridSize();
	sf::Vector2f zeroPos = {x * gridSize.x, y * gridSize.y };




	if (watingVertexList.empty())
	{
		usingVertexList[x][y] = tileCount*4;

		tileMap.append(sf::Vertex(zeroPos, color));
		tileMap.append(sf::Vertex({ zeroPos.x + gridSize.x, zeroPos.y }, color));
		tileMap.append(sf::Vertex({ zeroPos.x + gridSize.x, zeroPos.y + gridSize.y }, color));
		tileMap.append(sf::Vertex({ zeroPos.x, zeroPos.y + gridSize.y }, color));

		tileCount++;
	}
	else
	{
		usingVertexList[x][y] = watingVertexList.front();

		tileMap[watingVertexList.front()].position = zeroPos;
		tileMap[watingVertexList.front()].color = color;
		tileMap[watingVertexList.front() + 1].position = { zeroPos.x + gridSize.x, zeroPos.y };
		tileMap[watingVertexList.front() + 1].color = color;
		tileMap[watingVertexList.front() + 2].position = { zeroPos.x + gridSize.x, zeroPos.y + gridSize.y };
		tileMap[watingVertexList.front() + 2].color = color;
		tileMap[watingVertexList.front() + 3].position = { zeroPos.x, zeroPos.y + gridSize.y };
		tileMap[watingVertexList.front() + 3].color = color;

		watingVertexList.pop_front();
	}
}

void ObjectTileMap::UpdateTile(const sf::Vector2i& gridCoord)
{
	UpdateTile(gridCoord.x, gridCoord.y);
}

void ObjectTileMap::ResetTile(int x, int y)
{
	if (usingVertexList[x].find(y) == usingVertexList[x].end())
		return;

	int temp = usingVertexList[x][y]; // tileCount에 해당하는 값
	watingVertexList.push_back(temp);
	tileMap[temp].color = sf::Color::Transparent;
	tileMap[temp + 1].color = sf::Color::Transparent;
	tileMap[temp + 2].color = sf::Color::Transparent;
	tileMap[temp + 3].color = sf::Color::Transparent;
	usingVertexList[x].erase(y);
}

void ObjectTileMap::Init()
{
	sceneGame = std::dynamic_pointer_cast<SceneGame, Scene>(scene.lock());
	tileMap.setPrimitiveType(sf::Quads);
	Reset();

}

void ObjectTileMap::Draw(sf::RenderWindow& window)
{
	window.draw(tileMap);
}

void ObjectTileMap::Reset()
{
	const GridInfo& gridInfo = sceneGame.lock()->GetGridInfo();
	tileMap.resize(0);
	for (auto& x : gridInfo)
	{
		for (auto& y : x.second)
		{
			UpdateTile(y.second.second->GetGridCoord());
		}
	}
}

void ObjectTileMap::Release()
{
	GameObject::Release();
}
