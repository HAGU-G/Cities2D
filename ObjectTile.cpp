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

bool ObjectTile::AddAdjacnet(const std::string& key, ADDIREC ad, std::weak_ptr<ObjectTile> ptr)
{
	if (adjacent.find(ad) != adjacent.end())
	{
		adjacent[ad] = std::make_pair(key, ptr);
		ptr.lock()->AddAdjacnet(GetKey(), ADDIREC(~ad), std::dynamic_pointer_cast<ObjectTile, GameObject>(This()));
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
