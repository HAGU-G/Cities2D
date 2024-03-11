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
	scene.lock()->AddObject(objectTileMap);
	objectTileMap->Init();
	objectTileMap->Reset();
	return objectTileMap;
}

void ObjectTileMap::UpdateTile(int x, int y)
{
	//color를 texCoord로 변환할 필요가 있음.

	sf::Vector2f gridSize = sceneGame.lock()->GetGridSize();
	sf::Vector2f zeroPos = { x * gridSize.x, y * gridSize.y };
	sf::Vector2f centerPos = gridSize * 0.5f;

	sf::Vector2f zeroTex;

	switch (sceneGame.lock()->GetTileInfo(x, y).first)
	{
	case GAME_OBJECT_TYPE::ROAD:
		zeroTex = { 0, 50 };
		break;
	case GAME_OBJECT_TYPE::HOME:
		zeroTex = { 50, 50 };
		break;
	case GAME_OBJECT_TYPE::WORK_PLACE:
		zeroTex = { 100, 50 };
		break;
	case GAME_OBJECT_TYPE::SHOP:
		zeroTex = { 100, 0 };
		break;
	default:
		ResetTile(x, y);
		return;
		break;
	}

	//타일에 무언가 있을 때


	const std::unordered_map<ObjectTile::ADDIREC, std::weak_ptr<ObjectTile>>& adj
		= sceneGame.lock()->GetTileInfo(x, y).second.lock()->GetAdjacent();

	sf::Vector2f link1 = zeroTex;
	sf::Vector2f link2 = zeroTex + centerPos;
	sf::Vector2f link3 = zeroTex + sf::Vector2f(0.f, gridSize.y);

	sf::Vector2f unLink1 = zeroTex + sf::Vector2f(gridSize.x, 0.f);
	sf::Vector2f unLink2 = zeroTex + centerPos;
	sf::Vector2f unLink3 = zeroTex;
	if (usingVertexList[x].find(y) == usingVertexList[x].end())
	{
		if (waitingVertexList.empty())
		{
			usingVertexList[x][y] = tileCount * 12;

			if (adj.find(ObjectTile::AD_LEFT) != adj.end())
			{
				tileMap.append(sf::Vertex(zeroPos, link1));
				tileMap.append(sf::Vertex(zeroPos + centerPos, link2));
				tileMap.append(sf::Vertex(zeroPos + sf::Vector2f(0.f, gridSize.y), link3));
			}
			else
			{
				tileMap.append(sf::Vertex(zeroPos, unLink1));
				tileMap.append(sf::Vertex(zeroPos + centerPos, unLink2));
				tileMap.append(sf::Vertex(zeroPos + sf::Vector2f(0.f, gridSize.y), unLink3));
			}

			if (adj.find(ObjectTile::AD_UP) != adj.end())
			{
				tileMap.append(sf::Vertex(zeroPos + sf::Vector2f(gridSize.x, 0.f), link1));
				tileMap.append(sf::Vertex(zeroPos + centerPos, link2));
				tileMap.append(sf::Vertex(zeroPos, link3));
			}
			else
			{
				tileMap.append(sf::Vertex(zeroPos + sf::Vector2f(gridSize.x, 0.f), unLink1));
				tileMap.append(sf::Vertex(zeroPos + centerPos, unLink2));
				tileMap.append(sf::Vertex(zeroPos, unLink3));
			}

			if (adj.find(ObjectTile::AD_RIGHT) != adj.end())
			{
				tileMap.append(sf::Vertex(zeroPos + sf::Vector2f(gridSize.x, gridSize.y), link1));
				tileMap.append(sf::Vertex(zeroPos + centerPos, link2));
				tileMap.append(sf::Vertex(zeroPos + sf::Vector2f(gridSize.x, 0.f), link3));
			}
			else
			{
				tileMap.append(sf::Vertex(zeroPos + sf::Vector2f(gridSize.x, gridSize.y), unLink1));
				tileMap.append(sf::Vertex(zeroPos + centerPos, unLink2));
				tileMap.append(sf::Vertex(zeroPos + sf::Vector2f(gridSize.x, 0.f), unLink3));
			}

			if (adj.find(ObjectTile::AD_DOWN) != adj.end())
			{
				tileMap.append(sf::Vertex(zeroPos + sf::Vector2f(0.f, gridSize.y), link1));
				tileMap.append(sf::Vertex(zeroPos + centerPos, link2));
				tileMap.append(sf::Vertex(zeroPos + sf::Vector2f(gridSize.x, gridSize.y), link3));
			}
			else
			{
				tileMap.append(sf::Vertex(zeroPos + sf::Vector2f(0.f, gridSize.y), unLink1));
				tileMap.append(sf::Vertex(zeroPos + centerPos, unLink2));
				tileMap.append(sf::Vertex(zeroPos + sf::Vector2f(gridSize.x, gridSize.y), unLink3));
			}
			tileCount++;
		}
		else
		{
			usingVertexList[x][y] = waitingVertexList.front();

			if (adj.find(ObjectTile::AD_LEFT) != adj.end())
			{
				tileMap[waitingVertexList.front()] = sf::Vertex(sf::Vertex(zeroPos, link1));
				tileMap[waitingVertexList.front() + 1] = sf::Vertex(zeroPos + centerPos, link2);
				tileMap[waitingVertexList.front() + 2] = sf::Vertex(zeroPos + sf::Vector2f(0.f, gridSize.y), link3);
			}
			else
			{
				tileMap[waitingVertexList.front()] = sf::Vertex(zeroPos, unLink1);
				tileMap[waitingVertexList.front() + 1] = sf::Vertex(zeroPos + centerPos, unLink2);
				tileMap[waitingVertexList.front() + 2] = sf::Vertex(zeroPos + sf::Vector2f(0.f, gridSize.y), unLink3);
			}

			if (adj.find(ObjectTile::AD_UP) != adj.end())
			{
				tileMap[waitingVertexList.front() + 3] = sf::Vertex(zeroPos + sf::Vector2f(gridSize.x, 0.f), link1);
				tileMap[waitingVertexList.front() + 4] = sf::Vertex(zeroPos + centerPos, link2);
				tileMap[waitingVertexList.front() + 5] = sf::Vertex(zeroPos, link3);
			}
			else
			{
				tileMap[waitingVertexList.front() + 3] = sf::Vertex(zeroPos + sf::Vector2f(gridSize.x, 0.f), unLink1);
				tileMap[waitingVertexList.front() + 4] = sf::Vertex(zeroPos + centerPos, unLink2);
				tileMap[waitingVertexList.front() + 5] = sf::Vertex(zeroPos, unLink3);
			}

			if (adj.find(ObjectTile::AD_RIGHT) != adj.end())
			{
				tileMap[waitingVertexList.front() + 6] = sf::Vertex(zeroPos + sf::Vector2f(gridSize.x, gridSize.y), link1);
				tileMap[waitingVertexList.front() + 7] = sf::Vertex(zeroPos + centerPos, link2);
				tileMap[waitingVertexList.front() + 8] = sf::Vertex(zeroPos + sf::Vector2f(gridSize.x, 0.f), link3);
			}
			else
			{
				tileMap[waitingVertexList.front() + 6] = sf::Vertex(zeroPos + sf::Vector2f(gridSize.x, gridSize.y), unLink1);
				tileMap[waitingVertexList.front() + 7] = sf::Vertex(zeroPos + centerPos, unLink2);
				tileMap[waitingVertexList.front() + 8] = sf::Vertex(zeroPos + sf::Vector2f(gridSize.x, 0.f), unLink3);
			}

			if (adj.find(ObjectTile::AD_DOWN) != adj.end())
			{
				tileMap[waitingVertexList.front() + 9] = sf::Vertex(zeroPos + sf::Vector2f(0.f, gridSize.y), link1);
				tileMap[waitingVertexList.front() + 10] = sf::Vertex(zeroPos + centerPos, link2);
				tileMap[waitingVertexList.front() + 11] = sf::Vertex(zeroPos + sf::Vector2f(gridSize.x, gridSize.y), link3);
			}
			else
			{
				tileMap[waitingVertexList.front() + 9] = sf::Vertex(zeroPos + sf::Vector2f(0.f, gridSize.y), unLink1);
				tileMap[waitingVertexList.front() + 10] = sf::Vertex(zeroPos + centerPos, unLink2);
				tileMap[waitingVertexList.front() + 11] = sf::Vertex(zeroPos + sf::Vector2f(gridSize.x, gridSize.y), unLink3);
			}


			waitingVertexList.pop_front();
		}
	}
	else
	{
		int i = usingVertexList[x][y];

		if (adj.find(ObjectTile::AD_LEFT) != adj.end())
		{
			tileMap[i] = sf::Vertex(sf::Vertex(zeroPos, link1));
			tileMap[i + 1] = sf::Vertex(zeroPos + centerPos, link2);
			tileMap[i + 2] = sf::Vertex(zeroPos + sf::Vector2f(0.f, gridSize.y), link3);
		}
		else
		{
			tileMap[i] = sf::Vertex(zeroPos, unLink1);
			tileMap[i + 1] = sf::Vertex(zeroPos + centerPos, unLink2);
			tileMap[i + 2] = sf::Vertex(zeroPos + sf::Vector2f(0.f, gridSize.y), unLink3);
		}

		if (adj.find(ObjectTile::AD_UP) != adj.end())
		{
			tileMap[i + 3] = sf::Vertex(zeroPos + sf::Vector2f(gridSize.x, 0.f), link1);
			tileMap[i + 4] = sf::Vertex(zeroPos + centerPos, link2);
			tileMap[i + 5] = sf::Vertex(zeroPos, link3);
		}
		else
		{
			tileMap[i + 3] = sf::Vertex(zeroPos + sf::Vector2f(gridSize.x, 0.f), unLink1);
			tileMap[i + 4] = sf::Vertex(zeroPos + centerPos, unLink2);
			tileMap[i + 5] = sf::Vertex(zeroPos, unLink3);
		}

		if (adj.find(ObjectTile::AD_RIGHT) != adj.end())
		{
			tileMap[i + 6] = sf::Vertex(zeroPos + sf::Vector2f(gridSize.x, gridSize.y), link1);
			tileMap[i + 7] = sf::Vertex(zeroPos + centerPos, link2);
			tileMap[i + 8] = sf::Vertex(zeroPos + sf::Vector2f(gridSize.x, 0.f), link3);
		}
		else
		{
			tileMap[i + 6] = sf::Vertex(zeroPos + sf::Vector2f(gridSize.x, gridSize.y), unLink1);
			tileMap[i + 7] = sf::Vertex(zeroPos + centerPos, unLink2);
			tileMap[i + 8] = sf::Vertex(zeroPos + sf::Vector2f(gridSize.x, 0.f), unLink3);
		}

		if (adj.find(ObjectTile::AD_DOWN) != adj.end())
		{
			tileMap[i + 9] = sf::Vertex(zeroPos + sf::Vector2f(0.f, gridSize.y), link1);
			tileMap[i + 10] = sf::Vertex(zeroPos + centerPos, link2);
			tileMap[i + 11] = sf::Vertex(zeroPos + sf::Vector2f(gridSize.x, gridSize.y), link3);
		}
		else
		{
			tileMap[i + 9] = sf::Vertex(zeroPos + sf::Vector2f(0.f, gridSize.y), unLink1);
			tileMap[i + 10] = sf::Vertex(zeroPos + centerPos, unLink2);
			tileMap[i + 11] = sf::Vertex(zeroPos + sf::Vector2f(gridSize.x, gridSize.y), unLink3);
		}

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
	waitingVertexList.push_back(temp);
	for (int i = 0; i < 12; i++)
	{
		tileMap[temp + i].color = sf::Color::Transparent;
	}
	usingVertexList[x].erase(y);
}

void ObjectTileMap::Init()
{
	sceneGame = std::dynamic_pointer_cast<SceneGame, Scene>(scene.lock());

	renderStates.texture = &SFGM_TEXTURE.Load("resource/tile/Tile.png");

	tileMap.setPrimitiveType(sf::Triangles);

	gridLine.setPrimitiveType(sf::Lines);
	gridLine.resize(80);
}

void ObjectTileMap::Update(float timeDelta, float timeScale)
{
	if (doDrawLine)
	{
		sf::Vector2f gridSize = C_SCENE_GAME(sceneGame.lock())->GetGridSize();
		sf::Vector2f gridPos = C_SCENE_GAME(sceneGame.lock())->GetSelectGridPos();

		sf::Vector2f hLeft(-9, -10);
		sf::Vector2f hRight(10, -10);
		sf::Vector2f vUp(-10, -9);
		sf::Vector2f vDown(-10, 10);

		sf::Vector2f right(1, 0);
		sf::Vector2f down(0, 1);

		for (int i = 0; i < gridLine.getVertexCount() / 4; i++)
		{

			gridLine[4 * i].position = gridPos + (hLeft += down) * gridSize.x;
			gridLine[4 * i + 1].position = gridPos + (hRight += down) * gridSize.x;
			gridLine[4 * i + 2].position = gridPos + (vUp += right) * gridSize.x;
			gridLine[4 * i + 3].position = gridPos + (vDown += right) * gridSize.x;

			gridLine[4 * i].color = sf::Color::Black;
			gridLine[4 * i + 1].color = sf::Color::Black;
			gridLine[4 * i + 2].color = sf::Color::Black;
			gridLine[4 * i + 3].color = sf::Color::Black;

		}
	}
}

void ObjectTileMap::Draw(sf::RenderWindow& window)
{
	window.draw(tileMap, renderStates);
	if (doDrawLine)
		window.draw(gridLine);
}

void ObjectTileMap::Reset()
{
	const GridInfo& gridInfo = sceneGame.lock()->GetGridInfo();
	tileMap.resize(0);
	usingVertexList.clear();
	waitingVertexList.clear();
	tileCount = 0;
	for (auto& x : gridInfo)
	{
		for (auto& y : x.second)
		{
			UpdateTile(y.second.second.lock()->GetGridCoord());
		}
	}
}

void ObjectTileMap::Release()
{
	waitingVertexList.clear();
	waitingVertexList.clear();
	tileCount = 0;
	GameObject::Release();
}
