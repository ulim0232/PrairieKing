#pragma once
#include "Scene.h"

class SpriteGo;
class SceneClear : public Scene
{
	sf::Sound clearBgm;

	sf::Vector2f centerPos;

	SpriteGo* cowboy;
	SpriteGo* clearHeart;
	SpriteGo* EndingIcon;

	float timer = 0.f;

public:
	SceneClear();
	virtual ~SceneClear() override = default;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
};

