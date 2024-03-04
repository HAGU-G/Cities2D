#pragma once
#ifndef SCENE_H
#define SCENE_H
#include "SceneManager.h"

class GameObject;

class Scene
{
private:
	static size_t currentCount;
	static size_t totalCount;

	const std::string name;

protected:
	GameObjectList gameObjectList;
	std::list<std::pair<std::string, std::shared_ptr<GameObject>>> drawList;
	FilePathList resourcePathList; //파일 경로들이 담긴 컨테이너

	int drawLayer = 0;

	float timeScale = 1.f;
	bool useGlobalTimeScale = true;

	sf::View view;
	sf::Vector2f mousePosWorld;
	sf::Vector2f worldCenter = { 0.f, 0.f };

	explicit Scene(const std::string& name);
	Scene(const Scene&) = delete;
	Scene(Scene&&) = delete;
	Scene& operator=(const Scene&) = delete;
	Scene& operator=(Scene&&) = delete;

public:
	virtual ~Scene();

	virtual void Init();
	virtual void PreUpdate(float timeDelta, float timeScale);
	virtual void Update(float timeDelta, float timeScale);
	virtual void PhygicsUpdate(float timeDelta, float timeScale);
	virtual void Draw(sf::RenderWindow& window);
	virtual void Reset();
	virtual void Release();

	//Set
	void UseGlobalTimeScale(bool value) { useGlobalTimeScale = value; }
	void SetTimeScale(float value) { timeScale = value; }

	//Get
	static size_t GetScenesCount();
	static size_t GetScenesTotalCount();
	const std::string& GetSceneName() const;
	const std::shared_ptr<Scene>& This() { return SceneManager::Get(name); };
	const GameObjectList& GetObjects() const;
	const FilePathList& GetResourcePathList() const;
	float GetTimeScale() const;
	inline const sf::View& GetView() const { return view; }
	inline const sf::Vector2f& GetMousePosWolrd() const { return mousePosWorld; }

	//gameObjectsList
	bool AddObject(const std::shared_ptr<GameObject>& object);
	const std::shared_ptr<GameObject>& GetObject(const std::string& key) const;
	const std::shared_ptr<GameObject>& GetObject(const std::shared_ptr<GameObject>& object) const;
	bool DeleteObject(const std::string& key);

	//drawList
	void ResetDrawList();

};



#endif // !SCENE_H