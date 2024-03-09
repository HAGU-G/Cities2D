#include "pch.h"
#include "SceneManager.h"

std::unordered_map<std::string, std::shared_ptr<Scene>> SceneManager::usingSceneList;
std::unordered_map<std::string, std::shared_ptr<Scene>> SceneManager::unuseSceneList;
std::unordered_map<std::string, std::shared_ptr<Scene>> SceneManager::waitingSceneList;

void SceneManager::Init()
{
	for (auto& scene : usingSceneList)
	{
		scene.second->Init();
	}
	for (auto& scene : waitingSceneList)
	{
		scene.second->Init();
	}
	for (auto& scene : unuseSceneList)
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
	else
	{
		auto scene = unuseSceneList.find(name);
		if (scene != unuseSceneList.end())
		{
			Wait(name);
			Use(name);
		}
	}
}

void SceneManager::Wait(const std::string& name)
{
	auto usingScene = usingSceneList.find(name);
	if (usingScene != usingSceneList.end())
	{
		waitingSceneList.insert(std::make_pair(usingScene->first, usingScene->second));
		usingSceneList.erase(name);
	}
	else
	{
		auto unuseScene = unuseSceneList.find(name);
		if (unuseScene != unuseSceneList.end())
		{
			waitingSceneList.insert(std::make_pair(unuseScene->first, unuseScene->second));
			unuseScene->second->AddResource();
			unuseSceneList.erase(name);
		}
	}
	SFGM_TEXTURE.Load();
	SFGM_FONT.Load();
	SFGM_SOUNDBUFFER.Load();
	SFGM_CSVFILE.Load();
}

void SceneManager::Unuse(const std::string& name)
{
	auto usingScene = usingSceneList.find(name);
	if (usingScene != usingSceneList.end())
	{
		unuseSceneList.insert(std::make_pair(usingScene->first, usingScene->second));
		usingScene->second->RemoveResource();
		usingSceneList.erase(name);
	}
	else
	{
		auto watingScene = waitingSceneList.find(name);
		if (watingScene != waitingSceneList.end())
		{
			unuseSceneList.insert(std::make_pair(watingScene->first, watingScene->second));
			watingScene->second->RemoveResource();
			waitingSceneList.erase(name);
		}
	}
	SFGM_TEXTURE.UnLoad();
	SFGM_FONT.UnLoad();
	SFGM_SOUNDBUFFER.UnLoad();
	SFGM_CSVFILE.UnLoad();
}

void SceneManager::Add(const std::shared_ptr<Scene>& scene)
{
	unuseSceneList.insert(std::make_pair(scene->GetSceneName(), scene));
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
	for (auto& scene : unuseSceneList)
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
	for (auto& scene : unuseSceneList)
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
	for (auto& scene : unuseSceneList)
	{
		scene.second->Release();
	}
	usingSceneList.clear();
	waitingSceneList.clear();
	unuseSceneList.clear();
}
