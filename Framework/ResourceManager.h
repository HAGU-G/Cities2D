#pragma once
#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#define SFGM_TEXTURE (ResourceManager<sf::Texture>::Instance())
#define SFGM_FONT (ResourceManager<sf::Font>::Instance())
#define SFGM_SOUNDBUFFER (ResourceManager<sf::SoundBuffer>::Instance())
#define SFGM_CSVFILE (ResourceManager<CsvFile>::Instance())

#include "Scene.h"
#include "CsvFile.h"

//sf::Texture, sf::Font, sf::SoundBuffer, CsvFile
template<typename T>
class ResourceManager final
{
private:
	T unknownResource;
	FilePathList resourcePathList;
	std::unordered_map<std::string, std::shared_ptr<T>> resourceObjectList;

	ResourceManager()
	{
		if (typeid(T).name() == typeid(sf::Texture).name())
			unknownResource.loadFromFile("resource/Unknown.png");
		else if (typeid(T).name() == typeid(sf::Font).name())
			unknownResource.loadFromFile("resource/font/ROKAF Sans Medium.ttf");
		else if (typeid(T).name() == typeid(CsvFile).name())
			unknownResource.loadFromFile("data/Unknown.csv");
		//else if (typeid(T).name() == typeid(sf::SoundBuffer))
	}

public:
	~ResourceManager()
	{
		resourcePathList.clear();
		resourceObjectList.clear();
	}

	//리소스 경로 추가. 중복 제외.
	bool Add(const std::string& path)
	{
		return resourcePathList.insert(path).second;
	}
	size_t Add(const FilePathList& pathList)
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
	size_t Add(const std::shared_ptr<Scene>& scene)
	{
		return Add(scene->GetResourcePathList());
	}

	//resourcePathList를 resourceObjectList의 파일 경로들로 변경
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

	//리소스 경로 제거
	void Remove(const std::string& path)
	{
		if (resourcePathList.empty())
			return;
		resourcePathList.erase(path);
	}

	//미사용중인 리소스 경로 제거
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

	//리소스 경로 모두 제거. 제거한 개수 반환.
	size_t RemoveAll()
	{
		size_t count = 0;
		if (resourcePathList.empty())
			return count;

		count = resourcePathList.size();
		resourcePathList.clear();
		return count;
	}


	//리소스 오브젝트 추가. 추가한 개수 반환. 중복 제외.
	size_t Load()
	{
		size_t count = 0;
		if (resourcePathList.empty())
			return count;

		for (auto& path : resourcePathList)
		{
			if (resourceObjectList.find(path) != resourceObjectList.end())
				continue;
			std::shared_ptr<T> tempObject = std::make_unique<T>();
			if (tempObject->loadFromFile(path))
			{
				resourceObjectList.insert(std::make_pair(path, tempObject));
				count++;
			}
		}
		return count;
	}

	//리소스 오브젝트 추가. 추가된 리소스 오브젝트 반환.
	const T& Load(const std::string& resourcePath)
	{
		std::shared_ptr<T> tempObject = std::make_unique<T>();
		if (tempObject->loadFromFile(resourcePath))
		{
			return *resourceObjectList.insert(std::make_pair(resourcePath, tempObject)).first->second;
		}
		return unknownResource;
	}

	//resourcePathList에 파일 경로가 없는 리소스 오브젝트 제거. 제거한 개수 반환.
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

	//리소스 오브젝트 모두 제거. 제거한 개수 반환.
	size_t UnLoadALL()
	{
		size_t count = resourceObjectList.size();
		resourceObjectList.clear();
		return count;
	}

	//Unload → Load. 제거/추가한 개수 반환.
	std::pair<size_t, size_t> ReLoad()
	{
		return std::make_pair(UnLoad(), Load());
	}

	//UnloadALL → Load. 제거/추가한 개수 반환.
	std::pair<size_t, size_t> ReLoadALL()
	{
		return std::make_pair(UnLoadALL(), Load());
	}

	//추가된 리소스 오브젝트 Get
	const T& Get(const std::string& path)
	{
		auto it = resourceObjectList.find(path);
		if (it == resourceObjectList.end())
		{
			return unknownResource;
		}
		return *(it->second);
	}

	//경로와 오브젝트 모두 제거. 제거한 경로/오브젝트 개수 반환.
	std::pair<size_t, size_t> Reset()
	{
		return std::make_pair(UnLoadALL(), RemoveAll());
	}

	//싱글톤
	static ResourceManager<T>& Instance()
	{
		static ResourceManager<T> instance;
		return instance;
	}
};




#endif // !RESOURCEMANAGER_H