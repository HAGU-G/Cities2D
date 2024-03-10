#include "pch.h"
#include "ObjectIndicater.h"
#include "SceneGame.h"

ObjectIndicater::ObjectIndicater(std::weak_ptr<Scene> scene, GAME_OBJECT_TYPE objectType)
	:GameObject(scene, objectType)
{
}

ObjectIndicater::~ObjectIndicater()
{
	Release();
}

void ObjectIndicater::Init()
{
	rect.setSize({ 50.f,50.f });
	rect.setFillColor({ 50,120,60,200 });
}

void ObjectIndicater::Update(float timeDelta, float timeScale)
{
	rect.setPosition(std::dynamic_pointer_cast<SceneGame, Scene>(scene.lock())->GetSelectGridPos());
}

void ObjectIndicater::Draw(sf::RenderWindow& window)
{
	window.draw(rect);
}
