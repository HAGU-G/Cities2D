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
	std::list<std::pair<std::string, std::weak_ptr<GameObject>>> drawList;
	FilePathList resourcePathList; //파일 경로들이 담긴 컨테이너
	std::deque<std::string> deleteDeque;

	int drawLayer = 0;

	float timeScale = 1.f;
	bool useGlobalTimeScale = true;

	sf::View view;
	sf::View resetView;
	sf::Vector2f mousePosWorld;
	sf::Vector2f worldCenter = { 0.f, 0.f };

	explicit Scene(const std::string& name);
	Scene(const Scene&) = delete;
	Scene(Scene&&) = delete;
	Scene& operator=(const Scene&) = delete;
	Scene& operator=(Scene&&) = delete;

public:
	virtual ~Scene();

	virtual void AddResource();//empty
	virtual void RemoveResource();//empty
	virtual void Init();
	virtual void PreUpdate(float timeDelta, float timeScale);	//생성
	virtual void Update(float timeDelta, float timeScale);		//메인 업데이트
	virtual void PostUpdate(float timeDelta, float timeScale);	//삭제
	virtual void PhysicsUpdate(float timeDelta, float timeScale);
	virtual void Draw(sf::RenderWindow& window);
	virtual void Reset();
	virtual void Release();

	virtual void Enter();
	virtual void Exit();

	//Set
	void UseGlobalTimeScale(bool value) { useGlobalTimeScale = value; }
	void SetTimeScale(float value) { timeScale = value; }

	//Get
	const std::shared_ptr<Scene>& This() { return SceneManager::Get(name); };
	static size_t GetScenesCount();
	static size_t GetScenesTotalCount();
	const std::string& GetSceneName() const;
	const GameObjectList& GetObjects() const;
	const FilePathList& GetResourcePathList() const;
	float GetTimeScale() const;
	inline const sf::View& GetView() const { return view; }
	inline const sf::View& GetResetView() const { return resetView; }
	inline const sf::Vector2f& GetMousePosWorld() const { return mousePosWorld; }

	//gameObjectsList
	const std::shared_ptr<GameObject>& AddObject(const std::shared_ptr<GameObject>& object);
	const std::shared_ptr<GameObject>& GetObject(const std::string& key) const;
	const std::shared_ptr<GameObject>& GetObject(std::weak_ptr<GameObject> object) const;
	void DeleteObject(const std::string& key);

	//drawList
	void ResetDrawList();

};



#endif // !SCENE_H