#pragma once
#include "GameObject.h"
class ObjectIndicater : public GameObject
{
protected:
	sf::RectangleShape rect;

public:
	explicit ObjectIndicater(std::weak_ptr<Scene> scene, GAME_OBJECT_TYPE objectType);
	~ObjectIndicater() override;
	ObjectIndicater(const ObjectIndicater&) = delete;
	ObjectIndicater(ObjectIndicater&&) = delete;
	ObjectIndicater& operator=(const ObjectIndicater&) = delete;
	ObjectIndicater& operator=(ObjectIndicater&&) = delete;


	void Init() override;
	void Update(float timeDelta, float timeScale) override;
	void Draw(sf::RenderWindow& window) override;
};

