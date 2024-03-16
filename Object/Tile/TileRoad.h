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

	void Init() override;

	static std::shared_ptr<TileRoad> Create(std::weak_ptr<Scene> scene, const sf::Vector2i& gridCoord);
	static std::shared_ptr<TileRoad> Create(std::weak_ptr<Scene> scene, const sf::Vector2i& gridCoord,
		const std::list<GAME_OBJECT_TAG>& tagList);

	void UpdateAdjacent() override;
};

