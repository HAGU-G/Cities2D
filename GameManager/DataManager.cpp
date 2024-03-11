#include "pch.h"
#include "DataManager.h"
#include "CsvFile.h"
#include "SceneGame.h"
#include "RCIManager.h"
#include "_Include_Tile.h"
#include "ObjectUnit.h"

bool DataManager::LoadTile(const std::shared_ptr<SceneGame>& sceneGame)
{
	const CsvFile& csv = SFGM_CSVFILE.Load("data/save/Tiles.csv");
	if (csv.IsUnknown())
		return false;
	const rapidcsv::Document& doc = csv.GetDocument();

	for (int i = 0; i < doc.GetRowCount(); i++)
	{
		auto row = doc.GetRow<std::string>(i);
		int divide = 0; // '/'로 구분
		std::string tempStr;

		//타입
		GAME_OBJECT_TYPE type = GAME_OBJECT_TYPE(std::stoi(row[0]));
		//그리드
		sf::Vector2i grid;
		divide = 0;
		tempStr = "";
		auto it = row[1].begin();
		while (it != row[1].end())
		{
			if (*it == '/')
			{
				if (divide == 0)
				{
					grid.x = std::stoi(tempStr);
					tempStr = "";
					divide++;
				}
				else if (divide == 1)
				{
					grid.y = std::stoi(tempStr);
					tempStr = "";
					divide++;
				}
			}
			else
			{
				tempStr += *it;
			}
			it++;
		}
		//태그
		std::list<GAME_OBJECT_TAG> tagList;
		tempStr = "";
		it = row[2].begin();
		while (it != row[2].end())
		{
			if (*it == '/')
			{
				tagList.push_back(GAME_OBJECT_TAG(std::stoi(tempStr)));
				tempStr = "";
			}
			else
			{
				tempStr += *it;
			}
			it++;
		}
		//TextureRect
		sf::IntRect rect;
		divide = 0;
		tempStr = "";
		it = row[3].begin();
		while (it != row[3].end())
		{
			if (*it == '/')
			{
				switch (divide)
				{
				case 0:
					rect.left = std::stoi(tempStr);
					break;
				case 1:
					rect.top = std::stoi(tempStr);
					break;
				case 2:
					rect.width = std::stoi(tempStr);
					break;
				case 3:
					rect.height = std::stoi(tempStr);
					break;
				default:
					break;
				}
				tempStr = "";
				divide++;
			}
			else
			{
				tempStr += *it;
			}
			it++;
		}
		//RCI
		RCI rci;
		divide = 0;
		tempStr = "";
		it = row[4].begin();
		while (it != row[4].end())
		{
			if (*it == '/')
			{
				switch (divide)
				{
				case 0:
					rci.residence = std::stoi(tempStr);
					break;
				case 1:
					rci.commerce = std::stoi(tempStr);
					break;
				case 2:
					rci.industry = std::stoi(tempStr);
					break;
				case 3:
					rci.cost = std::stoi(tempStr);
					break;
				default:
					break;
				}
				tempStr = "";
				divide++;
			}
			else
			{
				tempStr += *it;
			}
			it++;
		}

		//오브젝트 생성
		if (!sceneGame->LoadObjectTile(rci, grid, tagList, rect, type))
			return false;
	}

	SFGM_CSVFILE.UnLoad();
	return false;
}

bool DataManager::SaveTile(const std::shared_ptr<SceneGame>& sceneGame)
{
	const GridInfo& gridInfo = sceneGame->GetGridInfo();
	if (gridInfo.empty())
		return false;

	std::ofstream outFile("data/save/Tiles.csv");
	if (!outFile.is_open())
		return false;

	outFile << "OBJECT_TYPE,GRID,TAGS,TEXTURE_RECT,RCI" << std::endl;

	std::string str;
	std::string comma = ",";
	std::string slash = "/";
	for (auto& x : gridInfo)
	{
		for (auto& y : x.second)
		{
			if (y.second.second.expired())
				continue;

			str = comma; //타입은 switch문에서 앞쪽에 넣어준다.
			std::shared_ptr<ObjectTile> tile = y.second.second.lock();
			str += to_string(tile->GetGridCoord().x) + slash + to_string(tile->GetGridCoord().y) + slash + comma;

			for (auto& tag : tile->GetGameObjectTagList())
			{
				str += to_string((int)tag) + slash;
			}
			str += comma;

			const sf::IntRect& rect = tile->GetTextureRect();
			str += to_string(rect.left) + slash + to_string(rect.top) + slash
				+ to_string(rect.width) + slash + to_string(rect.height) + slash + comma;

			switch (y.second.first)
			{

			case GAME_OBJECT_TYPE::ROAD:
			{
				str = to_string((int)y.second.first) + str;
				str += "0/0/0/0/";

				break;
			}
			case GAME_OBJECT_TYPE::HOME:
			case GAME_OBJECT_TYPE::WORK_PLACE:
			{
				str = std::to_string((int)y.second.first) + str;
				std::shared_ptr<TileBuilding> building = std::dynamic_pointer_cast<TileBuilding, ObjectTile>(tile);

				const RCI& rci = building->GetRCI();
				str += to_string(rci.residence) + slash + to_string(rci.commerce) + slash + to_string(rci.industry) + slash + to_string(rci.cost) + slash;
				break;
			}
			default:
				break;
			}

			outFile << str << std::endl;

		}
	}
	outFile.close();
	return true;
}




bool DataManager::LoadUnit(const std::shared_ptr<SceneGame>& sceneGame)
{
	const CsvFile& csv = SFGM_CSVFILE.Load("data/save/Units.csv");
	if (csv.IsUnknown())
		return false;
	const rapidcsv::Document& doc = csv.GetDocument();

	for (int i = 0; i < doc.GetRowCount(); i++)
	{
		auto row = doc.GetRow<std::string>(i);
		int divide = 0; // '/'로 구분
		std::string tempStr;
		sf::Vector2i tempVi;
		sf::Vector2f tempVf;

		//타입
		std::shared_ptr<ObjectUnit> unit = sceneGame->AddUnit(ObjectUnit::Create(sceneGame));
		unit->SetPosition({ std::stof(row[1]), std::stof(row[2]) });

		//bool 형
		if (std::stoi(row[5]) & (1 << 3))
		{
			unit->BeCitizen();
		}
		else
		{
			unit->NoCitizen();
		}
		unit->isMoving = std::stoi(row[5]) & (1 << 0);

		//집 직장 설정
		divide = 0;
		tempStr = "";
		auto it = row[3].begin();
		while (it != row[3].end())
		{
			if (*it == '/')
			{
				switch (divide)
				{
				case 0:
					if (tempStr != "N")
						tempVi.x = std::stoi(tempStr);
					break;
				case 1:
					if (tempStr != "N")
					{
						tempVi.y = std::stoi(tempStr);
						std::shared_ptr<TileBuilding> tempTile = C_TILE_BUILDING(sceneGame->GetTileInfo(tempVi).second.lock());
						if (tempTile != nullptr)
						{
							tempTile->UseR(unit);
							unit->SetHome(tempTile);
						}
					}
					break;
				case 2:
					if (tempStr != "N")
						tempVi.x = std::stoi(tempStr);
					break;
				case 3:
					if (tempStr != "N")
					{
						tempVi.y = std::stoi(tempStr);
						std::shared_ptr<TileBuilding> tempTile = C_TILE_BUILDING(sceneGame->GetTileInfo(tempVi).second.lock());
						if (tempTile != nullptr)
						{
							tempTile->UseI(unit);
							unit->SetWorkPlace(tempTile);
						}
					}
					break;
				default:
					break;
				}
				tempStr = "";
				divide++;
			}
			else
			{
				tempStr += *it;
			}
			it++;
		}

		//pathToWorkPlace
		divide = 0;
		tempStr = "";
		if (row[4] != "N")
		{
			auto it = row[4].begin();
			while (it != row[4].end())
			{
				if (*it == '/')
				{
					if (divide == 0)
					{
						tempVi.x = std::stoi(tempStr);
						divide++;
					}
					else if (divide == 1)
					{
						tempVi.y = std::stoi(tempStr);
						unit->pathToWorkPlace.push_back(tempVi);
						divide--;
					}
					tempStr = "";
				}
				else
				{
					tempStr += *it;
				}
				it++;
			}
		}

		//bool형 2
		unit->hasHome = std::stoi(row[5]) & (1 << 2);
		unit->hasWorkPlace = std::stoi(row[5]) & (1 << 1);

		//find
		unit->findTimer = std::stof(row[6]);
		unit->findInterval = std::stof(row[7]);

		//patience
		unit->patience = std::stoi(row[8]);

		//status
		unit->status = ObjectUnit::STATUS(std::stoi(row[9]));

		//life
		unit->lifeTimer = std::stof(row[10]);
		unit->lifeInterval = std::stof(row[11]);

		//speed
		unit->speed = std::stof(row[12]);

		//walkPath
		divide = 0;
		tempStr = "";
		if (row[13] != "N")
		{
			auto it = row[13].begin();
			while (it != row[13].end())
			{
				if (*it == '/')
				{
					if (divide == 0)
					{
						tempVi.x = std::stoi(tempStr);
						divide++;
					}
					else if (divide == 1)
					{
						tempVi.y = std::stoi(tempStr);
						unit->walkPath.push_back(tempVi);
						divide--;
					}
					tempStr = "";
				}
				else
				{
					tempStr += *it;
				}
				it++;
			}
		}

		//길찾기,이전좌표
		divide = 0;
		tempStr = "";
		it = row[14].begin();
		while (it != row[14].end())
		{
			if (*it == '/')
			{
				switch (divide)
				{
				case 0:
					if (tempStr != "N")
						tempVi.x = std::stoi(tempStr);
					break;
				case 1:
					if (tempStr != "N")
					{
						tempVi.y = std::stoi(tempStr);
						unit->nextTile = C_TILE_BUILDING(sceneGame->GetTileInfo(tempVi).second.lock());
					}
					break;
				case 2:
					if (tempStr != "N")
						tempVi.x = std::stoi(tempStr);
					break;
				case 3:
					if (tempStr != "N")
					{
						tempVi.y = std::stoi(tempStr);
						unit->startingPoint = C_TILE_BUILDING(sceneGame->GetTileInfo(tempVi).second.lock());
					}
				case 4:
					if (tempStr != "N")
						tempVi.x = std::stoi(tempStr);
					break;
				case 5:
					if (tempStr != "N")
					{
						tempVi.y = std::stoi(tempStr);
						unit->destination = C_TILE_BUILDING(sceneGame->GetTileInfo(tempVi).second.lock());
					}
					break;
				case 6:
					tempVi.x = std::stoi(tempStr);
					break;
				case 7:
					tempVi.y = std::stoi(tempStr);
					unit->preGridCoord = tempVi;
					break;
				default:
					break;
				}
				tempStr = "";
				divide++;
			}
			else
			{
				tempStr += *it;
			}
			it++;
		}

		//태그
		std::list<GAME_OBJECT_TAG> tagList;
		tempStr = "";
		if (row[15] != "N")
		{
			it = row[15].begin();
			while (it != row[15].end())
			{
				if (*it == '/')
				{
					tagList.push_back(GAME_OBJECT_TAG(std::stoi(tempStr)));
					tempStr = "";
				}
				else
				{
					tempStr += *it;
				}
				it++;
			}
		}
		unit->gameObjectTagList = tagList;
	}

	SFGM_CSVFILE.UnLoad();
	return false;
}

bool DataManager::SaveUnit(const std::shared_ptr<SceneGame>& sceneGame)
{
	const std::unordered_map<std::string, std::weak_ptr<ObjectUnit>>& unitList = sceneGame->GetUnitList();
	if (unitList.empty())
		return false;

	std::ofstream outFile("data/save/Units.csv");
	if (!outFile.is_open())
		return false;

	outFile << "OBJECT_TYPE,POSITION_X,POSITION_Y,HOME/WORK_GRID,PATH_TO_WORK_PLACE,IS_CITIZEN/HAS_HOME/HAS_WORK_PLACE/IS_MOVING,FIND_TIMER,FIND_INTERVAL,PATIENCE,STATUS,LIFE_TIEMR,LIFE_INTERVAL,SPEED,WALK_PATH,NEXT/START/DEST/PRE_GRID,TAGS" << std::endl;

	std::string str;
	std::string comma = ",";
	std::string slash = "/";
	for (auto& pair : unitList)
	{
		if (pair.second.expired())
			continue;

		str = comma; //타입은 switch문에서 앞쪽에 넣어준다.
		std::shared_ptr<ObjectUnit> unit = pair.second.lock();

		//position
		str += to_string(unit->GetPosition().x) + comma + to_string(unit->GetPosition().y) + comma;

		//home, work grid
		if (!unit->home.expired())
		{
			str += to_string(unit->home.lock()->GetGridCoord().x) + slash + to_string(unit->home.lock()->GetGridCoord().y) + slash;
		}
		else
		{
			str += "N/N/";
		}
		if (!unit->workPlace.expired())
		{
			str += to_string(unit->workPlace.lock()->GetGridCoord().x) + slash + to_string(unit->workPlace.lock()->GetGridCoord().y) + slash;
		}
		else
		{
			str += "N/N/";
		}
		str += comma;

		//PATH_TO_WORK_PLACE
		if (!unit->pathToWorkPlace.empty())
		{
			for (auto& i : unit->pathToWorkPlace)
			{
				str += to_string(i.x) + slash + to_string(i.y) + slash;
			}
		}
		else
		{
			str += "N";
		}
		str += comma;

		//IS_CITIZEN / HAS_HOME / HAS_WORK_PLACE / IS_MOVING
		str += to_string(((int)(unit->isCitizen) << 3) + ((int)(unit->hasHome) << 2) + ((int)(unit->hasWorkPlace) << 1) + ((int)(unit->isMoving) << 0)) + comma;

		//FIND_TIMER, FIND_INTERVAL
		str += to_string(unit->findTimer) + comma;
		str += to_string(unit->findInterval) + comma;

		//PATIENCE
		str += to_string(unit->patience) + comma;

		//STATUS
		str += to_string((int)unit->status) + comma;

		//LIFE_TIEMR, LIFE_INTERVAL
		str += to_string(unit->lifeTimer) + comma;
		str += to_string(unit->lifeInterval) + comma;

		//SPEED
		str += to_string(unit->speed) + comma;

		//WALK_PATH
		if (!unit->walkPath.empty())
		{
			for (auto& i : unit->walkPath)
			{
				str += to_string(i.x) + slash + to_string(i.y) + slash;
			}
		}
		else
		{
			str += "N";
		}
		str += comma;

		//NEXT / START / DEST / PRE_GRID
		if (!unit->nextTile.expired())
		{
			str += to_string(unit->nextTile.lock()->GetGridCoord().x) + slash + to_string(unit->nextTile.lock()->GetGridCoord().y) + slash;
		}
		else
		{
			str += "N/N/";
		}
		if (!unit->startingPoint.expired())
		{
			str += to_string(unit->startingPoint.lock()->GetGridCoord().x) + slash + to_string(unit->startingPoint.lock()->GetGridCoord().y) + slash;
		}
		else
		{
			str += "N/N/";
		}
		if (!unit->destination.expired())
		{
			str += to_string(unit->destination.lock()->GetGridCoord().x) + slash + to_string(unit->destination.lock()->GetGridCoord().y) + slash;
		}
		else
		{
			str += "N/N/";
		}
		str += to_string(unit->preGridCoord.x) + slash + to_string(unit->preGridCoord.y) + slash + comma;

		//TAGS
		if (!unit->gameObjectTagList.empty())
		{
			for (auto& tag : unit->gameObjectTagList)
			{
				str += to_string((int)tag) + slash;
			}
		}
		else
		{
			str += "N";
		}

		switch (unit->GetGameObjectType())
		{

		case GAME_OBJECT_TYPE::CITIZEN:
		{
			str = to_string((int)GAME_OBJECT_TYPE::ROAD) + str;
			break;
		}
		default:
			break;
		}

		outFile << str << std::endl;

	}
	outFile.close();
	return true;



}

bool DataManager::SaveMayor(const std::shared_ptr<SceneGame>& sceneGame)
{
	return false;
}
