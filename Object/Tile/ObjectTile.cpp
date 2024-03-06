#include "pch.h"
#include "ObjectTile.h"

ObjectTile::ObjectTile(std::weak_ptr<Scene> scene, GAME_OBJECT_TYPE objectType, const sf::Vector2i& gridCoord)
	:GameObject(scene, objectType), gridCoord(gridCoord)
{

}

ObjectTile::~ObjectTile()
{
	Release();
}

bool ObjectTile::AddAdjacent(ADDIREC ad, std::weak_ptr<ObjectTile> ptr)
{
	if (adjacent.find(ad) == adjacent.end())
	{
		adjacent[ad] = ptr;
		ptr.lock()->AddAdjacent(ADDIREC(~ad), std::dynamic_pointer_cast<ObjectTile, GameObject>(This()));
		UpdateEdge(ad);
		return true;
	}
	else
	{
		return false;
	}
}

void ObjectTile::RemoveAdjacent(ADDIREC ad)
{
	auto it = adjacent.find(ad);
	if (it != adjacent.end())
	{
		adjacent.erase(it);
	}
	UpdateEdge(ad);
}

void ObjectTile::Init()
{
	sceneGame = std::dynamic_pointer_cast<SceneGame, Scene>(scene.lock());
	SetPosition(sf::Vector2f(gridCoord) * sceneGame.lock()->GetGridSize().x);
	gridCenterPos = position + sceneGame.lock()->GetGridSize() * 0.5f;

	SFGM_TEXTURE.Load("resource/building/House01.png");

	edge.setPrimitiveType(sf::Lines);
	edge.resize(8);

	Reset();
}

void ObjectTile::Update(float timeDelta, float timeScale)
{
	tempSprite.setRotation(scene.lock()->GetView().getRotation());
}

void ObjectTile::Draw(sf::RenderWindow& window)
{
	window.draw(tempSprite);
	window.draw(edge);
}

void ObjectTile::Reset()
{
	tempSprite.setTexture(SFGM_TEXTURE.Get("resource/building/House01.png"));
	tempSprite.setOrigin(tempSprite.getGlobalBounds().getSize() * 0.5f);

	for (int i = 0; i < 8; i++)
	{
		edge[i].position = gridCenterPos;
		edge[i].color = sf::Color::Magenta;
	}

	UpdateAdjacent();
}

void ObjectTile::Release()
{
	GameObject::Release();
	if (!adjacent.empty())
	{
		for (auto& pair : adjacent)
		{
			if (!pair.second.expired())
				pair.second.lock()->RemoveAdjacent(ADDIREC(~pair.first));
		}
		adjacent.clear();
	}
}

std::shared_ptr<ObjectTile> ObjectTile::Create(std::weak_ptr<Scene> scene, GAME_OBJECT_TYPE objectType, const sf::Vector2i& gridCoord)
{
	std::shared_ptr<ObjectTile> objectTile = std::make_shared<ObjectTile>(scene, objectType, gridCoord);
	scene.lock()->AddObject(objectTile);
	objectTile->Init();

	return objectTile;
}

void ObjectTile::SetPosition(const sf::Vector2f& position)
{
	GameObject::SetPosition(position);
	tempSprite.setPosition(position + sceneGame.lock()->GetGridSize() * 0.5f);
}

void ObjectTile::UpdateAdjacent()
{
	//TODO 인접리스트 한번에 업데이트하는 작업 중.
	adjacent.clear();

	const TileInfo& upInfo = sceneGame.lock()->GetTileInfo(gridCoord.x, gridCoord.y - 1);
	const TileInfo& downInfo = sceneGame.lock()->GetTileInfo(gridCoord.x, gridCoord.y + 1);
	const TileInfo& leftInfo = sceneGame.lock()->GetTileInfo(gridCoord.x - 1, gridCoord.y);
	const TileInfo& rightInfo = sceneGame.lock()->GetTileInfo(gridCoord.x + 1, gridCoord.y);

	if (upInfo.first == GAME_OBJECT_TYPE::BUILDING)
	{
		AddAdjacent(AD_UP, upInfo.second);
	}
	if (downInfo.first == GAME_OBJECT_TYPE::BUILDING)
	{
		AddAdjacent(AD_DOWN, downInfo.second);
	}
	if (leftInfo.first == GAME_OBJECT_TYPE::BUILDING)
	{
		AddAdjacent(AD_LEFT, leftInfo.second);
	}
	if (rightInfo.first == GAME_OBJECT_TYPE::BUILDING)
	{
		AddAdjacent(AD_RIGHT, rightInfo.second);
	}
}

void ObjectTile::UpdateEdge(ADDIREC ad)
{
	switch (ad)
	{
	case ObjectTile::AD_UP:
		if (adjacent.find(AD_UP) == adjacent.end())
			edge[1].position = gridCenterPos;
		else
			edge[1].position = adjacent[AD_UP].lock()->GetGridCenterPos();
		break;
	case ObjectTile::AD_DOWN:
		if (adjacent.find(AD_DOWN) == adjacent.end())
			edge[3].position = gridCenterPos;
		else
			edge[3].position = adjacent[AD_DOWN].lock()->GetGridCenterPos();
		break;
	case ObjectTile::AD_LEFT:
		if (adjacent.find(AD_LEFT) == adjacent.end())
			edge[5].position = gridCenterPos;
		else
			edge[5].position = adjacent[AD_LEFT].lock()->GetGridCenterPos();
		break;
	case ObjectTile::AD_RIGHT:
		if (adjacent.find(AD_RIGHT) == adjacent.end())
			edge[7].position = gridCenterPos;
		else
			edge[7].position = adjacent[AD_RIGHT].lock()->GetGridCenterPos();
		break;
	default:
		break;
	}
}
