#include "pch.h"
#include "rapidcsv.h"
#include "Animator.h"

Animator::Animator()
{
}

Animator::~Animator()
{
	currentClip = nullptr;
	target = nullptr;
}

void Animator::AddEvent(const std::string& clipId, int frame, std::function<void()> action)
{
	eventList.push_back({ clipId, frame, action });
}

void Animator::ClearEvent()
{
	eventList.clear();
}

void Animator::SetFrame(const AnimationFrame& frame)
{
	target->setTexture(frame.GetTexture());
	target->setTextureRect(frame.texCoord);
}

void Animator::Update(float dt, float timeScale)
{
	//첫프레임 부분에 대한 것이 부족하다.
	//개선 필요!
	//마지막 프레임에서 event호출이 두번 이뤄진다. 수정 필요!


	if (!isPlaying)
		return;

	accumTime += dt * speed * timeScale;
	if (accumTime < clipDuration)
		return;

	accumTime = 0.f;
	currentFrame += frameDirection;

	if (currentFrame == totalFrame)
	{
		if (!playQueue.empty())
		{
			Play(playQueue.front(), false);
			playQueue.pop();
			return;
		}
		switch (currentClip->loopType)
		{
		case AnimationLoopTypes::Single:
			currentFrame = totalFrame - 1;
			isPlaying = false;
			break;
		case AnimationLoopTypes::Loop:
			currentFrame = 0;
			break;
		case AnimationLoopTypes::PingPong:
			if (frameDirection > 0)
			{
				currentFrame = totalFrame - 2;
				frameDirection = -1;
				totalFrame = -1;
			}
			else if (frameDirection < 0)
			{
				currentFrame = totalFrame + 2;
				frameDirection = 1;
				totalFrame = currentClip->GetTotalFrame();
			}
			break;
		default:
			break;
		}
	}

	for (auto& event : eventList)
	{
		if (currentClip->id == event.clipId && currentFrame == event.frame)
		{
			if (event.action != nullptr)
				event.action();
		}
	}

	SetFrame(currentClip->frames[currentFrame]);
}

void Animator::PlayQueue(const std::string& clipId)
{
	playQueue.push(clipId);
	isPlaying = true;

}

void Animator::Play(const std::string& clipId, bool clearQueue)
{
	if (clearQueue)
	{
		while (!playQueue.empty())
		{
			playQueue.pop();
		}
	}

	isPlaying = true;
	accumTime = 0.f;

	currentClip = &SFGM_ANICLIP.Get(clipId);
	currentFrame = 0;
	totalFrame = currentClip->GetTotalFrame();
	frameDirection = 1;
	clipDuration = 1.f / currentClip->fps;
	SetFrame(currentClip->frames[0]);
}

void Animator::Pause()
{
	isPlaying = false;
}

void Animator::Stop()
{
	isPlaying = false;
}

bool AnimationClip::loadFromFile(const std::string& filePath)
{
	const rapidcsv::Document& doc = SFGM_CSVFILE.Get(filePath).GetDocument();

	id = doc.GetCell<std::string>(0, 0);
	fps = doc.GetCell<int>(1, 0);
	loopType = AnimationLoopTypes(doc.GetCell<int>(2, 0));

	for (int i = 3; i < doc.GetRowCount(); i++)
	{
		auto row = doc.GetRow<std::string>(i);
		frames.push_back({ row[0], { std::stoi(row[1]), std::stoi(row[2]), std::stoi(row[3]), std::stoi(row[4]) } });
	}

	return true;
}
