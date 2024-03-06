#pragma once
#include "ObjectTile.h"
class TileRoad : public ObjectTile
{
protected:

public:
	explicit TileRoad(std::weak_ptr<Scene> scene, const sf::Vector2i& gridCoord);
	~TileRoad() override = default;
	TileRoad(const TileRoad&) = delete;
	TileRoad(TileRoad&&) = delete;
	TileRoad& operator=(const TileRoad&) = delete;
	TileRoad& operator=(TileRoad&&) = delete;

	static std::shared_ptr<ObjectTile> Create(std::weak_ptr<Scene> scene, const sf::Vector2i& gridCoord);

	void UpdateAdjacent() override;
};

