#pragma once
#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#define SFGM_TEXTURE (ResourceManager<sf::Texture>::Instance())
#define SFGM_FONT (ResourceManager<sf::Font>::Instance())
#define SFGM_SOUNDBUFFER (ResourceManager<sf::SoundBuffer>::Instance())

#include "Scene.h"

//sf::Texture, sf::Font, sf::SoundBuffer
template<typename T>
class ResourceManager final
{
private:
	T unknownResource;
	std::set<std::string> resourcePathList;
	std::unordered_map<std::string, std::shared_ptr<T>> resourceObjectList;

	ResourceManager()
	{
		unknownResource.loadFromFile("unknown.png");
	}

public:
	~ResourceManager()
	{
		resourcePathList.clear();
		resourceObjectList.clear();
	}

	//���ҽ� ��� �߰�. �ߺ� ����.
	bool Add(const std::string& path)
	{
		return resourcePathList.insert(path).second;
	}
	size_t Add(const std::set<std::string>& pathList)
	{
		size_t count = 0;
		if (pathList.empty())
			return count;

		for (auto& path : pathList)
		{
			if (Add(path))
				count++;
		}
		return count;
	}
	size_t Add(std::shared_ptr<Scene> scene)
	{
		return Add(scene->GetResourcePathList());
	}

	//resourcePathList�� resourceObjectList�� ���� ��ε�� ����
	size_t AddFromLoaded()
	{
		RemoveAll();
		if (resourceObjectList.empty())
			return 0;
		for (auto& pair : resourceObjectList)
		{
			resourcePathList.insert(pair.first);
		}
		return resourcePathList.size();
	}

	//���ҽ� ��� ����
	void Remove(const std::string& path)
	{
		if (resourcePathList.empty())
			return;
		resourcePathList.erase(path);
	}

	//�̻������ ���ҽ� ��� ����
	size_t Remove()
	{
		size_t count = 0;
		if (resourcePathList.empty())
			return count;
		if (resourceObjectList.empty())
			return RemoveAll();

		auto it = resourcePathList.begin();
		while (it != resourcePathList.end())
		{
			if (resourceObjectList.find(*it) == resourceObjectList.end())
			{
				it = resourcePathList.erase(it);
				count++;
			}
		}
		return count;
	}

	//���ҽ� ��� ��� ����. ������ ���� ��ȯ.
	size_t RemoveAll()
	{
		size_t count = 0;
		if (resourcePathList.empty())
			return count;

		count = resourcePathList.size();
		resourcePathList.clear();
		return count;
	}


	//���ҽ� ������Ʈ �߰�. �߰��� ���� ��ȯ. �ߺ� ����.
	size_t Load()
	{
		size_t count = 0;
		if (resourcePathList.empty())
			return count;

		for (auto& path : resourcePathList)
		{
			if (resourceObjectList.find(path) != resourceObjectList.end())
				continue;
			std::unique_ptr<T> tempObject = std::make_unique<T>();
			if (tempObject->loadFromFile(path))
			{
				resourceObjectList.insert(std::make_pair(path, tempObject.release()));
				count++;
			}
		}
		return count;
	}

	//���ҽ� ������Ʈ �߰�. �߰��� ���ҽ� ������Ʈ ��ȯ.
	const T& Load(const std::string& resourcePath)
	{
		std::unique_ptr<T> tempObject = std::make_unique<T>();
		if (tempObject->loadFromFile(resourcePath))
		{
			return *resourceObjectList.insert(std::make_pair(resourcePath, tempObject.release())).first->second;
		}
		return unknownResource;
	}

	//resourcePathList�� ���� ��ΰ� ���� ���ҽ� ������Ʈ ����. ������ ���� ��ȯ.
	size_t UnLoad()
	{
		size_t count = 0;
		if (resourceObjectList.empty())
			return count;
		if (resourcePathList.empty())
			return UnLoadALL();

		auto it = resourceObjectList.begin();
		while (it != resourceObjectList.end())
		{
			if (resourcePathList.find(it->first) == resourcePathList.end())
			{
				it = resourceObjectList.erase(it);
				count++;
			}
		}
		return count;
	}

	//���ҽ� ������Ʈ ��� ����. ������ ���� ��ȯ.
	size_t UnLoadALL()
	{
		size_t count = resourceObjectList.size();
		resourceObjectList.clear();
		return count;
	}

	//Unload �� Load. ����/�߰��� ���� ��ȯ.
	std::pair<size_t, size_t> ReLoad()
	{
		return std::make_pair(UnLoad(), Load());
	}

	//UnloadALL �� Load. ����/�߰��� ���� ��ȯ.
	std::pair<size_t, size_t> ReLoadALL()
	{
		return std::make_pair(UnLoadALL(), Load());
	}

	//�߰��� ���ҽ� ������Ʈ Get
	const T& Get(const std::string& path)
	{
		auto it = resourceObjectList.find(path);
		if (it == resourceObjectList.end())
		{
			return unknownResource;
		}
		return *(it->second);
	}

	//��ο� ������Ʈ ��� ����. ������ ���/������Ʈ ���� ��ȯ.
	std::pair<size_t, size_t> Reset()
	{
		return std::make_pair(UnLoadALL(), RemoveAll());
	}

	//�̱���
	static ResourceManager<T>& Instance()
	{
		static ResourceManager<T> instance;
		return instance;
	}
};




#endif // !RESOURCEMANAGER_H