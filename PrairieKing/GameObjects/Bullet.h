#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"
#include "TileMap.h"

//class Zombie;
class Bullet : public SpriteGo
{
protected:

	sf::Vector2f direction;
	float speed = 0.f;
	float range = 2000.f;
	int damage = 1;
	sf::FloatRect tileBounds;
	//const list<Zombie*>* zombies;
	
public:
	ObjectPool<Bullet>* pool;

	Bullet(const string& textureId = "", const string& n = "");
	virtual ~Bullet() override;

	//void SetZombieList(const list<Zombie*>* list);
	void Fire(const sf::Vector2f pos, const sf::Vector2f direction, float speed); 
	//�Ű�����: ���� ��ġ, ����, �ӵ�, ���� �ð�or�Ÿ��� ������ �Ѿ��� ���������

	virtual void Init() override; //���� �Ҵ�(������)
	virtual void Release() override; //���� �Ҵ� ����(�Ҹ���)
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetDamage(int num) { damage = num; }
	int GetDamage() { return damage; }
	void SetTileMapBound(const sf::FloatRect& bounds);

};
