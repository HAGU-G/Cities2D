#pragma once
#ifndef SCENE_H
#define SCENE_H




class GameObject;

class Scene
{
private:
	static size_t currentCount;
	static size_t totalCount;

	const std::string name;

protected:
	std::unordered_map<std::string, std::shared_ptr<GameObject>> gameObjectList;
	std::list<std::pair<std::string, std::shared_ptr<GameObject>>> drawList;
	std::set<std::string> resourcePathList; //파일 경로들이 담긴 컨테이너

	bool doDraw = true;
	bool doUpdate = true;
	int drawLayer = 0;

	float timeScale = 1.f;
	bool useGlobalTimeScale = true;

	explicit Scene(const std::string& name);
	Scene(const Scene&) = delete;
	Scene(Scene&&) = delete;
	Scene& operator=(const Scene&) = delete;
	Scene& operator=(Scene&&) = delete;

public:
	virtual ~Scene();

	virtual void Init();
	virtual void Update(float timeDelta, float timeScale);
	virtual void PhygicsUpdate(float timeDelta, float timeScale);
	virtual void Draw(sf::RenderWindow& window);
	virtual void Reset();
	virtual void Release();

	//Set

	//Get
	static size_t GetScenesCount();
	static size_t GetScenesTotalCount();
	const std::string& GetSceneName();
	const std::unordered_map<std::string, std::shared_ptr<GameObject>>& GetObjects() const;
	const std::set<std::string>& GetResourcePathList() const;

	//gameObjectsList
	bool AddObject(const std::shared_ptr<GameObject>& object);
	const std::shared_ptr<GameObject>& GetObject(const std::string& key) const;
	const std::shared_ptr<GameObject>& GetObject(const std::shared_ptr<GameObject>& object) const;
	bool DeleteObject(const std::string& key);

	//drawList
	void ResetDrawList();

};



#endif // !SCENE_H