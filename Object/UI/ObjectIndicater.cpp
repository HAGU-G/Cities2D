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

	explain.setCharacterSize(24);
	explain.setFillColor(sf::Color::Black);
	explain.setFont(SFGM_FONT.Get("resource/font/DOSIyagiBoldface.ttf"));
	explain.setOutlineThickness(2.f);
	explain.setOutlineColor(sf::Color::White);
}

void ObjectIndicater::PreUpdate(float timeDelta, float timeScale)
{
	auto sceneGame = this->sceneGame.lock();
	SetPosition(sceneGame->GetSelectGridPos());

	switch (clickMode)
	{
	case -1:
		if (gridCoord == sf::Vector2i(0, 0))
			canClick = false;
		else
			canClick = true;
		rect.setFillColor({ 160,60,50,80 });
		rect.setOutlineColor({ 120,30,20,180 });
		destroySprite.setRotation(sceneGame->GetView().getRotation());
		destroySprite.setPosition(position + destroySprite.getOrigin());

		if (preGridCoord != gridCoord)
		{
			preGridCoord = gridCoord;
			if (!sceneGame->GetTileInfo(gridCoord).second.expired())
				buildingSprite = &sceneGame->GetTileInfo(gridCoord).second.lock()->GetSprite();
		}

		if (sceneGame->GetTileInfo(gridCoord).second.expired())
		{
			buildingSprite = nullptr;
		}
		else
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
			&& sceneGame->GetTileInfo(gridCoord).first == GAME_OBJECT_TYPE::NONE
			&& sceneGame->GetMoney() >= rci.cost)
			canClick = true;
		else
			canClick = false;


		if (canClick)
		{
			rect.setFillColor({ 50,120,60,80 });
			rect.setOutlineColor({ 20,80,30,180 });
			overSprite.setColor({ build.r, build.g, build.b, 80 });
		}
		else
		{
			rect.setFillColor({ 160,60,50,80 });
			rect.setOutlineColor({ 120,30,20,180 });
			overSprite.setColor({ destroy.r, destroy.g, destroy.b, 80 });
		}
		std::wstringstream text;
		if (rci.residence > 0)
			text << L"주거: " << rci.residence <<"  ";
		if (rci.commerce > 0)
			text << L"상업: " << rci.commerce << "  ";
		if (rci.industry > 0)
			text << L"일자리: " << rci.industry << "  ";
		text << L"\n가격: " << rci.cost << L" / 유지비: " << -rci.tex;
		if (sceneGame->GetMoney() < rci.cost)
			text << L"\n ** 자금이 부족합니다 **";

		overSprite.setTexture(SFGM_TEXTURE.Get(rci.texturePath));
		overSprite.setTextureRect(rci.textureRect);
		overSprite.setOrigin(overSprite.getLocalBounds().width * 0.5f,
			overSprite.getLocalBounds().height - sceneGame->GetGridSize().y);
		overSprite.setRotation(sceneGame->GetView().getRotation());
		overSprite.setScale(1.f, sceneGame->GetTilt());
		overSprite.setPosition(position + destroySprite.getOrigin());

		explain.setString(text.str());
		explain.setOrigin(-destroySprite.getOrigin().x * 1.2f / sceneGame->GetZoomRatio(), destroySprite.getOrigin().y / sceneGame->GetZoomRatio() / sceneGame->GetTilt());
		explain.setPosition(position + destroySprite.getOrigin());
		explain.setRotation(sceneGame->GetView().getRotation());
		explain.setScale(1.f * sceneGame->GetZoomRatio(), sceneGame->GetTilt() * sceneGame->GetZoomRatio());
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
	case 1:
		window.draw(overSprite);
		window.draw(explain);
		break;
	}

}

void ObjectIndicater::SetPosition(const sf::Vector2f& position)
{
	GameObject::SetPosition(position);
	rect.setPosition(position);
	gridCoord = sceneGame.lock()->PosToGridCoord(position);
	explain.setString("");
}

std::shared_ptr<ObjectIndicater> ObjectIndicater::Create(std::weak_ptr<Scene> scene)
{
	std::shared_ptr<ObjectIndicater> objectindicater = std::make_shared<ObjectIndicater>(scene);
	scene.lock()->AddObject(objectindicater);
	objectindicater->Init();
	objectindicater->Reset();
	return objectindicater;
}
