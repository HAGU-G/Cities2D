#include "pch.h"
#include "ObjectTile.h"

ObjectTile::ObjectTile(std::weak_ptr<Scene> scene, GAME_OBJECT_TYPE objectType, const sf::Vector2i& gridCoord)
	:GameObject(scene, objectType), gridCoord(gridCoord)
{

}

ObjectTile::~ObjectTile()
{
	Release();
}

bool ObjectTile::AddAdjacent(ADDIREC ad, std::weak_ptr<ObjectTile> ptr)
{
	if (adjacent.find(ad) == adjacent.end())
	{
		adjacent[ad] = ptr;
		ptr.lock()->AddAdjacent(ADDIREC(~ad), std::dynamic_pointer_cast<ObjectTile, GameObject>(This()));
		UpdateEdge(ad);
		return true;
	}
	else
	{
		return false;
	}
}

void ObjectTile::RemoveAdjacent(ADDIREC ad)
{
	auto it = adjacent.find(ad);
	if (it != adjacent.end())
	{
		adjacent.erase(it);
	}
	UpdateEdge(ad);
}

std::pair<bool, std::list<sf::Vector2i>> ObjectTile::FindShortPath(GAME_OBJECT_TAG toTag, bool doCheck)
{
	visitList.clear();
	std::list<sf::Vector2i> path;
	bool find = false;

	std::shared_ptr<ObjectTile> node;
	for (auto& i : adjacent)
	{
		node = i.second.lock();
		if (node->visit)
			continue;

		visitList.push_back(std::dynamic_pointer_cast<ObjectTile,GameObject>(node->This()));
		node->visit = true;
		const std::list<GAME_OBJECT_TAG>& nodeTags = node->GetGameObjectTagList();

		if (std::find(nodeTags.begin(), nodeTags.end(), toTag) != nodeTags.end())
		{
			if (doCheck)
			{
				if (ConditionCheck(toTag, node))
				{
					find = true;
					path.push_back(node->GetGridCoord());
					ResetVisit(visitList);
					return { find, path };
				}
			}
			else
			{
				find = true;
				path.push_back(node->GetGridCoord());
				ResetVisit(visitList);
				return { find, path };
			}
		}


		if (std::find(nodeTags.begin(), nodeTags.end(), GAME_OBJECT_TAG::MOVEABLE) != nodeTags.end())
		{
			path.push_back(node->GetGridCoord());
			std::pair<bool, std::list<sf::Vector2i>> tempPath = node->FindShortPath(toTag, doCheck);
			if (tempPath.first)
			{
				find = true;
				path.splice(path.end(), tempPath.second);
			}
		}
	}



//못찾아도 path는 넘겨야지
	return { find, path };
}

bool ObjectTile::ConditionCheck(GAME_OBJECT_TAG tag, std::weak_ptr<ObjectTile> tile)
{
	switch (tag)
	{
	case GAME_OBJECT_TAG::R:
		break;
	case GAME_OBJECT_TAG::C:

		break;
	case GAME_OBJECT_TAG::I:

		break;
	default:
		break;
	}

	return true;
}

void ObjectTile::ResetVisit(std::deque<std::weak_ptr<ObjectTile>>& visitList)
{
	for (auto& ptr : visitList)
	{
		ptr.lock()->visit = false;
	}
}

void ObjectTile::Init()
{
	sceneGame = std::dynamic_pointer_cast<SceneGame, Scene>(scene.lock());
	SetPosition(sf::Vector2f(gridCoord) * sceneGame.lock()->GetGridSize().x);
	gridCenterPos = position + sceneGame.lock()->GetGridSize() * 0.5f;

	edge.setPrimitiveType(sf::Lines);
	edge.resize(8);

	Reset();
}

void ObjectTile::Draw(sf::RenderWindow& window)
{
	window.draw(edge);
}

void ObjectTile::Reset()
{

	for (int i = 0; i < 8; i++)
	{
		edge[i].position = gridCenterPos;
		edge[i].color = sf::Color::Magenta;
	}

	UpdateAdjacent();
}

void ObjectTile::Release()
{
	GameObject::Release();
	if (!adjacent.empty())
	{
		for (auto& pair : adjacent)
		{
			if (!pair.second.expired())
				pair.second.lock()->RemoveAdjacent(ADDIREC(~pair.first));
		}
		adjacent.clear();
	}
}

void ObjectTile::UpdateEdge(ADDIREC ad)
{
	switch (ad)
	{
	case ObjectTile::AD_UP:
		if (adjacent.find(AD_UP) == adjacent.end())
			edge[1].position = gridCenterPos;
		else
			edge[1].position = adjacent[AD_UP].lock()->GetGridCenterPos();
		break;
	case ObjectTile::AD_DOWN:
		if (adjacent.find(AD_DOWN) == adjacent.end())
			edge[3].position = gridCenterPos;
		else
			edge[3].position = adjacent[AD_DOWN].lock()->GetGridCenterPos();
		break;
	case ObjectTile::AD_LEFT:
		if (adjacent.find(AD_LEFT) == adjacent.end())
			edge[5].position = gridCenterPos;
		else
			edge[5].position = adjacent[AD_LEFT].lock()->GetGridCenterPos();
		break;
	case ObjectTile::AD_RIGHT:
		if (adjacent.find(AD_RIGHT) == adjacent.end())
			edge[7].position = gridCenterPos;
		else
			edge[7].position = adjacent[AD_RIGHT].lock()->GetGridCenterPos();
		break;
	default:
		break;
	}
}
