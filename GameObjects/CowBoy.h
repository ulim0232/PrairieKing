#pragma once
#include "GameObject.h"
#include "AnimationController.h"

class SpriteGo;

class CowBoy : public GameObject
{
protected:
	AnimationController legAnimation;

	float gapLegToHead = 4.f;

	float speed = 200.f;

	sf::Vector2f look; //�ٶ󺸴� ����
	sf::Vector2f direction;
	sf::Vector2f velocity; //dir*dt

public:
	sf::Sprite head;
	sf::Sprite leg;

	CowBoy(const std::string& n = "");
	virtual ~CowBoy() override;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	virtual void SetPosition(const sf::Vector2f& p);
	virtual void SetPosition(float x, float y);

	virtual void SetOrigin(Origins origin);
	virtual void SetOrigin(float x, float y);
};

