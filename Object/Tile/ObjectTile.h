#pragma once
#include "GameObject.h"
#include "SceneGame.h"
#include <queue>

class ObjectTile : public GameObject
{
protected:
	//인접 방향 Adjacent direction
	enum ADDIREC
	{
		AD_UP = 0,
		AD_DOWN = ~AD_UP,
		AD_LEFT = 1,
		AD_RIGHT = ~AD_LEFT,
	};

	std::weak_ptr<SceneGame> sceneGame;
	sf::Vector2i gridCoord;
	sf::Vector2f gridCenterPos;

	std::unordered_map<ADDIREC, std::weak_ptr<ObjectTile>> adjacent; //인접 리스트 최대 4개
	sf::VertexArray edge;

	explicit ObjectTile(std::weak_ptr<Scene> scene, GAME_OBJECT_TYPE objectType, const sf::Vector2i& gridCoord);
public:
	~ObjectTile() override;
	ObjectTile(const ObjectTile&) = delete;
	ObjectTile(ObjectTile&&) = delete;
	ObjectTile& operator=(const ObjectTile&) = delete;
	ObjectTile& operator=(ObjectTile&&) = delete;

	void Init() override;
	void Draw(sf::RenderWindow& window) override;
	void Reset() override;
	void Release() override;

	void SetPosition(const sf::Vector2f& position) override;
	bool AddAdjacent(ADDIREC ad, std::weak_ptr<ObjectTile> ptr);
	virtual void UpdateAdjacent() = 0;
	void UpdateEdge(ADDIREC ad);

	void RemoveAdjacent(ADDIREC ad);

	inline const sf::Vector2i& GetGridCoord() { return gridCoord; }
	inline const sf::Vector2f& GetGridCenterPos() { return gridCenterPos; }




	//fromTile에서 목표 태그 찾기
	static std::deque<sf::Vector2i> FindShortPath(
		std::weak_ptr<ObjectTile> fromTile, GAME_OBJECT_TAG toTag, bool doCheck = true);
	//fromTile에서 toTile 길 찾기
	static std::deque<sf::Vector2i> FindShortPath(
		std::weak_ptr<ObjectTile> fromTile, std::weak_ptr<ObjectTile> toTile);
	//fromGridCoord에서 가장 가까운 타일 찾기
	static sf::Vector2i FindShortPath(sf::Vector2i fromGridCoord, GridInfo& gridInfo);

	virtual bool ConditionCheck(GAME_OBJECT_TAG tag);
};

