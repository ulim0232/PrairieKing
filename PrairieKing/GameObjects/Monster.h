#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"
#include "AnimationController.h"

class CowBoy;
class TileMap;
class Monster : public SpriteGo
{
public:
	enum class Types
	{
		Boss,
		Imp,
		Mirra,
		Mushroom,
		Ogre,
		Orc,
		Pixy
	};
	static const int TotalTypes = 6;

protected:
	Types monsterType;
	sf::Vector2i boxSize = { 28.f, 28.f };
	sf::RectangleShape hitBox;
	sf::Vector2f velocity;
	sf::Vector2f direction; //이동하는 방향

	float speed = 0.f; //이동 속도
	int maxHp = 0;
	int hp = 0;
	string idleClipPath;
	string dieClipPath;
	string bombClipPath;
	bool isAlive;

	float duration = 3.f; //죽으면 3초 후 삭제
	float timer = 0.f;

	CowBoy* cowboy = nullptr; //플레이어 추적에 사용
	ObjectPool<Monster>* pool = nullptr;

	AnimationController animation;
	TileMap* tileMap;
	int tileWidth = 0;
	//sf::Vector2i tileSize = { 80, 45 };
	sf::Vector2i tileSize = { 32, 32 };
	sf::FloatRect intersection;
	sf::Vector2f tileMapLT;
public:
	Monster(const string& n = "");
	virtual ~Monster() override;

	virtual void Init() override; //동적 할당(생성자)
	virtual void Release() override; //동적 할당 해제(소멸자)
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetPool(ObjectPool<Monster>* pool) { this->pool = pool; }

	void SetType(Types t);
	Types GetType() const;

	void SetCowboy(CowBoy* p);
	bool GetIsAllive();
	void OnHitBullet(int damage); //총알에 맞았을 때
	void OnDie(); //죽을 때
	void OnBoom();
	void SetTileMap(TileMap* map, int width);
	bool IsCollisoinTile(int index);

};

