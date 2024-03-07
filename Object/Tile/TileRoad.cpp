#include "pch.h"
#include "TileRoad.h"

TileRoad::TileRoad(std::weak_ptr<Scene> scene, const sf::Vector2i& gridCoord)
	:ObjectTile(scene, GAME_OBJECT_TYPE::ROAD, gridCoord)
{
}

void TileRoad::Init()
{
	ObjectTile::Init();
	AddTag(GAME_OBJECT_TAG::MOVEABLE);
}

std::shared_ptr<ObjectTile> TileRoad::Create(std::weak_ptr<Scene> scene, const sf::Vector2i& gridCoord)
{
	std::shared_ptr<ObjectTile> tileRoad = std::make_shared<TileRoad>(scene, gridCoord);
	scene.lock()->AddObject(tileRoad);
	tileRoad->Init();

	return tileRoad;
}

void TileRoad::UpdateAdjacent()
{
	adjacent.clear();

	const TileInfo& upInfo = sceneGame.lock()->GetTileInfo(gridCoord.x, gridCoord.y - 1);
	const TileInfo& downInfo = sceneGame.lock()->GetTileInfo(gridCoord.x, gridCoord.y + 1);
	const TileInfo& leftInfo = sceneGame.lock()->GetTileInfo(gridCoord.x - 1, gridCoord.y);
	const TileInfo& rightInfo = sceneGame.lock()->GetTileInfo(gridCoord.x + 1, gridCoord.y);

	if (upInfo.first >= GAME_OBJECT_TYPE::TILE && upInfo.first < GAME_OBJECT_TYPE::TILE_END)
	{
		AddAdjacent(AD_UP, upInfo.second);
	}
	if (downInfo.first >= GAME_OBJECT_TYPE::TILE && downInfo.first < GAME_OBJECT_TYPE::TILE_END)
	{
		AddAdjacent(AD_DOWN, downInfo.second);
	}
	if (leftInfo.first >= GAME_OBJECT_TYPE::TILE && leftInfo.first < GAME_OBJECT_TYPE::TILE_END)
	{
		AddAdjacent(AD_LEFT, leftInfo.second);
	}
	if (rightInfo.first >= GAME_OBJECT_TYPE::TILE && rightInfo.first < GAME_OBJECT_TYPE::TILE_END)
	{
		AddAdjacent(AD_RIGHT, rightInfo.second);
	}
}
