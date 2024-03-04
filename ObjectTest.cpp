#include "pch.h"
#include "ObjectTest.h"

ObjectTest::ObjectTest(GAME_OBJECT_TYPE objectType)
	:GameObject(objectType)
{
}

ObjectTest::~ObjectTest()
{
}

void ObjectTest::Init()
{
	(ResourceManager<sf::Texture>::Instance()).Load("mage.png");
	sprite.setTexture(SFGM_TEXTURE.Get("mage.png"));
}

void ObjectTest::Update(float timeDelta, float timeScale)
{
}

void ObjectTest::Draw(RenderWindow& window)
{
	window.draw(sprite);
}
