#pragma once
#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

class Scene;

static class SceneManager final
{
private:
	static std::unordered_map<std::string,std::shared_ptr<Scene>> usingSceneList;
	static std::unordered_map<std::string,std::shared_ptr<Scene>> waitingSceneList;

public:

	//Management
	static void Init();
	static void PreUpdate(float timeDelta); //Using
	static void Update(float timeDelta); //Using
	static void PhygicsUpdate(float timeDelta); //Using

	//Draw - sceneList�� �߰��� ������� Draw. Scene�ȿ����� ���ӿ�����Ʈ�� ���̾� ������� Draw
	static void Draw(sf::RenderWindow& window); //Using
	//DrawOnLayer - ����ϰ� �ִ� Scene�� ������Ʈ���� ���̾� ������� Draw.
	static void DrawOnLayer(sf::RenderWindow& window); //Using
	static void Reset();
	static void Release();

	//SceneList
	static void Use(const std::string& name);
	static void UnUse(const std::string& name);
	static void Add(const std::shared_ptr<Scene>& scene);
	static void AddUse(const std::shared_ptr<Scene>& scene);
	static const std::shared_ptr<Scene>& Get(const std::string& name);
};



#endif // !SCENEMANAGER_H