#include "pch.h"
#include "SceneTest.h"
#include "ObjectTest.h"

SceneTest::SceneTest(const std::string& name)
	:Scene(name)
{
}

SceneTest::~SceneTest()
{
}

void SceneTest::Init()
{
	AddObject(make_shared<ObjectTest>(GAME_OBJECT_TYPE::BUILDING));
	Scene::Init();
}

void SceneTest::Update(float timeDelta, float timeScale)
{
	Scene::Update(timeDelta, timeScale);
}

void SceneTest::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
