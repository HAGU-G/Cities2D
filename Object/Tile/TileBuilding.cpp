#include "pch.h"
#include "TileBuilding.h"
#include "ObjectUnit.h"
#include <algorithm>


TileBuilding::TileBuilding(RCI rci, std::weak_ptr<Scene> scene, const sf::Vector2i& gridCoord, GAME_OBJECT_TYPE type)
	:ObjectTile(scene, gridCoord, type), rci(rci)
{
	GM_RCI.UpdateRCI(rci.residence, rci.commerce, rci.industry);
}

TileBuilding::~TileBuilding()
{
	Release();

}

void TileBuilding::Init()
{
	ObjectTile::Init();

}

void TileBuilding::Update(float timeDelta, float timeScale)
{
	ObjectTile::Update(timeDelta, timeScale);
	buildingSprite.setRotation(scene.lock()->GetView().getRotation());

	if (sceneGame.lock()->DoPayTex())
	{
		sceneGame.lock()->MoneyLoss(rci.tex);
	}
}

void TileBuilding::Draw(sf::RenderWindow& window)
{
	window.draw(buildingSprite);
	ObjectTile::Draw(window);
}

void TileBuilding::Reset()
{
	gameObjectTagList.clear();

	for (auto& pair : rci.residenceSlot)
	{
		if (!pair.second.expired())
			pair.second.lock()->NoHome();
	}
	rci.commerceSlot.clear();
	for (auto& pair : rci.industrySlot)
	{
		if (!pair.second.expired())
			pair.second.lock()->NoWorkPlace();
	}

	if (rci.residence > 0)
		AddTag(GAME_OBJECT_TAG::R);
	if (rci.commerce > 0)
		AddTag(GAME_OBJECT_TAG::C);
	if (rci.industry > 0)
		AddTag(GAME_OBJECT_TAG::I);

	buildingSprite.setTexture(SFGM_TEXTURE.Get("resource/tile/Buildings.png"));

	std::initializer_list<int> elements = { rci.residence, rci.commerce, rci.industry };
	int rciMax = std::max(elements);

	if (rciMax == rci.commerce)
	{
		textureRect = { 0, 0, 50, 50 };
		buildingSprite.setTextureRect(textureRect);
	}
	else if (rciMax == rci.industry)
	{
		textureRect = { 0, 50, 50, 100 };
		buildingSprite.setTextureRect(textureRect);
	}
	else
	{
		textureRect = { 0, 0, 50, 50 };
		buildingSprite.setTextureRect(textureRect);
	}

	buildingSprite.setOrigin(buildingSprite.getLocalBounds().width * 0.5f,
		buildingSprite.getLocalBounds().height - sceneGame.lock()->GetGridSize().y * 0.5f);

	ObjectTile::Reset();
}

void TileBuilding::Release()
{
	for (auto& pair :rci.residenceSlot)
	{
		if (!pair.second.expired())
			pair.second.lock()->NoHome();
	}
	rci.commerceSlot.clear();
	for (auto& pair : rci.industrySlot)
	{
		if (!pair.second.expired())
			pair.second.lock()->NoWorkPlace();
	}

	GM_RCI.UpdateRCI(-rci.residence, -rci.commerce, -rci.industry);
	rci = RCI();
}

std::shared_ptr<TileBuilding> TileBuilding::Create(RCI rci, std::weak_ptr<Scene> scene, const sf::Vector2i& gridCoord, GAME_OBJECT_TYPE type)
{
	std::shared_ptr<TileBuilding> tileBuilding = std::make_shared<TileBuilding>(rci, scene, gridCoord, type);
	scene.lock()->AddObject(tileBuilding);
	tileBuilding->Init();
	tileBuilding->Reset();
	return tileBuilding;
}

std::shared_ptr<TileBuilding> TileBuilding::Create(const RCI& rci, std::weak_ptr<Scene> scene, const sf::Vector2i& gridCoord,
	const std::list<GAME_OBJECT_TAG>& tagList, const sf::IntRect& rect, GAME_OBJECT_TYPE type)
{
	std::shared_ptr<TileBuilding> tileBuilding = Create(rci, scene, gridCoord, type);
	for (auto tag : tagList)
	{
		tileBuilding->AddTag(tag);
	}
	tileBuilding->SetTextureRect(rect);

	return tileBuilding;
}


void TileBuilding::SetPosition(const sf::Vector2f& position)
{
	ObjectTile::SetPosition(position);
	buildingSprite.setPosition(position + sceneGame.lock()->GetGridSize() * 0.5f);
}

void TileBuilding::UpdateAdjacent()
{
	adjacent.clear();

	const TileInfo& upInfo = sceneGame.lock()->GetTileInfo(gridCoord.x, gridCoord.y - 1);
	const TileInfo& downInfo = sceneGame.lock()->GetTileInfo(gridCoord.x, gridCoord.y + 1);
	const TileInfo& leftInfo = sceneGame.lock()->GetTileInfo(gridCoord.x - 1, gridCoord.y);
	const TileInfo& rightInfo = sceneGame.lock()->GetTileInfo(gridCoord.x + 1, gridCoord.y);

	if (upInfo.first == GAME_OBJECT_TYPE::ROAD)
	{
		AddAdjacent(AD_UP, upInfo.second);
	}
	if (downInfo.first == GAME_OBJECT_TYPE::ROAD)
	{
		AddAdjacent(AD_DOWN, downInfo.second);
	}
	if (leftInfo.first == GAME_OBJECT_TYPE::ROAD)
	{
		AddAdjacent(AD_LEFT, leftInfo.second);
	}
	if (rightInfo.first == GAME_OBJECT_TYPE::ROAD)
	{
		AddAdjacent(AD_RIGHT, rightInfo.second);
	}
}

bool TileBuilding::CanUseR(std::weak_ptr<ObjectUnit> citizen)
{
	if (citizen.expired())
		return false;
	if (adjacent.empty())
		return false;
	if (rci.residenceSlot.size() == rci.residence)
		return false;

	return true;
}

void TileBuilding::UseR(std::weak_ptr<ObjectUnit> citizen)
{
	rci.residenceSlot.insert(std::make_pair(citizen.lock()->GetKey(), citizen));
	GM_RCI.UseRegidence(1);
}

void TileBuilding::UnuseR(const std::string& key)
{
	if (rci.residenceSlot.erase(key) > 0)
		GM_RCI.UseRegidence(-1);
}

bool TileBuilding::CanUseI(std::weak_ptr<ObjectUnit> citizen)
{
	if (citizen.expired())
		return false;
	if (adjacent.empty())
		return false;
	if (rci.industrySlot.size() == rci.industry)
		return false;

	return true;
}

void TileBuilding::UseI(std::weak_ptr<ObjectUnit> citizen)
{
	rci.industrySlot.insert(std::make_pair(citizen.lock()->GetKey(), citizen));
	GM_RCI.UseIndustry(1);
}

void TileBuilding::UnuseI(const std::string& key)
{
	if (rci.industrySlot.erase(key) > 0)
		GM_RCI.UseIndustry(-1);
}

bool TileBuilding::ConditionCheck(GAME_OBJECT_TAG tag)
{
	switch (tag)
	{
	case GAME_OBJECT_TAG::R:
		if (rci.residenceSlot.size() < rci.residence)
			return true;
		break;
	case GAME_OBJECT_TAG::C:
		if (rci.commerceSlot.size() < rci.commerce)
			return true;
		break;
	case GAME_OBJECT_TAG::I:
		if (rci.industrySlot.size() < rci.industry)
			return true;
		break;
	default:
		break;
	}
	return false;
}
