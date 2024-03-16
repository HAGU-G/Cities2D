#pragma once
#include "ObjectTile.h"
#include "RCIManager.h"

class DataManager;

class TileBuilding : public ObjectTile
{
	friend DataManager;

protected:
	RCI rci;
	float soundTimer = 0.f;
	float soundDuration = 0.f;

public:
	explicit TileBuilding(RCI rci, std::weak_ptr<Scene> scene, const sf::Vector2i& gridCoord, GAME_OBJECT_TYPE type);
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
	static std::shared_ptr<TileBuilding> Create(RCI rci, std::weak_ptr<Scene> scene, const sf::Vector2i& gridCoord, GAME_OBJECT_TYPE type);
	static std::shared_ptr<TileBuilding> Create(const RCI& rci, std::weak_ptr<Scene> scene, const sf::Vector2i& gridCoord,
		const std::list<GAME_OBJECT_TAG>& tagList, GAME_OBJECT_TYPE type, float soundTimer, float soundDuration);

	void UpdateAdjacent() override;

	inline const RCI& GetRCI() { return rci; }
	inline const sf::Sprite& GetSprite() const { return buildingSprite; }
	inline const sf::IntRect& GetTextureRect() { return rci.textureRect; }

	bool CanUseR(std::weak_ptr<ObjectUnit> citizen);
	void UseR(std::weak_ptr<ObjectUnit> citizen);
	void UnuseR(const std::string& key);

	bool CanUseI(std::weak_ptr<ObjectUnit> citizen);
	void UseI(std::weak_ptr<ObjectUnit> citizen);
	void UnuseI(const std::string& key);

	bool CanUseC(std::weak_ptr<ObjectUnit> citizen);
	void UseC(std::weak_ptr<ObjectUnit> citizen);
	void UnuseC(const std::string& key);

	bool ConditionCheck(GAME_OBJECT_TAG tag) override;

	void Enter();
	void PlaySound();
};

