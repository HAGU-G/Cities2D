#include "pch.h"
#include "TileHouse.h"



TileHouse::TileHouse(std::weak_ptr<Scene> scene, const sf::Vector2i& gridCoord)
	:ObjectTile(scene, GAME_OBJECT_TYPE::BUILDING, gridCoord)
{
}

std::shared_ptr<ObjectTile> TileHouse::Create(std::weak_ptr<Scene> scene, const sf::Vector2i& gridCoord)
{
	std::shared_ptr<ObjectTile> tileHouse = std::make_shared<TileHouse>(scene, gridCoord);
	scene.lock()->AddObject(tileHouse);
	tileHouse->Init();

	return tileHouse;
}

void TileHouse::UpdateAdjacent()
{
	adjacent.clear();

	const TileInfo& upInfo = sceneGame.lock()->GetTileInfo(gridCoord.x, gridCoord.y - 1);
	const TileInfo& downInfo = sceneGame.lock()->GetTileInfo(gridCoord.x, gridCoord.y + 1);
	const TileInfo& leftInfo = sceneGame.lock()->GetTileInfo(gridCoord.x - 1, gridCoord.y);
	const TileInfo& rightInfo = sceneGame.lock()->GetTileInfo(gridCoord.x + 1, gridCoord.y);

	if (upInfo.first == GAME_OBJECT_TYPE::ROAD)
	{
		AddAdjacent(AD_UP, upInfo.second);
	}
	if (downInfo.first == GAME_OBJECT_TYPE::ROAD)
	{
		AddAdjacent(AD_DOWN, downInfo.second);
	}
	if (leftInfo.first == GAME_OBJECT_TYPE::ROAD)
	{
		AddAdjacent(AD_LEFT, leftInfo.second);
	}
	if (rightInfo.first == GAME_OBJECT_TYPE::ROAD)
	{
		AddAdjacent(AD_RIGHT, rightInfo.second);
	}
}