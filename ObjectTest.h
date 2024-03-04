#pragma once
#include "GameObject.h"
class ObjectTest : public GameObject
{
protected:
	Sprite sprite;
	Texture texture;

public:
	explicit ObjectTest(GAME_OBJECT_TYPE objectType);
	~ObjectTest();
	ObjectTest(const ObjectTest&) = delete;
	ObjectTest(ObjectTest&&) = delete;
	ObjectTest& operator=(const ObjectTest&) = delete;
	ObjectTest& operator=(ObjectTest&&) = delete;


	void Init() override;
	void Update(float timeDelta, float timeScale) override;
	void Draw(RenderWindow& window) override;
};

