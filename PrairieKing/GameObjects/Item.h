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
	};
protected:
	bool isSpawn = false;
	ItemTypes type;
	CowBoy* cowboy = nullptr;
	float spawnLimit = 10.f;
	float timer = 0.f;

public:
	static const int TotalTypes = 3;
	Item(const string& textureId = "", const string& n = "");
	virtual ~Item() override;

	void SetType(ItemTypes t);
	ItemTypes GetType() const;
	void SetPlayer(CowBoy* p);
	void SetIsSpawn(bool is);

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
};

