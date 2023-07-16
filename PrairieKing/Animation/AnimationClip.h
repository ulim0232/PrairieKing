#pragma once

enum class AnimationLoopTypes
{
	Single,
	Loop,
};

struct AnimationFrame
{
	std::string textureId;
	sf::IntRect texCoord;
	std::function<void()> action;// = nullptr; //특정한 프레임이 시작할 때 호출할 함수
};

struct AnimationClip
{
	std::string id; //애니메이션 클립의 id
	std::vector<AnimationFrame> frames;
	enum class AnimationLoopTypes loopType;
	int fps; //frame을 바꾸는 속도

	bool LoadFromFile(const string& path);
};