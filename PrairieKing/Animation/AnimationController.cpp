#include "stdafx.h"
#include "AnimationController.h"
#include "ResourceMgr.h"

void AnimationController::AddClip(const AnimationClip& newClip)
{
	if (clips.find(newClip.id) == clips.end()) //없는 경우 end 리턴함
	{
		clips.insert({ newClip.id, newClip });
	}
}

void AnimationController::Update(float dt)
{
	if (!isPlaying)
		return;

	accumTime += dt * speed; //speed에 따라서 재생 속도 적용 가능
	if (accumTime < clipDuration)
		return;

	//clip 변경
	accumTime = 0;
	currentFrame++;

	if (currentFrame == totalFrame) //마지막 프레임까지 재생되었을 때
	{
		if (!queue.empty())
		{
			std::string id = queue.front(); //제일 먼저 담긴 것
			queue.pop();
			Play(id, false); //true이면 queue를 비우기 때문에 false
			return;
		}
		else
		{
			switch (currentClip->loopType)
			{
			case AnimationLoopTypes::Single:
				currentFrame = totalFrame - 1; //마지막 프레임 유지
				return;
			case AnimationLoopTypes::Loop:
				currentFrame = 0; //첫 프레임으로 돌아가기
				break;
			}
		}
	}
	
	if (currentClip->frames[currentFrame].action != nullptr) 
		//참조하고 있지 않으면 nullptr리턴함
	{
		currentClip->frames[currentFrame].action(); 
		//펑터, 객체를 함수처럼 사용할 수 있다
	}

	if (target != nullptr)
	{
		//texture 교체
		SetFrame(currentClip->frames[currentFrame]);
	}
}

void AnimationController::Play(const std::string& clipid, bool clearQueue)
{
	if (clearQueue)
	{
		while(!queue.empty())
		{
			queue.pop();
		}
	}
	auto find = clips.find(clipid);
	if (find == clips.end()) //오류, 존재 하지 않을 경우 find 함수는 end리턴
	{
		Stop();
		std::cout << "ERR:: NOT EXIST CLIP" << std::endl;
		return;
	}

	isPlaying = true;
	currentClip = &find->second;
	currentFrame = 0;
	totalFrame = (int)currentClip->frames.size();
	clipDuration = 1.f / currentClip->fps;
	accumTime = 0.f;

	SetFrame(currentClip->frames[currentFrame]);
}

void AnimationController::PlayQueue(const std::string& clipid)
{
	queue.push(clipid);
}

void AnimationController::Stop()
{
	isPlaying = false;
}

string AnimationController::GetCurrentClipId()
{
	if (currentClip == nullptr)
	{
		return "";
	}
	return currentClip->id;
}

void AnimationController::SetFrame(const AnimationFrame& frame)
{
	sf::Texture* tex = RESOURCE_MGR.GetTexture(frame.textureId);

	target->setTexture(*tex);
	target->setTextureRect(frame.texCoord);
}
