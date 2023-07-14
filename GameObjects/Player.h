#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"

class Player :public SpriteGo
{
protected:
	AnimationController animation;

	float speed = 200.f;

	sf::Vector2f look; //바라보는 방향
	sf::Vector2f direction;
	sf::Vector2f velocity; //dir*dt

	

public:
	Player(const std::string& textureId = "", const std::string& n = "")
		:SpriteGo(textureId, n) {}
	virtual ~Player() override { Release(); }

	virtual void Init() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
};

