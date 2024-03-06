#pragma once
#include "ObjectTile.h"
class TileHouse : public ObjectTile
{
protected:

public:
	explicit TileHouse(std::weak_ptr<Scene> scene, const sf::Vector2i& gridCoord);
	~TileHouse() override = default;
	TileHouse(const TileHouse&) = delete;
	TileHouse(TileHouse&&) = delete;
	TileHouse& operator=(const TileHouse&) = delete;
	TileHouse& operator=(TileHouse&&) = delete;

	static std::shared_ptr<ObjectTile> Create(std::weak_ptr<Scene> scene, const sf::Vector2i& gridCoord);

	void UpdateAdjacent() override;
};

