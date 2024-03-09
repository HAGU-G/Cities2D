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

public:

	//Management
	static void Init(); //usingList
	static void PreUpdate(float timeDelta); //usingList
	static void Update(float timeDelta); //usingList
	static void PostUpdate(float timeDelta); //usingList
	static void PhygicsUpdate(float timeDelta); //usingList

	//Draw - sceneList�� �߰��� ������� Draw. Scene�ȿ����� ���ӿ�����Ʈ�� ���̾� ������� Draw
	static void Draw(sf::RenderWindow& window); //usingList
	//DrawOnLayer - ����ϰ� �ִ� Scene�� ������Ʈ���� ���̾� ������� Draw.
	static void DrawOnLayer(sf::RenderWindow& window); //usingList
	static void Reset(); //allList
	static void Release(); //allList

	//SceneList
	static void Use(const std::string& name); //wait to use, LoadResource ȣ�� �ʿ�
	static void Wait(const std::string& name); //to wait
	static void Unuse(const std::string& name); //to unuse

	static void Add(const std::shared_ptr<Scene>& scene);

	static const std::shared_ptr<Scene>& Get(const std::string& name);
};



#endif // !SCENEMANAGER_H