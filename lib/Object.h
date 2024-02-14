#pragma once
namespace sfgm {


	class Object //��ü������ �����Ҵ� ����
	{
	private:
		static unsigned long long objectsCount;

	protected:
		std::string key;
		float x;
		float y;
		float z;
		int layer;

		std::list<OBJECTTAG> objectTags;

		//pre_***()�� ���� ȣ��
		virtual void Init() = 0;
		virtual void Update(float timeDelta, float timeScale) = 0;
		virtual void Draw() = 0;

	public:
		explicit Object(std::string objectType);
		virtual ~Object();

		Object(const Object&) = delete;
		Object(Object&&) = delete;
		Object& operator=(const Object&) = delete;
		Object& operator=(Object&&) = delete;

		void pre_Init();
		void pre_Update(float timeDelta, float timeScale);
		void pre_Draw();


		//Get

		inline static const auto GetObjectsCount() { return objectsCount; };
		inline const auto& GetKey() const { return key; };
		inline const auto& GetObjectTags() const { return objectTags; };


		//Set

		bool AddTag(OBJECTTAG tag);
		bool DeleteTag(OBJECTTAG tag);
	};
}