#pragma once
/**/
class AnimationController
{
protected:
	std::unordered_map<std::string, AnimationClip> clips; //클립의 id, 클립
	std::queue<std::string> queue;
	
	float speed = 1.f; //애니메이션 재생 속도, 2면 2배 빠르게

	AnimationClip* currentClip = nullptr; //현재 재생하고 있는 클립의 포인터
	int currentFrame = -1; //지금 재생하는 클립의 재생중인 몇 번째 프래임을 
	int totalFrame = 0;
	float clipDuration = 0.f; //클립을 재생할 시간
	float accumTime = 0.f;
	
	bool isPlaying = false;

	sf::Sprite* target = nullptr; //애니메이션이 재생될 스프라이트 객체

public:
	void AddClip(const AnimationClip& newClip);
	void SetTarget(sf::Sprite* sprite) { target = sprite; }
	sf::Sprite* GetTarget() const { return target; }
	bool IsPlaying() { return isPlaying; }
	void SetSpeed(float s) { speed = s; }
	float GetSpeed() { return speed;  }

	void Update(float dt);

	void Play(const std::string& clipid, bool clearQueue = true);
	void PlayQueue(const std::string& clipid); //queue에 담는 용도로 사용
	void Stop();
	
	string GetCurrentClipId();
	void SetFrame(const AnimationFrame& frame);
};

