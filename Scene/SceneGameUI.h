#pragma once
#include "Scene.h"
class SceneGameUI : public Scene
{
protected:

public:
	explicit SceneGameUI(const std::string& name);
	~SceneGameUI() override = default;
	SceneGameUI(const Scene&) = delete;
	SceneGameUI(SceneGameUI&&) = delete;
	SceneGameUI& operator=(const SceneGameUI&) = delete;
	SceneGameUI& operator=(SceneGameUI&&) = delete;
};

