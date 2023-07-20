#pragma once
#include "SpriteGo.h"

class CowBoy;

class Item : public SpriteGo
{
public:
	enum class ItemTypes
	{
		Coin,
		Life,
		Coffee,
		Shotgun,

	};
protected:
	bool isSpawn = false;
	ItemTypes type;
	CowBoy* cowboy = nullptr;
	float spawnLimit = 10.f;
	float timer = 0.f;

	float blinkDuration;
	float blinkTimer;

	sf::Clock clock;
	const sf::Time blinkTime = sf::seconds(0.2f);
	bool blinkTimeCheck = false;

public:
	static const int TotalTypes = 4;
	Item(const string& textureId = "", const string& n = "");
	virtual ~Item() override;

	void SetType(ItemTypes t);
	ItemTypes GetType() const;
	void SetPlayer(CowBoy* p);
	void SetIsSpawn(bool is);
	void blink();

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
};

