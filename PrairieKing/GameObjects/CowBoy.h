#pragma once
#include "GameObject.h"
#include "AnimationController.h"
#include "TileMap.h"
#include "Bullet.h"
#include "Item.h"

class SpriteGo;

class CowBoy : public GameObject
{
protected:
	AnimationController legAnimation;
	AnimationController headAnimation;

	/*사운드*/
	sf::SoundBuffer footStepBuffer;
	sf::Sound footStep;

	/*--이동--*/
	sf::Vector2f newPosition;
	sf::Vector2f look; //바라보는 방향
	sf::Vector2f direction;
	sf::Vector2f velocity; //dir*dt

	/*--텍스쳐--*/
	sf::Texture* texture;
	sf::Texture* left = nullptr;
	sf::Texture* right = nullptr;
	sf::Texture* back = nullptr;
	sf::Texture* front = nullptr;
	sf::Texture* origintex = nullptr;

	/*--맵--*/
	TileMap* tileMap;
	sf::RectangleShape hitBox;
	sf::RectangleShape tileBox;
	sf::Vector2i boxSize = { 28.f, 28.f };
	sf::Vector2i tileSize = { 32.f, 32.f };
	sf::FloatRect intersection;
	
	ObjectPool<Bullet> poolBullets;

	bool iscoll = false;
	bool isDie = false;
	bool isSpeedUp = false; //커피
	bool isShotGun = false; //삿건
	bool isMachineGun = false; //머신건
	bool isWheel = false; // 바퀴
	bool rebound = false;

	float duration = 3.f;
	float timerCoffee = 0.f; //아이템 유지 시간
	float timerShot = 0.f;
	float timerMuchine = 0.f;
	float timerWheel = 0.f;
	float itmeDuration = 5.f; //아이템 유지 시간
	float gapLegToHead = 4.f;
	float speed = 150.f; //100
	float timer = 0.f; //연사 속도 조절에 사용
	float shotDelay = 0.3f;
	float shotSpeed = 400.f;

	/*샷건, 바퀴에 사용*/
	int bulletCount = 0;
	float angleStep = 0;
	float startAngle = 0;

	int tileWidth = 0;
	
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

	void SetTileMap(TileMap* map, int width);

	bool IsTileCollision();
	bool IsCollisoinTile(int index);

	void TakeItem(Item::ItemTypes type);

	void CowBoyDie();
	void SetIsDie(bool is);
	
	sf::RectangleShape GetHitBox();
	
};

