#include "pch.h"
#include "GameObject.h"


size_t GameObject::currentCount = 0;
size_t GameObject::totalCount = 0;

void GameObject::Setkey(const std::string& loadedKey)
{
	key = loadedKey;
}

GameObject::GameObject(std::weak_ptr<Scene> scene, GAME_OBJECT_TYPE objectType)
	:scene(scene), gameObjectType(objectType),
	key(std::to_string((int)objectType) + "_" + std::to_string(time(NULL)) + "_" + std::to_string(totalCount))
{
	currentCount++;
	totalCount++;
}

GameObject::~GameObject()
{
	currentCount--;
	Release();
}

bool GameObject::AddTag(GAME_OBJECT_TAG tag)
{
	if (std::find(gameObjectTagList.begin(), gameObjectTagList.end(), tag) == gameObjectTagList.end())
	{
		gameObjectTagList.push_back(tag);
		return true;
	}
	return false;
}

bool GameObject::RemoveTag(GAME_OBJECT_TAG tag)
{
	if (std::find(gameObjectTagList.begin(), gameObjectTagList.end(), tag) != gameObjectTagList.end())
	{
		gameObjectTagList.remove(tag);
		return true;
	}
	return false;
}



void GameObject::Init()
{
}

void GameObject::PreUpdate(float timeDelta, float timeScale)
{
}

void GameObject::Update(float timeDelta, float timeScale)
{
}

void GameObject::PostUpdate(float timeDelta, float timeScale)
{
}

void GameObject::PhygicsUpdate(float timeDelta, float timeScale)
{
}

void GameObject::Draw(sf::RenderWindow& window)
{
}

void GameObject::Reset()
{
}

void GameObject::Release()
{
	scene.reset();
}

void GameObject::SetScene(std::weak_ptr<Scene> scene)
{
	this->scene = scene;
}

void GameObject::SetPositionX(float x)
{
	position.x = x;
}

void GameObject::SetPositionY(float y)
{
	position.y = y;
}

void GameObject::SetPosition(const sf::Vector2f& position)
{
	this->position = position;
}

void GameObject::SetDrawLayer(int value)
{
	drawLayer = value;
}

void GameObject::SetPhygicsLayer(int value)
{
	drawLayer = value;
}

const std::shared_ptr<GameObject>& GameObject::This()
{
	return scene.lock()->GetObject(key);
}

size_t GameObject::GetObjectsCount()
{
	return currentCount;
}

size_t GameObject::GetObjectsTotalCount()
{
	return totalCount;
}

std::string GameObject::GetKey() const
{
	return key;
}

const GAME_OBJECT_TYPE& GameObject::GetGameObjectType() const
{
	return gameObjectType;
}

const std::list<GAME_OBJECT_TAG>& GameObject::GetGameObjectTagList() const
{
	return gameObjectTagList;
}

const int& GameObject::GetDrawLayer() const
{
	return drawLayer;
}

const int& GameObject::GetPhygicsLayer() const
{
	return phygicsLayer;
}

const sf::Vector2f& GameObject::GetPosition() const
{
	return position;
}

