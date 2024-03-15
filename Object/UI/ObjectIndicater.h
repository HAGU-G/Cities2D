#pragma once
#include "GameObject.h"
#include "RCIManager.h"
class SceneGame;

class ObjectIndicater : public GameObject
{
protected:
	std::weak_ptr<SceneGame> sceneGame;
	RCI rci;

	sf::RectangleShape rect;
	const sf::Sprite* buildingSprite = nullptr;
	sf::Sprite overSprite;
	sf::Sprite destroySprite;
	sf::Color build{ 50,200,50,240 };
	sf::Color destroy{ 200,50,50,240 };
	sf::Text explain;

	sf::Vector2i gridCoord;
	sf::Vector2i preGridCoord;

	bool canClick = false;


public:
	int clickMode = 0; //0:Nothing 1:build -1:destroy

	explicit ObjectIndicater(std::weak_ptr<Scene> scene);
	~ObjectIndicater() override;
	ObjectIndicater(const ObjectIndicater&) = delete;
	ObjectIndicater(ObjectIndicater&&) = delete;
	ObjectIndicater& operator=(const ObjectIndicater&) = delete;
	ObjectIndicater& operator=(ObjectIndicater&&) = delete;

	void Init() override;
	void PreUpdate(float timeDelta, float timeScale) override;
	void Draw(sf::RenderWindow& window) override;
	void SetPosition(const sf::Vector2f& position) override;
	inline void SetRCI(RCI rci) { this->rci = rci;}

	inline bool CanClick() const { return canClick; }

	static std::shared_ptr<ObjectIndicater> Create(std::weak_ptr<Scene> scene);
};

