#pragma once
#ifndef DEFINES_H
#define DEFINES_H

class GameObject;

typedef std::unordered_map<std::string, std::shared_ptr<GameObject>> GameObjectList;
typedef std::unordered_set<std::string> FilePathList;

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
	NONE = 0,
	BUILDING,
	ROAD,
	//You can edit here

	COUNT
};

#endif // !DEFINES_H