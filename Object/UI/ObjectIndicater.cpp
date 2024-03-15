#include "pch.h"
#include "ObjectIndicater.h"
#include "SceneGame.h"
#include "_Include_Tile.h"

ObjectIndicater::ObjectIndicater(std::weak_ptr<Scene> scene)
	:GameObject(scene, GAME_OBJECT_TYPE::NONE)
{
}

ObjectIndicater::~ObjectIndicater()
{
	Release();
}

void ObjectIndicater::Init()
{
	sceneGame = C_SCENE_GAME(scene.lock());
	rect.setSize(sceneGame.lock()->GetGridSize() - sf::Vector2f(4.f, 4.f));
	rect.setOrigin(-2.f, -2.f);
	rect.setOutlineThickness(4.f);

	destroySprite.setTexture(SFGM_TEXTURE.Get("resource/ui/destroySprite.png"));
	destroySprite.setOrigin(25.f, 25.f);
	destroySprite.setColor(destroy);
}

void ObjectIndicater::Update(float timeDelta, float timeScale)
{
	auto sceneGame = this->sceneGame.lock();

	//선택한 타일의 건물 스프라이트 불러오기
	SetPosition(sceneGame->GetSelectGridPos());
	if (preGridCoord != gridCoord)
	{
		preGridCoord = gridCoord;

		if (!sceneGame->GetTileInfo(gridCoord).second.expired())
			buildingSprite = &sceneGame->GetTileInfo(gridCoord).second.lock()->GetSprite();

	}
	if (sceneGame->GetTileInfo(gridCoord).second.expired())
		buildingSprite = nullptr;

	//클릭 모드
	switch (clickMode)
	{
	case -1:
		rect.setFillColor({ 160,60,50,80 });
		rect.setOutlineColor({ 120,30,20,180 });
		destroySprite.setRotation(sceneGame->GetView().getRotation());
		destroySprite.setPosition(position + destroySprite.getOrigin());
		if (buildingSprite != nullptr)
		{
			overSprite = *buildingSprite;
			overSprite.setColor(destroy);
		}
		break;

	case 0:
		rect.setFillColor({ 255,255,255,80 });
		rect.setOutlineColor({ 255,255,255,180 });
		break;

	case 1:
		if ((sceneGame->GetTileInfo(gridCoord + sf::Vector2i(0, -1)).first == GAME_OBJECT_TYPE::ROAD
			|| sceneGame->GetTileInfo(gridCoord + sf::Vector2i(0, 1)).first == GAME_OBJECT_TYPE::ROAD
			|| sceneGame->GetTileInfo(gridCoord + sf::Vector2i(-1, 0)).first == GAME_OBJECT_TYPE::ROAD
			|| sceneGame->GetTileInfo(gridCoord + sf::Vector2i(1, 0)).first == GAME_OBJECT_TYPE::ROAD)
			&& sceneGame->GetTileInfo(gridCoord).first == GAME_OBJECT_TYPE::NONE)
			canBuild = true;
		else
			canBuild = false;

		if (canBuild)
		{
			rect.setFillColor({ 50,120,60,80 });
			rect.setOutlineColor({ 20,80,30,180 });
		}
		else
		{
			rect.setFillColor({ 160,60,50,80 });
			rect.setOutlineColor({ 120,30,20,180 });
		}
		break;
	}

}

void ObjectIndicater::Draw(sf::RenderWindow& window)
{
	window.draw(rect);
	switch (clickMode)
	{
	case -1:
		window.draw(destroySprite);
		if (buildingSprite != nullptr)
			window.draw(overSprite);
		break;
	default:
		break;
	}

}

void ObjectIndicater::SetPosition(const sf::Vector2f& position)
{
	GameObject::SetPosition(position);
	rect.setPosition(position);
	gridCoord = sceneGame.lock()->PosToGridCoord(position);
}

std::shared_ptr<ObjectIndicater> ObjectIndicater::Create(std::weak_ptr<Scene> scene)
{
	std::shared_ptr<ObjectIndicater> objectindicater = std::make_shared<ObjectIndicater>(scene);
	scene.lock()->AddObject(objectindicater);
	objectindicater->Init();
	objectindicater->Reset();
	return objectindicater;
}
