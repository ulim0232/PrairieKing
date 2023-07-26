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
		Mirra,
		Imp,
		Pixy,
		Mushroom,
		Ogre,
		Orc
	};
	static const int TotalTypes = 6;

protected:
	Types monsterType;
	sf::Vector2i boxSize = { 20.f, 20.f };
	sf::RectangleShape hitBox;
	sf::Vector2f velocity;
	sf::Vector2f direction; //�̵��ϴ� ����

	float speed = 0.f; //�̵� �ӵ�
	int maxHp = 0;
	int hp = 0;
	string idleClipPath;
	string dieClipPath;
	string bombClipPath;
	bool isAlive;

	float duration = 3.f; //������ 3�� �� ����
	float timer = 0.f;

	CowBoy* cowboy = nullptr; //�÷��̾� ������ ���
	ObjectPool<Monster>* pool = nullptr;

	AnimationController animation;
	TileMap* tileMap;
	int tileWidth = 0;
	//sf::Vector2i tileSize = { 80, 45 };
	sf::Vector2i tileSize = { 32, 32 };
	sf::FloatRect intersection;
	sf::Vector2f tileMapLT;
	bool isCollision = false;
	float collTime = 1.5f;
	float collTimer = 0.f;
	int collCount = 0;

	float absX;
	float absY;
	float distance;
public:
	Monster(const string& n = "");
	virtual ~Monster() override;

	virtual void Init() override; //���� �Ҵ�(������)
	virtual void Release() override; //���� �Ҵ� ����(�Ҹ���)
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetPool(ObjectPool<Monster>* pool) { this->pool = pool; }

	void SetType(Types t);
	Types GetType() const;

	void SetCowboy(CowBoy* p);
	bool GetIsAllive();
	void OnHitBullet(int damage); //�Ѿ˿� �¾��� ��
	void OnDie(); //���� ��
	void OnBoom();
	void SetTileMap(TileMap* map, int width);
	bool IsCollisoinTile(int index);

};

