#include "pch.h"
#include "ObjectTest.h"
#include "SceneGame.h"

ObjectTest::ObjectTest(std::weak_ptr<Scene> scene, GAME_OBJECT_TYPE objectType)
	:GameObject(scene, objectType)
{
}

ObjectTest::~ObjectTest()
{
	Release();
}

void ObjectTest::Init()
{
	(ResourceManager<sf::Texture>::Instance()).Load("mage.png");
	sprite.setTexture(SFGM_TEXTURE.Get("mage.png"));
}

void ObjectTest::Update(float timeDelta, float timeScale)
{
	sprite.setPosition(std::dynamic_pointer_cast<SceneGame, Scene>(scene.lock())->GetSelectGridPos());
}

void ObjectTest::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}
