#pragma once
#include "ObjectTile.h"
#include "RCIManager.h"

class TileBuilding : public ObjectTile
{
protected:
	RCI rci;
	sf::Sprite buildingSprite;

public:
	explicit TileBuilding(RCI rci, std::weak_ptr<Scene> scene, const sf::Vector2i& gridCoord);
	~TileBuilding() override;
	TileBuilding(const TileBuilding&) = delete;
	TileBuilding(TileBuilding&&) = delete;
	TileBuilding& operator=(const TileBuilding&) = delete;
	TileBuilding& operator=(TileBuilding&&) = delete;

	void Update(float timeDelta, float timeScale);
	void Draw(sf::RenderWindow& window);
	void Reset();

	void SetPosition(const sf::Vector2f& position) override;
	static std::shared_ptr<ObjectTile> Create(RCI rci, std::weak_ptr<Scene> scene, const sf::Vector2i& gridCoord);
	void UpdateAdjacent() override;

	inline const RCI& GetRCI() { return rci; }
	bool MoveIn(std::weak_ptr<ObjectUnit> citizen);
};

