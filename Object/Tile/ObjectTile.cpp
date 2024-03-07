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

std::deque<sf::Vector2i> ObjectTile::FindShortPath(
	std::weak_ptr<ObjectTile> fromTile, GAME_OBJECT_TAG toTag, bool doCheck)
{
	std::deque<sf::Vector2i> realPath; //ã�� ���
	std::shared_ptr<ObjectTile> currentTile = fromTile.lock(); //���� Ÿ��

	if (fromTile.expired()) //���� Ÿ�Ͽ� �ƹ��͵� ���� ��� ����
		return realPath;
	if (currentTile->adjacent.empty()) //����� Ÿ���� ���� ��� ����
		return realPath;

	std::queue<std::weak_ptr<ObjectTile>> nodeList; //Ž���� ���
	std::unordered_map<int, std::unordered_map<int, bool>> visitList; //�湮 ���
	std::unordered_map<int, std::unordered_map<int, sf::Vector2i>> path; //�˻����� ��� ��ǥ (���� Ÿ�� ��ǥ)
	bool isFind = false; //��ǥ�� ã�Ҵ��� ����
	sf::Vector2i currentGridCoord = currentTile->gridCoord; //���� Ÿ����ǥ

	//�ڽſ� ���� ���� �˻�
	visitList[currentGridCoord.x][currentGridCoord.y] = true;
	const std::list<GAME_OBJECT_TAG>& nodeTags = currentTile->GetGameObjectTagList();
	if (std::find(nodeTags.begin(), nodeTags.end(), toTag) != nodeTags.end())
	{
		if (doCheck)
		{
			if (currentTile->ConditionCheck(toTag))
			{
				realPath.push_front(currentGridCoord); //���� ��ġ �߰�
				return realPath;
			}
		}
		else
		{
			realPath.push_front(currentGridCoord);
			return realPath;
		}
	}
	nodeList.push(fromTile);

	//�ٸ� Ÿ�� �˻�
	while (!nodeList.empty()) //Ž���� ��尡 ���� ������ �ݺ�
	{
		if (isFind) //ã������ while ����
			break;

		sf::Vector2i preGridCoord = nodeList.front().lock()->gridCoord; //��� ��ǥ
		for (auto& i : nodeList.front().lock()->adjacent) //����� ��������Ʈ ��ȸ
		{
			currentTile = i.second.lock(); //���� Ÿ�� ����
			currentGridCoord = currentTile->gridCoord; //���� Ÿ����ǥ ����
			if (visitList[currentGridCoord.x][currentGridCoord.y]) //�湮 �˻�
				continue;

			path[currentGridCoord.x][currentGridCoord.y] = preGridCoord; //���� Ÿ�� ��ǥ ���
			visitList[currentGridCoord.x][currentGridCoord.y] = true; //�湮 ���

			const std::list<GAME_OBJECT_TAG>& nodeTags = currentTile->GetGameObjectTagList();
			if (std::find(nodeTags.begin(), nodeTags.end(), toTag) != nodeTags.end())
			{
				if (doCheck)
				{
					if (currentTile->ConditionCheck(toTag))
					{
						//��ã�� BEGIN
						isFind = true;
						sf::Vector2i findRoad = currentGridCoord; //��θ� �ձ� ���� ����
						realPath.push_front(findRoad); //���� ��ġ �߰�
						while (findRoad != fromTile.lock()->gridCoord) //���� ��ǥ�� �ɶ����� �ݺ�
						{
							findRoad = path[findRoad.x][findRoad.y]; //findRoad�� �� Ÿ�� ��ǥ�� ����
							realPath.push_front(findRoad);//�� Ÿ�� ��ǥ �߰�
						}
						break;
						//��ã�� END
					}
				}
				else
				{
					//��ã�� BEGIN
					isFind = true;
					sf::Vector2i findRoad = currentGridCoord;
					realPath.push_front(findRoad);
					while (findRoad != fromTile.lock()->gridCoord)
					{
						findRoad = path[findRoad.x][findRoad.y];
						realPath.push_front(findRoad);
					}
					break;
					//��ã�� END
				}
			}
			if (!currentTile->adjacent.empty())
			{
				if (std::find(nodeTags.begin(), nodeTags.end(), GAME_OBJECT_TAG::MOVEABLE) != nodeTags.end())
					nodeList.push(currentTile);
			}
		}
		nodeList.pop();
	}

	return realPath;
}

bool ObjectTile::ConditionCheck(GAME_OBJECT_TAG tag)
{

	return true;
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
