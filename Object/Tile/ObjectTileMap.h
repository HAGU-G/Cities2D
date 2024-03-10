#pragma once
#include "GameObject.h"

class SceneGame;

class ObjectTileMap : public GameObject
{
protected:
	sf::VertexArray tileMap;
	sf::VertexArray gridLine;
	sf::RenderStates renderStates;

	std::weak_ptr<SceneGame> sceneGame;
	int tileCount = 0;
	std::unordered_map<int, std::unordered_map<int, int>> usingVertexList; //[x][y] = vertexIndex (4개 중 첫번째)
	std::list<int> waitingVertexList; //vertexIndex (4개 중 첫번째)

	bool doDrawLine = false;

public:
	explicit ObjectTileMap(std::weak_ptr<Scene> scene, GAME_OBJECT_TYPE objectType = GAME_OBJECT_TYPE::GROUND);
	~ObjectTileMap() override;
	ObjectTileMap(const ObjectTileMap&) = delete;
	ObjectTileMap(ObjectTileMap&&) = delete;
	ObjectTileMap& operator=(const ObjectTileMap&) = delete;
	ObjectTileMap& operator=(ObjectTileMap&&) = delete;

	void Init() override;
	void Update(float timeDelta, float timeScale) override;
	void Draw(sf::RenderWindow& window) override;
	void Reset() override;
	void Release() override;
	
	static std::shared_ptr<ObjectTileMap> Create(std::weak_ptr<Scene> scene);
	void UpdateTile(int x, int y);
	void UpdateTile(const sf::Vector2i& gridCoord);
	void ResetTile(int x, int y);
	inline void TurnDrawLine() { doDrawLine = !doDrawLine; }
};

