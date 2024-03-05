#pragma once
#include "GameObject.h"
#include "SceneTest.h"
//#include <tuple>

class ObjectTile : public GameObject
{
public:
	//인접 방향 Adjacent direction
	enum ADDIREC
	{
		AD_UP = 0,
		AD_DOWN = ~AD_UP,
		AD_LEFT = 1,
		AD_RIGHT = ~AD_LEFT,
	};

protected:
	std::weak_ptr<SceneTest> sceneTest;
	sf::Vector2i gridCoord;
	sf::Vector2f gridCenterPos;

	std::unordered_map<ADDIREC, std::pair<std::string, std::weak_ptr<ObjectTile>>> adjacent; //인접 리스트 최대 4개

	sf::Sprite tempSprite;

public:
	explicit ObjectTile(std::weak_ptr<Scene> scene, GAME_OBJECT_TYPE objectType, const sf::Vector2i& gridCoord);
	~ObjectTile() override;
	ObjectTile(const ObjectTile&) = delete;
	ObjectTile(ObjectTile&&) = delete;
	ObjectTile& operator=(const ObjectTile&) = delete;
	ObjectTile& operator=(ObjectTile&&) = delete;

	void Init() override;
	void Update(float timeDelta, float timeScale) override;
	void Draw(sf::RenderWindow& window) override;
	void Reset() override;
	void Release() override;
	static std::shared_ptr<ObjectTile> Create(std::weak_ptr<Scene> scene, GAME_OBJECT_TYPE objectType, const sf::Vector2i& gridCoord);


	void SetPosition(const sf::Vector2f& position) override;

	bool AddAdjacnet(const std::string& key, ADDIREC ad, std::weak_ptr<ObjectTile> ptr);
	void RemoveAdjacent(ADDIREC ad);
};

