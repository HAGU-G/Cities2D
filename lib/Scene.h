#pragma once

namespace sfgm {

	class Scene
	{
	private:
		static unsigned long long scenesCount;

	protected:
		std::map<std::string, std::deque<std::shared_ptr<Object>>> objects;

		Scene();
		virtual ~Scene();
		Scene(const Scene&) = delete;
		Scene(Scene&&) = delete;
		Scene& operator=(const Scene&) = delete;
		Scene& operator=(Scene&&) = delete;

	public:

		virtual void Init();
		virtual void Update(float timeDelta, float timeScale);
		virtual void Draw(sf::RenderWindow& window);

		//Set


		//Get
		inline static const auto GetScenesCount() { return scenesCount; };
		inline const auto& GetObjects() const { return objects; };

		//objects
		bool AddObject(const std::shared_ptr<Object>& object);
		bool DeleteObject(const std::shared_ptr<Object>& object);
		bool DeleteObject(const std::string& key);

	};

}//namespace sfgm