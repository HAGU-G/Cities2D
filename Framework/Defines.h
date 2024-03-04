#pragma once
#ifndef DEFINES_H
#define DEFINES_H
enum class GAME_OBJECT_TAG
{
	NONE = -1,
	ABLE_CLICK,
	ABLE_SELECT,
	DONT_DRAW,
	//You can edit here

	COUNT
};

enum class GAME_OBJECT_TYPE
{
	NONE = -1,
	ETC,
	BUILDING,
	ROAD,
	PERSON,
	CAR,
	LIGHT,
	//You can edit here

	COUNT
};

#endif // !DEFINES_H