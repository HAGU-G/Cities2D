#pragma once
#ifndef DEFINES_H
#define DEFINES_H

enum class WINDOW_MODE
{
	WINDOW,
	BORDERLESS,
	FULLSCREEN
};

enum class GAME_OBJECT_TAG
{
	NONE = 0,
	//You can edit here

	COUNT
};

enum class GAME_OBJECT_TYPE
{
	GROUND = -1,
	NONE = 0,
	BUILDING,
	ROAD,
	//You can edit here

	COUNT
};

class GameObject;
class ObjectTile;

typedef std::unordered_map<std::string, std::shared_ptr<GameObject>> GameObjectList;
typedef std::unordered_set<std::string> FilePathList;
typedef std::pair<GAME_OBJECT_TYPE, std::shared_ptr<ObjectTile>> TileInfo;
typedef std::unordered_map<int, std::unordered_map<int, TileInfo>> GridInfo;




#endif // !DEFINES_H