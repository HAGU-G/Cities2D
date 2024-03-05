#include "pch.h"
#include "SceneManager.h"

std::unordered_map<std::string, std::shared_ptr<Scene>> SceneManager::UsingSceneList;
std::unordered_map<std::string, std::shared_ptr<Scene>> SceneManager::WaitingSceneList;

void SceneManager::Init()
{
	for (auto& scene : UsingSceneList)
	{
		scene.second->Init();
	}
}

void SceneManager::PreUpdate(float timeDelta)
{
	for (auto& scene : UsingSceneList)
	{
		scene.second->PreUpdate(timeDelta, scene.second->GetTimeScale());
	}
}

void SceneManager::Update(float timeDelta)
{
	for (auto& scene : UsingSceneList)
	{
		scene.second->Update(timeDelta, scene.second->GetTimeScale());
	}
}

void SceneManager::PhygicsUpdate(float timeDelta)
{
	for (auto& scene : UsingSceneList)
	{
		scene.second->PhygicsUpdate(timeDelta, scene.second->GetTimeScale());
	}
}

void SceneManager::Draw(sf::RenderWindow& window)
{
	for (auto& scene : UsingSceneList)
	{
		scene.second->Draw(window);
	}
}

void SceneManager::DrawOnLayer(sf::RenderWindow& window)
{
}

void SceneManager::Use(const std::string& name)
{
	auto scene = WaitingSceneList.find(name);
	if (scene != WaitingSceneList.end())
	{
		UsingSceneList.insert(std::make_pair(scene->first, scene->second));
		WaitingSceneList.erase(name);
	}
}

void SceneManager::UnUse(const std::string& name)
{
	auto scene = UsingSceneList.find(name);
	if(scene != UsingSceneList.end())
	{
		WaitingSceneList.insert(std::make_pair(scene->first, scene->second));
		UsingSceneList.erase(name);
	}
}

void SceneManager::Add(const std::shared_ptr<Scene>& scene)
{
	WaitingSceneList.insert(std::make_pair(scene->GetSceneName(), scene));
}

void SceneManager::AddUse(const std::shared_ptr<Scene>& scene)
{
	UsingSceneList.insert(std::make_pair(scene->GetSceneName(), scene));
}

const std::shared_ptr<Scene>& SceneManager::Get(const std::string& name)
{
	for (auto& scene : UsingSceneList)
	{
		if (scene.first == name)
			return scene.second;
	}
	for (auto& scene : WaitingSceneList)
	{
		if (scene.first == name)
			return scene.second;
	}
	return std::shared_ptr<Scene>(nullptr);
}

void SceneManager::Reset()
{
	for (auto& scene : UsingSceneList)
	{
		scene.second->Reset();
	}
	for (auto& scene : WaitingSceneList)
	{
		scene.second->Reset();
	}
}

void SceneManager::Release()
{
	for (auto& scene : UsingSceneList)
	{
		scene.second->Release();
	}
	for (auto& scene : WaitingSceneList)
	{
		scene.second->Release();
	}
	UsingSceneList.clear();
	WaitingSceneList.clear();
}
