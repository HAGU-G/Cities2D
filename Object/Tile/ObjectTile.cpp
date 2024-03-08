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

void ObjectTile::SetPosition(const sf::Vector2f& position)
{
	GameObject::SetPosition(position);
	gridCenterPos = position + sceneGame.lock()->GetGridSize() * 0.5f;
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
	//std::cout << "태그 최단경로 찾기" << std::endl;
	std::deque<sf::Vector2i> realPath; //찾은 경로
	std::shared_ptr<ObjectTile> currentTile = fromTile.lock(); //현재 타일

	if (fromTile.expired()) //선택 타일에 아무것도 없을 경우 종료
		return realPath;
	if (currentTile->adjacent.empty()) //연결된 타일이 없을 경우 종료
		return realPath;

	std::queue<std::weak_ptr<ObjectTile>> nodeList; //탐색할 노드
	std::unordered_map<int, std::unordered_map<int, bool>> visitList; //방문 기록
	std::unordered_map<int, std::unordered_map<int, sf::Vector2i>> path; //검사중인 노드 좌표 (이전 타일 좌표)
	bool isFind = false; //목표를 찾았는지 여부
	sf::Vector2i currentGridCoord = currentTile->gridCoord; //현재 타일좌표

	//자신에 대해 먼저 검사
	visitList[currentGridCoord.x][currentGridCoord.y] = true;
	const std::list<GAME_OBJECT_TAG>& nodeTags = currentTile->GetGameObjectTagList();
	if (std::find(nodeTags.begin(), nodeTags.end(), toTag) != nodeTags.end())
	{
		if (doCheck)
		{
			if (currentTile->ConditionCheck(toTag))
			{
				realPath.push_front(currentGridCoord); //현재 위치 추가
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

	//다른 타일 검사
	while (!nodeList.empty()) //탐색할 노드가 없을 때까지 반복
	{
		if (isFind) //찾았으면 while 종료
			break;

		sf::Vector2i preGridCoord = nodeList.front().lock()->gridCoord; //노드 좌표
		for (auto& i : nodeList.front().lock()->adjacent) //노드의 인접리스트 순회
		{
			if (i.second.expired())
				continue;
			currentTile = i.second.lock(); //현재 타일 설정
			currentGridCoord = currentTile->gridCoord; //현재 타일좌표 설정
			if (visitList[currentGridCoord.x][currentGridCoord.y]) //방문 검사
				continue;

			path[currentGridCoord.x][currentGridCoord.y] = preGridCoord; //이전 타일 좌표 기록
			visitList[currentGridCoord.x][currentGridCoord.y] = true; //방문 기록

			const std::list<GAME_OBJECT_TAG>& nodeTags = currentTile->GetGameObjectTagList();
			if (std::find(nodeTags.begin(), nodeTags.end(), toTag) != nodeTags.end())
			{
				if (doCheck)
				{
					if (currentTile->ConditionCheck(toTag))
					{
						//길찾음 BEGIN
						isFind = true;
						sf::Vector2i findRoad = currentGridCoord; //경로를 잇기 위한 변수
						realPath.push_front(findRoad); //현재 위치 추가
						while (findRoad != fromTile.lock()->gridCoord) //시작 좌표가 될때까지 반복
						{
							findRoad = path[findRoad.x][findRoad.y]; //findRoad의 전 타일 좌표로 변경
							realPath.push_front(findRoad);//전 타일 좌표 추가
						}
						break;
						//길찾음 END
					}
				}
				else
				{
					//길찾음 BEGIN
					isFind = true;
					sf::Vector2i findRoad = currentGridCoord;
					realPath.push_front(findRoad);
					while (findRoad != fromTile.lock()->gridCoord)
					{
						findRoad = path[findRoad.x][findRoad.y];
						realPath.push_front(findRoad);
					}
					break;
					//길찾음 END
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

std::deque<sf::Vector2i> ObjectTile::FindShortPath(std::weak_ptr<ObjectTile> fromTile, std::weak_ptr<ObjectTile> toTile)
{
	//std::cout << "목적지 최단경로 찾기" << std::endl;
	std::deque<sf::Vector2i> realPath;
	if (fromTile.expired() || toTile.expired())
		return realPath;

	std::shared_ptr<ObjectTile> currentTile = fromTile.lock();
	if (currentTile->adjacent.empty())
		return realPath;

	std::queue<std::weak_ptr<ObjectTile>> nodeList;
	std::unordered_map<int, std::unordered_map<int, bool>> visitList;
	std::unordered_map<int, std::unordered_map<int, sf::Vector2i>> path;
	bool isFind = false;
	sf::Vector2i currentGridCoord = currentTile->gridCoord;

	//자신에 대해 먼저 검사
	visitList[currentGridCoord.x][currentGridCoord.y] = true;
	if (currentTile == toTile.lock())
	{
		realPath.push_front(currentGridCoord);
		return realPath;
	}
	nodeList.push(fromTile);

	//다른 타일 검사
	while (!nodeList.empty())
	{
		if (isFind)
			break;

		sf::Vector2i preGridCoord = nodeList.front().lock()->gridCoord;
		for (auto& i : nodeList.front().lock()->adjacent)
		{
			if (i.second.expired())
				continue;
			currentTile = i.second.lock();
			currentGridCoord = currentTile->gridCoord;
			if (visitList[currentGridCoord.x][currentGridCoord.y])
				continue;

			path[currentGridCoord.x][currentGridCoord.y] = preGridCoord;
			visitList[currentGridCoord.x][currentGridCoord.y] = true;

			if (currentTile == toTile.lock())
			{
				isFind = true;
				sf::Vector2i findRoad = currentGridCoord;
				realPath.push_front(findRoad);
				while (findRoad != fromTile.lock()->gridCoord)
				{
					findRoad = path[findRoad.x][findRoad.y];
					realPath.push_front(findRoad);
				}
				break;
			}

			if (!currentTile->adjacent.empty())
			{
				const std::list<GAME_OBJECT_TAG>& nodeTags = currentTile->GetGameObjectTagList();
				if (std::find(nodeTags.begin(), nodeTags.end(), GAME_OBJECT_TAG::MOVEABLE) != nodeTags.end())
					nodeList.push(currentTile);
			}
		}
		nodeList.pop();
	}

	return realPath;
}

sf::Vector2i ObjectTile::FindShortPath(sf::Vector2i fromGridCoord, GridInfo& gridInfo)
{
	//std::cout << "가까운 타일 찾기" << std::endl;
	std::queue<sf::Vector2i> nodeList; //탐색할 노드
	std::unordered_map<int, std::unordered_map<int, bool>> visitList; //방문 기록
	std::unordered_map<int, std::unordered_map<int, sf::Vector2i>> path; //검사중인 노드 좌표 (이전 타일 좌표)
	bool isFind = false; //목표를 찾았는지 여부
	sf::Vector2i currentGridCoord = fromGridCoord; //현재 그리드좌표
	std::shared_ptr<ObjectTile> currentTile = gridInfo[fromGridCoord.x][fromGridCoord.y].second.lock(); //현재 그리드좌표의 타일 포인터
	//자신에 대해 먼저 검사
	visitList[currentGridCoord.x][currentGridCoord.y] = true;
	if (currentTile != nullptr)
	{
		return currentGridCoord;
	}
	nodeList.push(currentGridCoord);

	//다른 타일 검사
	int count = 0;
	while (!nodeList.empty() && count < 181) //탐색할 노드가 없을 때까지 or 181번 반복 (반경 약 10칸)
	{
		count++;
		sf::Vector2i preGridCoord = nodeList.front(); //노드 좌표
		//노드의 상하좌우 검사
		//상
		currentGridCoord = preGridCoord + sf::Vector2i(0, -1);
		if (visitList[currentGridCoord.x][currentGridCoord.y])
			continue;
		visitList[currentGridCoord.x][currentGridCoord.y] = true;
		currentTile = gridInfo[currentGridCoord.x][currentGridCoord.y].second.lock();
		if (currentTile != nullptr)
			return currentGridCoord;
		nodeList.push(currentGridCoord);
		//하
		currentGridCoord = preGridCoord + sf::Vector2i(0, 1);
		if (visitList[currentGridCoord.x][currentGridCoord.y])
			continue;
		visitList[currentGridCoord.x][currentGridCoord.y] = true;
		currentTile = gridInfo[currentGridCoord.x][currentGridCoord.y].second.lock();
		if (currentTile != nullptr)
			return currentGridCoord;
		nodeList.push(currentGridCoord);
		//좌
		currentGridCoord = preGridCoord + sf::Vector2i(-1, 0);
		if (visitList[currentGridCoord.x][currentGridCoord.y])
			continue;
		visitList[currentGridCoord.x][currentGridCoord.y] = true;
		currentTile = gridInfo[currentGridCoord.x][currentGridCoord.y].second.lock();
		if (currentTile != nullptr)
			return currentGridCoord;
		nodeList.push(currentGridCoord);
		//우
		currentGridCoord = preGridCoord + sf::Vector2i(1, 0);
		if (visitList[currentGridCoord.x][currentGridCoord.y])
			continue;
		visitList[currentGridCoord.x][currentGridCoord.y] = true;
		currentTile = gridInfo[currentGridCoord.x][currentGridCoord.y].second.lock();
		if (currentTile != nullptr)
			return currentGridCoord;
		nodeList.push(currentGridCoord);

		nodeList.pop();
	}

	for (auto& x : gridInfo)
	{
		for (auto& y : x.second)
		{
			if (!y.second.second.expired())
				return y.second.second.lock()->GetGridCoord();
		}
	}

	return fromGridCoord;
}

bool ObjectTile::ConditionCheck(GAME_OBJECT_TAG tag)
{

	return true;
}

void ObjectTile::Init()
{
	sceneGame = std::dynamic_pointer_cast<SceneGame, Scene>(scene.lock());
	SetPosition(sf::Vector2f(gridCoord) * sceneGame.lock()->GetGridSize().x);

	edge.setPrimitiveType(sf::Lines);
	edge.resize(8);
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
