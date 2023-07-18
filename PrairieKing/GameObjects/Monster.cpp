#include "stdafx.h"
#include "Monster.h"
#include "DataTableMgr.h"
#include "MonsterTable.h"
#include "SceneMgr.h"
#include "SceneGame.h"
#include "ResourceMgr.h"
#include "CowBoy.h"

Monster::Monster(const string& n)
	:SpriteGo("", n)
{
}

Monster::~Monster()
{
}

void Monster::Init()
{
	SpriteGo::Init();
	string textureId = "graphics/monsters/Monster_sheet.png";
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip(idleClipPath));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip(dieClipPath));

	animation.SetTarget(&sprite);
	SetOrigin(Origins::MC);
	sprite.setScale({ 2.f, 2.f });
}

void Monster::Release()
{
	SpriteGo::Release();
}

void Monster::Reset()
{
	animation.Play("Idle");
	SetOrigin(Origins::MC);
	SetPosition({ 0, 0 });

	hp = maxHp; //ü�� �ʱ�ȭ
	isAlive = true;
}

void Monster::Update(float dt)
{
	SpriteGo::Update(dt); //���� ���
	animation.Update(dt);

	if(isAlive)
	{
		if (cowboy == nullptr)
			return;
		else
		{
			sf::Vector2f cowboyPos = cowboy->GetPosition();

			float distance = Utils::Distance(cowboyPos, position);
			look = direction = Utils::Normalize(cowboyPos - position); //������-�� ��ġ: ���� ���� �� ����

			if (distance > 25.f) //���� �Ÿ��� ��������� �������� ó��
			{
				position += direction * speed * dt;
				SetPosition(position);
			}
		}
	}
	else
	{
		timer += dt;
		if (timer > duration)
		{
			if (pool != nullptr)
			{
				SCENE_MGR.GetCurrScene()->RemoveGo(this);
				pool->Return(this);
			}
			else
			{
				SetActive(false);
			}
		}
	}
}

void Monster::SetType(Types t)
{
	monsterType = t;
	const MonsterInfo& info = DATATABLE_MGR.Get<MonsterTable>(DataTable::Ids::Monster)->Get(t);

	speed = info.speed;
	maxHp = info.maxHp;
	idleClipPath = info.idleClipPath;
	dieClipPath = info.dieClipPath;
}

Monster::Types Monster::GetType() const
{
	return monsterType;
}

void Monster::SetCowboy(CowBoy* p)
{
	cowboy = p;
}

bool Monster::GetIsAllive()
{
	return isAlive;
}

void Monster::OnHitBullet(int damage)
{
	if(isAlive)
	{
		hp -= damage;
		if (hp <= 0) //hp�� 0�� �Ǹ� �״´�
		{
			animation.Stop();
			animation.Play("DIE");
			isAlive = false;
			//Scene* scene = SCENE_MGR.GetCurrScene();
			//SceneGame* sceneGame = dynamic_cast<SceneGame*>(scene); //c++�� ����ȯ ������
			//if (sceneGame != nullptr)
			//{
			//	sceneGame->OnDieMonster(this);
			//}
		}
	}
}
