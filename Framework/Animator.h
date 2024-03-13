#pragma once
#include <queue>
#include <functional>

enum class AnimationLoopTypes
{
	Single,
	Loop,
	PingPong,
};

struct AnimationFrame
{
	std::string textureId;
	sf::IntRect texCoord;
	AnimationFrame(const std::string& id, const sf::IntRect& coord)
		:textureId(id), texCoord(coord)
	{

	}

	const sf::Texture& GetTexture() const
	{
		return SFGM_TEXTURE.Get(textureId);
	}
};

struct AnimationClip
{
	std::string id;
	int fps = 30;
	AnimationLoopTypes loopType = AnimationLoopTypes::Single;
	std::vector<AnimationFrame> frames;

	int GetTotalFrame() const
	{
		return frames.size();
	}
	bool loadFromFile(const std::string& filePath);
};

struct AnimationEvent
{
	std::string clipId;
	int frame;
	std::function<void()> action;
};

class Animator
{
protected:
	std::queue<std::string> playQueue;
	std::list<AnimationEvent> eventList;

	float speed = 1.f;

	bool isPlaying = false;
	
	const AnimationClip* currentClip = nullptr;
	int totalFrame = 0;
	int currentFrame = -1;
	int frameDirection = 1;
	
	float clipDuration = 0.f;

	float accumTime =0.f;

	sf::Sprite* target = nullptr;

public:
	Animator();
	~Animator();
	Animator(Animator&&) = delete;
	Animator(const Animator&) = delete;

	void AddEvent(const std::string& clipId, int frame, std::function<void()> action);
	void ClearEvent();

	void SetSpeed(float s) { speed = s; }
	void SetFrame(const AnimationFrame& frame);
	void SetTarget(sf::Sprite* t) { target = t; }

	bool IsPlaying() const { return isPlaying; }
	float GetSpeed() const { return speed; }
	sf::Sprite* GetTarget(sf::Sprite* t) const { return target; }
	const std::string& GetCurrentClip() const { return currentClip->id; }

	void Update(float dt, float timeScale);
	void Play(const std::string& clipId, bool clearQueue = true);
	void PlayQueue(const std::string& clipId);
	void Pause();
	void Stop();
};

