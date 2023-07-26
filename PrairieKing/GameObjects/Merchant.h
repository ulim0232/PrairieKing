#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"

class TileMap;
class CowBoy;

class Merchant :public SpriteGo
{
protected:
	float speed = 200.f;
	sf::Vector2f direction;

	AnimationController animation;

	sf::Texture* left = nullptr;
	sf::Texture* right = nullptr;
	sf::Texture* front = nullptr;

	TileMap* tileMap;
	sf::Vector2f tileMapLT;
	sf::Vector2f tileSize;

	sf::RectangleShape hitBox;

	CowBoy* cowboy = nullptr;

public:
	bool isArrival = false;
	Merchant(const string & n = "")
		:SpriteGo("", n) {}
	virtual ~Merchant() override { Release(); }

	virtual void Init() override; //동적 할당(생성자)
	virtual void Reset() override;

	virtual void Update(float dt) override;

	void SetTileMap(TileMap* map);
	void SetCowboy(CowBoy* p);
	void SetSpawnPosition();
};

