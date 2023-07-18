#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"
#include "TileMap.h"

class Monster;
class Bullet : public SpriteGo
{
protected:

	sf::Vector2f direction;
	float speed = 0.f;
	int damage = 1;
	sf::FloatRect tileBounds;
	const list<Monster*>* monsters;
	
public:
	ObjectPool<Bullet>* pool;

	Bullet(const string& textureId = "", const string& n = "");
	virtual ~Bullet() override;

	void SetMonsterList(const list<Monster*>* list);
	void Fire(const sf::Vector2f pos, const sf::Vector2f direction, float speed); 
	//매개변수: 시작 위치, 방향, 속도, 일정 시간or거리가 지나면 총알이 사라지도록

	virtual void Init() override; //동적 할당(생성자)
	virtual void Release() override; //동적 할당 해제(소멸자)
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetDamage(int num) { damage = num; }
	int GetDamage() { return damage; }
	void SetTileMapBound(const sf::FloatRect& bounds);

};

