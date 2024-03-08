#include "pch.h"
#include "DataManager.h"
#include "CsvFile.h"
#include "SceneGame.h"
#include "RCIManager.h"
#include "_Include_Tile.h"
bool DataManager::LoadGame(const std::shared_ptr<SceneGame>& sceneGame)
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
		if (!sceneGame->LoadObjectTile(type, grid, tagList, rect, rci))
			return false;
	}

	SFGM_CSVFILE.UnLoad();

	return false;
}

bool DataManager::SaveGame(const std::shared_ptr<SceneGame>& sceneGame)
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
				str = to_string((int)GAME_OBJECT_TYPE::ROAD) + str;
				str += "0/0/0/";

				break;
			}
			case GAME_OBJECT_TYPE::BUILDING:
			{
				str = std::to_string((int)GAME_OBJECT_TYPE::BUILDING) + str;
				std::shared_ptr<TileBuilding> building = std::dynamic_pointer_cast<TileBuilding, ObjectTile>(tile);

				const RCI& rci = building->GetRCI();
				str += to_string(rci.residence) + slash + to_string(rci.commerce) + slash + to_string(rci.industry) + slash;
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
