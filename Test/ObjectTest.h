#pragma once
#include "GameObject.h"
class ObjectTest : public GameObject
{
protected:
	sf::Sprite sprite;
	sf::Sprite sprite2;

public:
	explicit ObjectTest(std::weak_ptr<Scene> scene, GAME_OBJECT_TYPE objectType);
	~ObjectTest() override;
	ObjectTest(const ObjectTest&) = delete;
	ObjectTest(ObjectTest&&) = delete;
	ObjectTest& operator=(const ObjectTest&) = delete;
	ObjectTest& operator=(ObjectTest&&) = delete;


	void Init() override;
	void Update(float timeDelta, float timeScale) override;
	void Draw(sf::RenderWindow& window) override;
};

