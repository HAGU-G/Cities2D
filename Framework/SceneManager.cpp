#include "pch.h"
#include "SceneManager.h"



std::vector<std::shared_ptr<Scene>> SceneManager::UsingSceneList;
std::vector<std::shared_ptr<Scene>> SceneManager::WaitingSceneList;

void SceneManager::Init()
{
	for (auto& ptr : UsingSceneList)
	{
		ptr->Init();
	}
}
void SceneManager::Update(float timeDelta, float globalTimeScale)
{
	for (auto& ptr : UsingSceneList)
	{
		ptr->Update(timeDelta, globalTimeScale);
	}
}
void SceneManager::PhygicsUpdate(float timeDelta, float globalTimeScale)
{
	for (auto& ptr : UsingSceneList)
	{
		ptr->PhygicsUpdate(timeDelta, globalTimeScale);
	}
}
void SceneManager::Draw(sf::RenderWindow& window)
{
	for (auto& ptr : UsingSceneList)
	{
		ptr->Draw(window);
	}
}
void SceneManager::DrawOnLayer(sf::RenderWindow& window)
{
}
void SceneManager::Use(const std::string& name)
{
}
void SceneManager::UnUse(const std::string& name)
{
}
const std::shared_ptr<Scene>& SceneManager::Get(const std::string& name)
{
	for (auto& ptr : UsingSceneList)
	{
		if (ptr->GetSceneName() == name)
			return ptr;
	}
	for (auto& ptr : WaitingSceneList)
	{
		if (ptr->GetSceneName() == name)
			return ptr;
	}
	return nullptr;
}
void SceneManager::Reset()
{
	for (auto& ptr : UsingSceneList)
	{
		ptr->Reset();
	}
	for (auto& ptr : WaitingSceneList)
	{
		ptr->Reset();
	}
}
void SceneManager::Release()
{
	for (auto& ptr : UsingSceneList)
	{
		ptr->Release();
	}
	for (auto& ptr : WaitingSceneList)
	{
		ptr->Release();
	}
	UsingSceneList.clear();
	WaitingSceneList.clear();
}
