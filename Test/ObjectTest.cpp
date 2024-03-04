#include "pch.h"
#include "ObjectTest.h"
#include "SceneTest.h"

ObjectTest::ObjectTest(const std::shared_ptr<Scene>& scene, GAME_OBJECT_TYPE objectType)
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
	(ResourceManager<sf::Texture>::Instance()).Load("building.png");
	sprite.setTexture(SFGM_TEXTURE.Get("mage.png"));
	sprite2.setTexture(SFGM_TEXTURE.Get("building.png"));
	sprite2.setOrigin(70.f, 240.f);
	sprite2.setScale(1.f*0.5f, (scene.lock()->GetView().getSize().y / GameManager::GetWindowSize().y)*0.5f);
}

void ObjectTest::Update(float timeDelta, float timeScale)
{
	sprite.setPosition(std::dynamic_pointer_cast<SceneTest, Scene>(scene.lock())->GetSelectGridPos());
	sprite2.setPosition(sprite.getPosition() + sf::Vector2f(sprite.getGlobalBounds().width,sprite.getGlobalBounds().height) * 0.5f);
	sprite2.setRotation(scene.lock()->GetView().getRotation());
}

void ObjectTest::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);
	window.draw(sprite2);
}
