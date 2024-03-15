#pragma once
#ifndef OBJECT_H
#define OBJECT_H


class Scene;

class GameObject
{
private:
	static size_t currentCount;
	static size_t totalCount;

	std::string key; //"gameObjectType_CreatedTime_totalCount"
	GAME_OBJECT_TYPE gameObjectType;

	//AddObject 하기 전에 호출.
	void Setkey(const std::string& loadedKey);

protected:
	std::weak_ptr<Scene> scene;
	std::list<GAME_OBJECT_TAG> gameObjectTagList;

	float drawDeep = 0.f;

	bool isActive = true;
	bool isShow = true;
	bool isMute = false;

	sf::Vector2f position;
	sf::Vector2f direction;

	explicit GameObject(std::weak_ptr<Scene> scene, GAME_OBJECT_TYPE objectType = GAME_OBJECT_TYPE::NONE);
	GameObject(const GameObject&) = delete;
	GameObject(GameObject&&) = delete;
	GameObject& operator=(const GameObject&) = delete;
	GameObject& operator=(GameObject&&) = delete;

public:
	virtual ~GameObject();

	//Management
	virtual void Init(); //empty
	virtual void PreUpdate(float timeDelta, float timeScale); //empty
	virtual void Update(float timeDelta, float timeScale); //empty
	virtual void PostUpdate(float timeDelta, float timeScale); //empty
	virtual void Draw(sf::RenderWindow& window); //empty
	virtual void Reset(); //empty
	virtual void Release();

	//Set
	void SetScene(std::weak_ptr<Scene> scene);
	void SetPositionX(float x);
	void SetPositionY(float y);
	virtual void SetPosition(const sf::Vector2f& position);
	void SetDrawDeep(float deep);
	void SetActive(bool value);
	inline void SetShow(bool value) { isShow = value; }
	inline void SetMute(bool value) { isMute = value; }

	//Get
	const std::shared_ptr<GameObject>& This();
	static size_t GetObjectsCount();
	static size_t GetObjectsTotalCount();
	std::string GetKey() const;
	inline std::weak_ptr<Scene> GetScene() const { return scene; }
	const GAME_OBJECT_TYPE& GetGameObjectType() const;
	const std::list<GAME_OBJECT_TAG>& GetGameObjectTagList() const;
	const sf::Vector2f& GetPosition() const;
	const float& GetDrawDeep() const;
	inline bool IsActive() const { return isActive; }
	inline bool IsShow() const { return isShow; }
	inline bool IsMute() const { return isMute; }

	//Add
	bool AddTag(GAME_OBJECT_TAG tag);

	//Remove
	bool RemoveTag(GAME_OBJECT_TAG tag);
};


#endif // !OBJECT_H