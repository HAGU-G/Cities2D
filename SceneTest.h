#pragma once
#include "Scene.h"
class SceneTest : public Scene
{
protected:

public:
	explicit SceneTest(const std::string& name);
	~SceneTest();
	SceneTest(const Scene&) = delete;
	SceneTest(SceneTest&&) = delete;
	SceneTest& operator=(const SceneTest&) = delete;
	SceneTest& operator=(SceneTest&&) = delete;


	void Init() override;
	void Update(float timeDelta, float timeScale) override;
	void Draw(RenderWindow& window) override;

};

