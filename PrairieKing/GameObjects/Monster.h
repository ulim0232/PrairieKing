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
	sf::Vector2f direction; //�̵��ϴ� ����

	float speed = 0.f; //�̵� �ӵ�
	int maxHp = 0;
	int hp = 0;
	string idleClipPath;
	string dieClipPath;
	bool isAlive;

	float duration = 5.f; //������ 5�� �� ����
	float timer = 0.f;

	CowBoy* cowboy = nullptr; //�÷��̾� ������ ���
	ObjectPool<Monster>* pool = nullptr;

	AnimationController animation;
	TileMap* tileMap;
	int tileWidth = 0;
	sf::Vector2i tileSize = { 32.f, 32.f };
	sf::FloatRect intersection;
public:
	Monster(const string& n = "");
	virtual ~Monster() override;

	virtual void Init() override; //���� �Ҵ�(������)
	virtual void Release() override; //���� �Ҵ� ����(�Ҹ���)
	virtual void Reset() override;

	virtual void Update(float dt) override;

	void SetPool(ObjectPool<Monster>* pool) { this->pool = pool; }

	void SetType(Types t);
	Types GetType() const;

	void SetCowboy(CowBoy* p);
	bool GetIsAllive();
	void OnHitBullet(int damage); //�Ѿ˿� �¾��� ��
	void SetTileMap(TileMap* map, int width);
	bool IsCollisoinTile(int index);

};

