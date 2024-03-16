#pragma once
#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

class Scene;

class SceneManager final
{
private:
	static std::unordered_map<std::string,std::shared_ptr<Scene>> usingSceneList;
	static std::unordered_map<std::string,std::shared_ptr<Scene>> waitingSceneList;
	static std::unordered_map<std::string, std::shared_ptr<Scene>> unuseSceneList;

	static std::deque<std::string> toUse;
	static std::deque<std::string> toWait;
	static std::deque<std::string> toUnuse;


	//SceneList
	static void Use(); //wait to use
	static void Wait(); //to wait
	static void Unuse(); //to unuse

public:
	static bool canChange;

	//Management
	static void Init(); //usingList
	static void PreUpdate(float timeDelta); //usingList
	static void Update(float timeDelta); //usingList
	static void PostUpdate(float timeDelta); //usingList\

	//Draw - sceneList에 추가된 순서대로 Draw. Scene안에서는 게임오브젝트의 레이어 순서대로 Draw
	static void Draw(sf::RenderWindow& window); //usingList
	//DrawOnLayer - 사용하고 있는 Scene의 오브젝트들을 레이어 순서대로 Draw.
	static void Reset(); //allList
	static void Release(); //allList

	static void Use(const std::string& name, bool doForce = false); //wait to use
	static void Wait(const std::string& name, bool doForce = false); //to wait
	static void Unuse(const std::string& name, bool doForce = false); //to unuse

	static void Add(const std::shared_ptr<Scene>& scene);

	static const std::shared_ptr<Scene>& Get(const std::string& name);
};



#endif // !SCENEMANAGER_H