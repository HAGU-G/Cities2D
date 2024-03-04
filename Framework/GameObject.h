#pragma once
#ifndef OBJECT_H
#define OBJECT_H


class Scene;

class GameObject
{
private:
	static size_t currentCount;
	static size_t totalCount;

	const std::string key; //"gameObjectType_CreatedTime_totalCount"
	GAME_OBJECT_TYPE gameObjectType;

protected:
	std::weak_ptr<Scene> scene;
	std::list<GAME_OBJECT_TAG> gameObjectTagList;

	int drawLayer = 0;
	int phygicsLayer = 0;

	sf::Vector2f position;

	explicit GameObject(const std::shared_ptr<Scene>& scene, GAME_OBJECT_TYPE objectType);
	GameObject(const GameObject&) = delete;
	GameObject(GameObject&&) = delete;
	GameObject& operator=(const GameObject&) = delete;
	GameObject& operator=(GameObject&&) = delete;

public:
	virtual ~GameObject();

	//Management
	virtual void Init(); //empty
	virtual void Update(float timeDelta, float timeScale); //empty
	virtual void PhygicsUpdate(float timeDelta, float timeScale); //empty
	virtual void Draw(sf::RenderWindow& window); //empty
	virtual void Reset(); //empty
	virtual void Release();

	//Set
	void SetScene(const std::shared_ptr<Scene>& scene);
	void SetPositionX(float x);
	void SetPositionY(float y);
	void SetPosition(float x, float y);
	void SetPositionX(sf::Vector2f position);
	void SetPositionY(sf::Vector2f position);
	void SetPosition(sf::Vector2f position);
	void SetDrawLayer(int value);
	void SetPhygicsLayer(int value);

	//Get
	static size_t GetObjectsCount();
	static size_t GetObjectsTotalCount();
	std::string GetKey() const;
	const GAME_OBJECT_TYPE& GetGameObjectType() const;
	const std::list<GAME_OBJECT_TAG>& GetGameObjectTagList() const;

	const int& GetDrawLayer() const;
	const int& GetPhygicsLayer() const;

	const sf::Vector2f& GetPosition() const;

	//Add
	bool AddTag(GAME_OBJECT_TAG tag);

	//Remove
	bool RemoveTag(GAME_OBJECT_TAG tag);
};


#endif // !OBJECT_H