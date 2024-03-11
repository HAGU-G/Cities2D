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
	MOVEABLE,
	R,
	C,
	I,

	COUNT
};

enum class GAME_OBJECT_TYPE
{
	GROUND = -1,
	NONE = 0,

	TILE,
	ROAD = TILE,
	BUILDING,
	HOME = BUILDING,
	SHOP,
	WORK_PLACE,
	BUILDING_END,
	TILE_END = BUILDING_END, //TILE_END - TILE = TILE_COUNT

	UNIT,
	CITIZEN = UNIT,
	UNIT_END,

	UI,
	BUTTON = UI,
	UI_END
};

class GameObject;
class ObjectTile;
class ObjectUnit;

typedef std::unordered_map<std::string, std::shared_ptr<GameObject>> GameObjectList;
typedef std::unordered_set<std::string> FilePathList;
typedef std::pair<GAME_OBJECT_TYPE, std::weak_ptr<ObjectTile>> TileInfo;
typedef std::unordered_map<int, std::unordered_map<int, TileInfo>> GridInfo;
typedef std::unordered_map<int, std::unordered_map<int, std::list<std::weak_ptr<ObjectUnit>>>> UnitOnGrid;

using std::to_string;

#define C_SCENE_GAME std::dynamic_pointer_cast<SceneGame, Scene>
#define C_TILE_BUILDING std::dynamic_pointer_cast<TileBuilding, ObjectTile>
#define C_OBJECT_UNIT std::dynamic_pointer_cast<ObjectUnit, GameObject>


#endif // !DEFINES_H