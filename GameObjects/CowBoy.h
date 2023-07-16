#pragma once
#include "GameObject.h"
#include "AnimationController.h"

class SpriteGo;

class CowBoy : public GameObject
{
protected:
	AnimationController legAnimation;

	float gapLegToHead = 4.f;

	float speed = 150.f;

	sf::Vector2f look; //바라보는 방향
	sf::Vector2f direction;
	sf::Vector2f velocity; //dir*dt

	sf::Texture* left = nullptr;
	sf::Texture* right = nullptr;
	sf::Texture* back = nullptr;
	sf::Texture* front = nullptr;
	sf::Texture* origintex = nullptr;

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

