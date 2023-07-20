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

	float gapLegToHead = 4.f;
	float speed = 150.f; //100
	int tileWidth = 0;
	bool rebound = false;
	float timer = 0.f;
	float shotDelay = 0.3f;

	sf::Texture* texture;

	sf::Vector2f look; //바라보는 방향
	sf::Vector2f direction;
	sf::Vector2f velocity; //dir*dt

	sf::Texture* left = nullptr;
	sf::Texture* right = nullptr;
	sf::Texture* back = nullptr;
	sf::Texture* front = nullptr;
	sf::Texture* origintex = nullptr;

	TileMap* tileMap;
	sf::RectangleShape hitBox;
	sf::RectangleShape tileBox;
	sf::Vector2i boxSize = { 28.f, 28.f };
	sf::Vector2i tileSize = { 32.f, 32.f };
	sf::FloatRect intersection;
	sf::Vector2f newPosition;

	ObjectPool<Bullet> poolBullets;

	bool iscoll = false;

	bool isRevive = false;
	float duration = 3.f;
	float timerA = 0.f;
	bool isDie = false;
	bool isSpeedUp = false;
	float timerI = 0.f;
	float itmeDuration = 5.f;

	
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

	void CowBoyDie();
	sf::RectangleShape GetHitBox();
	void SetIsRevive(bool is);
	void SetIsDie(bool is);

	void TakeItem(Item::ItemTypes type);
};

