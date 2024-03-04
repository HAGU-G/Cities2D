#include "pch.h"
#include "Scene.h"


size_t Scene::currentCount = 0;
size_t Scene::totalCount = 0;

Scene::Scene(const std::string& name)
	:name(name)
{
	currentCount++;
	totalCount++;
}

Scene::~Scene()
{
	currentCount--;
}

void Scene::Init()
{
	view.setSize(sf::Vector2f( GameManager::GetWindowSize().x, GameManager::GetWindowSize().y*2.f ));
	view.setCenter(worldCenter);

	for (auto& pair : gameObjectList)
	{
		pair.second->Init();
	}
	Reset();
}

void Scene::PreUpdate(float timeDelta, float timeScale)
{
	mousePosWorld = GameManager::GetWindow().mapPixelToCoords(GameManager::GetMousePosWindow(),view);
}

void Scene::Update(float timeDelta, float timeScale)
{
	for (auto& pair : gameObjectList)
	{
		pair.second->Update(timeDelta, timeScale);
	}
}

void Scene::PhygicsUpdate(float timeDelta, float timeScale)
{
	for (auto& pair : gameObjectList)
	{
		pair.second->PhygicsUpdate(timeDelta, timeScale);
	}
}

void Scene::Draw(sf::RenderWindow& window)
{
	const sf::View& preView = window.getView();
	window.setView(view);
	for (auto& pair : drawList)
	{
		pair.second->Draw(window);
	}
	window.setView(preView);
}

void Scene::Reset()
{
	for (auto& pair : gameObjectList)
	{
		pair.second->Reset();
	}
}

void Scene::Release()
{
	for (auto& pair : gameObjectList)
	{
		pair.second->Release();
	}
	gameObjectList.clear();
	drawList.clear();
	resourcePathList.clear(); //파일 경로들이 담긴 컨테이너
}

float Scene::GetTimeScale() const
{
	if (useGlobalTimeScale)
	{
		return GameManager::GetGlobalTimeScale();
	}
	else
	{
		return timeScale;
	}
}

size_t Scene::GetScenesCount()
{
	return currentCount;
}

size_t Scene::GetScenesTotalCount()
{
	return totalCount;
}

const std::string& Scene::GetSceneName() const
{
	return name;
}

const std::unordered_map<std::string, std::shared_ptr<GameObject>>& Scene::GetObjects() const
{
	return gameObjectList;
}

const FilePathList& Scene::GetResourcePathList() const
{
	return resourcePathList;
}


bool Scene::AddObject(const std::shared_ptr<GameObject>& object)
{
	auto it = gameObjectList.insert(std::make_pair(object->GetKey(), object));
	if (it.second)
	{
		if (drawList.empty())
			drawList.push_back(*it.first);
		else if (it.first->second->GetDrawLayer() >= drawList.front().second->GetDrawLayer())
			drawList.push_back(*it.first);
		else
			drawList.push_front(*it.first);
	}
	return it.second;
}

void Scene::ResetDrawList()
{
	drawList.clear();
	for (auto& pair : gameObjectList)
	{
		if (drawList.empty())
			drawList.push_back(pair);
		else if (pair.second->GetDrawLayer() >= drawList.front().second->GetDrawLayer())
			drawList.push_back(pair);
		else
			drawList.push_front(pair);
	}

}

const std::shared_ptr<GameObject>& Scene::GetObject(const std::string& key) const
{
	auto it = gameObjectList.find(key);
	if (it != gameObjectList.end())
		return it->second;
	return std::shared_ptr<GameObject>(nullptr);
}
const std::shared_ptr<GameObject>& Scene::GetObject(const std::shared_ptr<GameObject>& object) const
{
	return GetObject(object->GetKey());
}

bool Scene::DeleteObject(const std::string& key)
{
	auto it = gameObjectList.find(key);
	if (it != gameObjectList.end())
	{
		auto drawIt = drawList.begin();
		while (drawIt != drawList.end())
		{
			if (drawIt->first == it->first)
			{
				drawList.erase(drawIt);
				break;
			}
		}
		gameObjectList.erase(it);
		return true;
	}
	return false;
}


