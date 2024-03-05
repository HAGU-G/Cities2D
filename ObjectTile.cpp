#include "pch.h"
#include "ObjectTile.h"

ObjectTile::ObjectTile(std::weak_ptr<Scene> scene, const sf::Vector2i& gridCoord, GAME_OBJECT_TYPE objectType)
	:GameObject(scene, objectType), gridCoord(gridCoord)
{
}

bool ObjectTile::AddAdjacnet(const std::string& key, AdDirec ad, std::weak_ptr<ObjectTile> ptr)
{
	std::pair<std::string, AdDirec> pairKey;
	if (adjacent.find(pairKey) != adjacent.end())
	{
		adjacent[pairKey] = ptr;
		ptr.lock()->AddAdjacnet(GetKey(), AdDirec(~ad), std::dynamic_pointer_cast<ObjectTile, GameObject>(This()));
		return true;
	}
	else
	{
		return false;
	}
}

void ObjectTile::RemoveAdjacent()
{
	//TODO 인접 요소 제거
}
