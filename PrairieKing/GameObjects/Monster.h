#pragma once
#include "SpriteGo.h"

class CowBoy;
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

	sf::Vector2f look; //�ٶ󺸴� ����
	sf::Vector2f direction; //�̵��ϴ� ����
	float speed = 0.f; //�̵� �ӵ�
	int maxHp = 0;
	int hp = 0;

	CowBoy* cowboy = nullptr; //�÷��̾� ������ ���

public:
	Monster(const string& n = "");
	virtual ~Monster() override;

	virtual void Init() override; //���� �Ҵ�(������)
	virtual void Release() override; //���� �Ҵ� ����(�Ҹ���)
	virtual void Reset() override;

	virtual void Update(float dt) override;

	void SetType(Types t);
	Types GetType() const;

	void SetCowboy(CowBoy* p);

	void OnHitBullet(int damage); //�Ѿ˿� �¾��� ��

};

