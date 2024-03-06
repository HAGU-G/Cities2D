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
	if (adjacent.find(ad) != adjacent.end())
	{
		adjacent[ad] = std::make_pair(ptr.lock()->GetKey(), ptr);
		ptr.lock()->AddAdjacent(ADDIREC(~ad), std::dynamic_pointer_cast<ObjectTile, GameObject>(This()));
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
}

void ObjectTile::Init()
{
	sceneGame = std::dynamic_pointer_cast<SceneGame, Scene>(scene.lock());
	SetPosition(sf::Vector2f(gridCoord) * sceneGame.lock()->GetGridSize().x);
	gridCenterPos = position + sceneGame.lock()->GetGridSize() * 0.5f;

	SFGM_TEXTURE.Load("resource/building/House01.png");

	Reset();
}

void ObjectTile::Update(float timeDelta, float timeScale)
{
	tempSprite.setRotation(scene.lock()->GetView().getRotation());
}

void ObjectTile::Draw(sf::RenderWindow& window)
{
	window.draw(tempSprite);
}

void ObjectTile::Reset()
{
	tempSprite.setTexture(SFGM_TEXTURE.Get("resource/building/House01.png"));
	tempSprite.setOrigin(tempSprite.getGlobalBounds().getSize() * 0.5f);
	adjacent.clear();
	UpdateAdjacent();
}

void ObjectTile::Release()
{
	GameObject::Release();
	if (!adjacent.empty())
	{
		for (auto& pair : adjacent)
		{
			pair.second.second.lock()->RemoveAdjacent(ADDIREC(~pair.first));
		}
		adjacent.clear();
	}
}

std::shared_ptr<ObjectTile> ObjectTile::Create(std::weak_ptr<Scene> scene, GAME_OBJECT_TYPE objectType, const sf::Vector2i& gridCoord)
{
	std::shared_ptr<ObjectTile> objectTile = std::make_shared<ObjectTile>(scene, objectType, gridCoord);
	objectTile->Init();
	scene.lock()->AddObject(objectTile);

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

	const TileInfo& upInfo = sceneGame.lock()->GetTileInfo(gridCoord.x, gridCoord.y - 1);
	const TileInfo& downInfo =
	const TileInfo& leftInfo =
	const TileInfo& rightInfo =


	if (.first == GAME_OBJECT_TYPE::BUILDING)
	{
		AddAdjacent(AD_UP,)
	}
	sceneGame.lock()->GetTileInfo(gridCoord.x, gridCoord.y+1);
	sceneGame.lock()->GetTileInfo(gridCoord.x-1, gridCoord.y);
	sceneGame.lock()->GetTileInfo(gridCoord.x+1, gridCoord.y);
}
