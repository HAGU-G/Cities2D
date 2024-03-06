#include "pch.h"
#include "TileBuilding.h"
#include "ObjectUnit.h"


TileBuilding::TileBuilding(RCI rci, std::weak_ptr<Scene> scene, const sf::Vector2i& gridCoord)
	:ObjectTile(scene, GAME_OBJECT_TYPE::BUILDING, gridCoord), rci(rci)
{
	GM_RCI.UpdateRCI(rci.residence, rci.commerce, rci.industry);
}

TileBuilding::~TileBuilding()
{
	GM_RCI.UpdateRCI(-rci.residence, -rci.commerce, -rci.industry);
}

void TileBuilding::Update(float timeDelta, float timeScale)
{
	ObjectTile::Update(timeDelta, timeScale);
	buildingSprite.setRotation(scene.lock()->GetView().getRotation());
}

void TileBuilding::Draw(sf::RenderWindow& window)
{
	window.draw(buildingSprite);
	ObjectTile::Draw(window);
}

void TileBuilding::Reset()
{
	buildingSprite.setTexture(SFGM_TEXTURE.Get("resource/building/House01.png"));
	buildingSprite.setOrigin(buildingSprite.getGlobalBounds().getSize() * 0.5f);

	ObjectTile::Reset();
}

std::shared_ptr<ObjectTile> TileBuilding::Create(RCI rci, std::weak_ptr<Scene> scene, const sf::Vector2i& gridCoord)
{
	std::shared_ptr<ObjectTile> tileBuilding = std::make_shared<TileBuilding>(rci, scene, gridCoord);
	scene.lock()->AddObject(tileBuilding);
	tileBuilding->Init();

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

bool TileBuilding::MoveIn(std::weak_ptr<ObjectUnit> citizen)
{
	if (rci.residenceSlot.size() == rci.residence)
		return false;

	rci.residenceSlot.insert(std::make_pair(citizen.lock()->GetKey(), citizen));
	citizen.lock()->SetHome(std::dynamic_pointer_cast<TileBuilding, GameObject>(This()));
	GM_RCI.UseRegidence(1);
	return true;
}

void TileBuilding::MoveOut(const std::string& key)
{
	rci.residenceSlot.erase(key);
}
