#pragma once
namespace sfgm {


	class Object //객체생성은 동적할당 권장
	{
	private:
		static size_t objectsCount;

	protected:
		OBJECT_TYPE objectType;
		std::string key;
		std::list<OBJECT_TAG> objectTags;

		float x, y;
		int drawLayer = 0;

		explicit Object(OBJECT_TYPE objectType);
		virtual ~Object();
		Object(const Object&) = delete;
		Object(Object&&) = delete;
		Object& operator=(const Object&) = delete;
		Object& operator=(Object&&) = delete;

	public:

		//
		virtual void Init() = 0;
		virtual void Update(float timeDelta, float timeScale) = 0;
		virtual void Draw(sf::RenderWindow& window) = 0;

		//Set
		inline void SetX(float x) { this->x = x; }
		inline void SetY(float y) { this->y = y; }
		inline void SetXY(float x, float y) { this->x = x; this->y = y; }
		inline void SetX(sf::Vector2f vector2) { this->x = vector2.x; }
		inline void SetY(sf::Vector2f vector2) { this->y = vector2.y; }
		inline void SetXY(sf::Vector2f vector2) { this->x = vector2.x; this->y = vector2.y; }
		inline void SetDrawLayer(int value) { drawLayer = value; }
		
		//Get
		inline static size_t GetObjectsCount() { return objectsCount; }
		inline std::string GetKey() const { return key; }
		inline float GetX() const { return x; }
		inline float GetY() const { return y; }
		inline sf::Vector2f GetXY() const { return sf::Vector2f(x, y); }

		//objectTags
		bool AddTag(OBJECT_TAG tag);
		bool DeleteTag(OBJECT_TAG tag);
	};
}//namespace sfgm