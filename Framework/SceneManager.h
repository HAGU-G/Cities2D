#pragma once
#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H






class Scene;

static class SceneManager final
{
private:
	static std::vector<std::shared_ptr<Scene>> UsingSceneList;
	static std::vector<std::shared_ptr<Scene>> WaitingSceneList;

public:

	//Management
	static void Init();
	static void Update(float timeDelta, float globalTimeScale); //Using
	static void PhygicsUpdate(float timeDelta, float globalTimeScale); //Using

	//Draw - sceneList에 추가된 순서대로 Draw. Scene안에서는 게임오브젝트의 레이어 순서대로 Draw
	static void Draw(sf::RenderWindow& window); //Using
	//DrawOnLayer - 사용하고 있는 Scene의 오브젝트들을 레이어 순서대로 Draw.
	static void DrawOnLayer(sf::RenderWindow& window); //Using
	static void Reset();
	static void Release();

	//SceneList
	static void Use(const std::string& name);
	static void UnUse(const std::string& name);
	static const std::shared_ptr<Scene>& Get(const std::string& name);
};



#endif // !SCENEMANAGER_H