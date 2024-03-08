#include "pch.h"
#include "SceneManager.h"

std::unordered_map<std::string, std::shared_ptr<Scene>> SceneManager::usingSceneList;
std::unordered_map<std::string, std::shared_ptr<Scene>> SceneManager::unUseSceneList;
std::unordered_map<std::string, std::shared_ptr<Scene>> SceneManager::waitingSceneList;

void SceneManager::Resource()
{
	for (auto& scene : waitingSceneList)
	{
		scene.second->Resource();
	}
	SFGM_FONT.Load();
	SFGM_SOUNDBUFFER.Load();
	SFGM_TEXTURE.Load();
}

void SceneManager::Init()
{
	for (auto& scene : usingSceneList)
	{
		scene.second->Init();
	}
}

void SceneManager::PreUpdate(float timeDelta)
{
	for (auto& scene : usingSceneList)
	{
		scene.second->PreUpdate(timeDelta, scene.second->GetTimeScale());
	}
}

void SceneManager::Update(float timeDelta)
{
	for (auto& scene : usingSceneList)
	{
		scene.second->Update(timeDelta, scene.second->GetTimeScale());
	}
}

void SceneManager::PostUpdate(float timeDelta)
{
	for (auto& scene : usingSceneList)
	{
		scene.second->PostUpdate(timeDelta, scene.second->GetTimeScale());
	}
}

void SceneManager::PhygicsUpdate(float timeDelta)
{
	for (auto& scene : usingSceneList)
	{
		scene.second->PhygicsUpdate(timeDelta, scene.second->GetTimeScale());
	}
}

void SceneManager::Draw(sf::RenderWindow& window)
{
	for (auto& scene : usingSceneList)
	{
		scene.second->Draw(window);
	}
}

void SceneManager::DrawOnLayer(sf::RenderWindow& window)
{
}

void SceneManager::Use(const std::string& name)
{
	auto scene = waitingSceneList.find(name);
	if (scene != waitingSceneList.end())
	{
		usingSceneList.insert(std::make_pair(scene->first, scene->second));
		waitingSceneList.erase(name);
	}
}

void SceneManager::UnUse(const std::string& name)
{
	auto scene = usingSceneList.find(name);
	if (scene != usingSceneList.end())
	{
		/*SFGM_FONT.
		SFGM_SOUNDBUFFER.
		SFGM_TEXTURE.*/
		waitingSceneList.insert(std::make_pair(scene->first, scene->second));
		usingSceneList.erase(name);
	}
}

void SceneManager::Add(const std::shared_ptr<Scene>& scene)
{
	waitingSceneList.insert(std::make_pair(scene->GetSceneName(), scene));
}

void SceneManager::AddUse(const std::shared_ptr<Scene>& scene)
{
	usingSceneList.insert(std::make_pair(scene->GetSceneName(), scene));
}

const std::shared_ptr<Scene>& SceneManager::Get(const std::string& name)
{
	for (auto& scene : usingSceneList)
	{
		if (scene.first == name)
			return scene.second;
	}
	for (auto& scene : waitingSceneList)
	{
		if (scene.first == name)
			return scene.second;
	}
	return std::shared_ptr<Scene>(nullptr);
}

void SceneManager::Reset()
{
	for (auto& scene : usingSceneList)
	{
		scene.second->Reset();
	}
	for (auto& scene : waitingSceneList)
	{
		scene.second->Reset();
	}
}

void SceneManager::Release()
{
	for (auto& scene : usingSceneList)
	{
		scene.second->Release();
	}
	for (auto& scene : waitingSceneList)
	{
		scene.second->Release();
	}
	usingSceneList.clear();
	waitingSceneList.clear();
}
