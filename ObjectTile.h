#pragma once
#include "GameObject.h"
//#include <tuple>

class ObjectTile : public GameObject
{
public:
	//���� ���� Adjacent direction
	enum AdDirec
	{
		AD_UP = 1,
		AD_DOWN = ~AD_UP,
		AD_LEFT = 2,
		AD_RIGHT = ~AD_LEFT,
	};

protected:
	sf::Vector2i gridCoord;
	std::unordered_map<std::pair<std::string, AdDirec>, std::weak_ptr<ObjectTile>> adjacent; //���� ����Ʈ �ִ� 4��

public:
	explicit ObjectTile(std::weak_ptr<Scene> scene, const sf::Vector2i& gridCoord ,GAME_OBJECT_TYPE objectType);
	~ObjectTile() override = default;
	ObjectTile(const ObjectTile&) = delete;
	ObjectTile(ObjectTile&&) = delete;
	ObjectTile& operator=(const ObjectTile&) = delete;
	ObjectTile& operator=(ObjectTile&&) = delete;

	bool AddAdjacnet(const std::string& key, AdDirec ad, std::weak_ptr<ObjectTile> ptr);
	void RemoveAdjacent();
};

