#include "pch.h"
#include "SceneManager.h"

std::unordered_map<std::string, std::shared_ptr<Scene>> SceneManager::usingSceneList;
std::unordered_map<std::string, std::shared_ptr<Scene>> SceneManager::unuseSceneList;
std::unordered_map<std::string, std::shared_ptr<Scene>> SceneManager::waitingSceneList;

bool SceneManager::canChange = true;
std::deque<std::string> SceneManager::toUse;
std::deque<std::string> SceneManager::toWait;
std::deque<std::string> SceneManager::toUnuse;

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
	if (canChange)
	{
		Use();
		Wait();
		Unuse();
	}
}

void SceneManager::PhysicsUpdate(float timeDelta)
{
	for (auto& scene : usingSceneList)
	{
		scene.second->PhysicsUpdate(timeDelta, scene.second->GetTimeScale());
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

void SceneManager::Use()
{
	std::string name;
	while (!toUse.empty())
	{
		name = toUse.front();
		toUse.pop_front();

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
				Wait(name,true);
				Use(name,true);
			}
		}
	}
}

void SceneManager::Wait()
{
	std::string name;
	while (!toWait.empty())
	{
		name = toWait.front();
		toWait.pop_front();

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

				SFGM_TEXTURE.Load();
				SFGM_FONT.Load();
				SFGM_SOUNDBUFFER.Load();
				SFGM_CSVFILE.Load();

				unuseScene->second->Init();
				unuseScene->second->Reset();
				unuseSceneList.erase(name);
			}
		}
	}
}

void SceneManager::Unuse()
{
	std::string name;
	bool doUnLoad = false;
	while (!toUnuse.empty())
	{
		name = toUnuse.front();
		toUnuse.pop_front();

		auto usingScene = usingSceneList.find(name);
		if (usingScene != usingSceneList.end())
		{
			unuseSceneList.insert(std::make_pair(usingScene->first, usingScene->second));
			usingScene->second->RemoveResource();
			usingScene->second->Release();
			usingSceneList.erase(name);
			doUnLoad = true;
		}
		else
		{
			auto watingScene = waitingSceneList.find(name);
			if (watingScene != waitingSceneList.end())
			{
				unuseSceneList.insert(std::make_pair(watingScene->first, watingScene->second));
				watingScene->second->RemoveResource();
				usingScene->second->Release();
				waitingSceneList.erase(name);
				doUnLoad = true;
			}
		}
	}
	if (doUnLoad)
	{
		for (auto& pair : usingSceneList)
		{
			pair.second->AddResource();
		}
		for (auto& pair : waitingSceneList)
		{
			pair.second->AddResource();
		}
		SFGM_TEXTURE.UnLoad();
		SFGM_FONT.UnLoad();
		SFGM_SOUNDBUFFER.UnLoad();
		SFGM_CSVFILE.UnLoad();
	}
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

void SceneManager::Use(const std::string& name, bool doForce)
{
	toUse.push_back(name);
	if (doForce)
		Use();
}

void SceneManager::Wait(const std::string& name, bool doForce)
{
	toWait.push_back(name);
	if (doForce)
		Wait();
}

void SceneManager::Unuse(const std::string& name, bool doForce)
{
	toUnuse.push_back(name);
	if (doForce)
		Unuse();
}
