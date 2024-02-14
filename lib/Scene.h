#pragma once
namespace sfgm {

	class Scene
	{
	private:
		static unsigned long long scenesCount;

	protected:
		std::map<std::string, std::list<std::shared_ptr<Object>>> objects;

		//pre_***()를 통해 호출
		virtual void Init() = 0;
		virtual void Update(float timeDelta, float timeScale) = 0;
		virtual void Draw() = 0;

	public:
		Scene();
		virtual ~Scene();

		Scene(const Scene&) = delete;
		Scene(Scene&&) = delete;
		Scene& operator=(const Scene&) = delete;
		Scene& operator=(Scene&&) = delete;

		void pre_Init();
		void pre_Update(float timeDelta, float timeScale);
		void pre_Draw();


		//Get
		inline static const auto GetScenesCount() { return scenesCount; };
		inline const auto& GetObjects() const { return objects; };

		//Set
		bool AddObject(const std::shared_ptr<Object> object);
		bool DeleteObject(const std::shared_ptr<Object>& object);

	};

}