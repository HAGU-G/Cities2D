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

	void Init() override;
	void Update(float timeDelta, float timeScale) override;
	void Draw(sf::RenderWindow& window) override;
	void Reset() override;
	void Release() override;

	void SetPosition(const sf::Vector2f& position) override;
	static std::shared_ptr<ObjectTile> Create(RCI rci, std::weak_ptr<Scene> scene, const sf::Vector2i& gridCoord);
	static std::shared_ptr<ObjectTile> Create(std::weak_ptr<Scene> scene, const sf::Vector2i& gridCoord,
		const std::list<GAME_OBJECT_TAG>& tagList, const sf::IntRect& rect, const RCI& rci);
	
	void UpdateAdjacent() override;

	inline const RCI& GetRCI() { return rci; }
	inline const sf::Sprite& GetSprite() const { return buildingSprite; }

	bool MoveIn(std::weak_ptr<ObjectUnit> citizen);
	void MoveOut(const std::string& key);
	bool Join(std::weak_ptr<ObjectUnit> citizen);
	void Quit(const std::string& key);

	bool ConditionCheck(GAME_OBJECT_TAG tag) override;
};

