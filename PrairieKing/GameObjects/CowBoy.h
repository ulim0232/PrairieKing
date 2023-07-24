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

	/*����*/
	sf::SoundBuffer footStepBuffer;
	sf::Sound footStep;

	/*--�̵�--*/
	sf::Vector2f newPosition;
	sf::Vector2f look; //�ٶ󺸴� ����
	sf::Vector2f direction;
	sf::Vector2f velocity; //dir*dt

	/*--�ؽ���--*/
	sf::Texture* texture;
	sf::Texture* left = nullptr;
	sf::Texture* right = nullptr;
	sf::Texture* back = nullptr;
	sf::Texture* front = nullptr;
	sf::Texture* origintex = nullptr;

	/*--��--*/
	TileMap* tileMap;
	sf::RectangleShape hitBox;
	sf::RectangleShape tileBox;
	sf::Vector2i boxSize = { 28.f, 28.f };
	sf::Vector2i tileSize = { 32.f, 32.f };

	//sf::Vector2i boxSize = { 70, 39.375 };
	//sf::Vector2i tileSize = { 80, 45 };

	sf::FloatRect intersection;
	
	ObjectPool<Bullet> poolBullets;

	bool iscoll = false;
	bool isDie = false;
	bool isSpeedUp = false; //Ŀ��
	bool isShotGun = false; //���
	bool isMachineGun = false; //�ӽŰ�
	bool isWheel = false; // ����
	bool rebound = false;

	float duration = 3.f;
	float timerCoffee = 0.f; //������ ���� �ð�
	float timerShot = 0.f;
	float timerMuchine = 0.f;
	float timerWheel = 0.f;
	float itmeDuration = 5.f; //������ ���� �ð�
	float gapLegToHead = 4.f;
	float speed = 150.f; //100
	float timer = 0.f; //���� �ӵ� ������ ���
	float shotDelay = 0.3f;
	float shotSpeed = 300.f; //800

	/*����, ������ ���*/
	int bulletCount = 0;
	float angleStep = 0;
	float startAngle = 0;

	int tileWidth = 0;

	sf::Vector2f tileMapLT;
	
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

