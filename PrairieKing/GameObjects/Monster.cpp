#include "stdafx.h"
#include "Monster.h"
#include "DataTableMgr.h"
#include "MonsterTable.h"
#include "SceneMgr.h"
#include "SceneGame.h"
#include "ResourceMgr.h"

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

	hp = maxHp; //체력 초기화
	isAlive = true;
}

void Monster::Update(float dt)
{
	SpriteGo::Update(dt); //삭제 고려
	animation.Update(dt);

	if (!isAlive)
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

void Monster::OnHitBullet(int damage)
{
	hp -= damage;
	if (hp <= 0) //hp가 0이 되면 죽는다
	{
		animation.Stop();
		animation.Play("DIE");
		isAlive = false;
		//Scene* scene = SCENE_MGR.GetCurrScene();
		//SceneGame* sceneGame = dynamic_cast<SceneGame*>(scene); //c++의 형변환 연산자
		//if (sceneGame != nullptr)
		//{
		//	sceneGame->OnDieMonster(this);
		//}
	}
}
