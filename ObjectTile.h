#pragma once
#include "GameObject.h"
class ObjectTile : public GameObject
{
	explicit GameObject(const std::shared_ptr<Scene>& scene, GAME_OBJECT_TYPE objectType);
	~GameObject() override = default;
	GameObject(const GameObject&) = delete;
	GameObject(GameObject&&) = delete;
	GameObject& operator=(const GameObject&) = delete;
	GameObject& operator=(GameObject&&) = delete;
};

