#include "stdafx.h"
#include "Monster.h"
#include "DataTableMgr.h"
#include "MonsterTable.h"
#include "SceneMgr.h"
#include "SceneGame.h"
#include "ResourceMgr.h"
#include "CowBoy.h"
#include "TileMap.h"

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

	hitBox.setSize(sf::Vector2f(boxSize));
	Utils::SetOrigin(hitBox, Origins::MC);
	hitBox.setFillColor(sf::Color::Transparent);
	hitBox.setOutlineThickness(1);
	hitBox.setOutlineColor(sf::Color::Cyan);
	hitBox.setPosition(sf::Vector2f(static_cast<int>(position.x), static_cast<int>(position.y)));
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

	if(isAlive)
	{
		if (cowboy == nullptr)
			return;
		else
		{
			sf::Vector2f cowboyPos = cowboy->GetPosition();

			float distance = Utils::Distance(cowboyPos, position);
			direction = Utils::Normalize(cowboyPos - position); //목적지-내 위치: 방향 구할 수 있음

			if (distance > 5.f) //일정 거리에 가까워지면 도착으로 처리
			{
				velocity = direction * speed;
				position += velocity * dt;
				SetPosition(position);
			}
		}
		if (sprite.getGlobalBounds().intersects(cowboy->GetHitBox().getGlobalBounds()))
		{
			Scene* scene = SCENE_MGR.GetCurrScene();
			SceneGame* sceneGame = dynamic_cast<SceneGame*>(scene); //c++의 형변환 연산자
			if (sceneGame != nullptr)
			{
				sceneGame->OnDieCowBoy();
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
		if (hp <= 0) //hp가 0이 되면 죽는다
		{
			OnDie();
			Scene* scene = SCENE_MGR.GetCurrScene();
			SceneGame* sceneGame = dynamic_cast<SceneGame*>(scene); //c++의 형변환 연산자
			if (sceneGame != nullptr)
			{
				sceneGame->OnDieMonster(this);
			}
		}
	}
}

void Monster::OnDie()
{
	if (animation.GetCurrentClipId() != "DIE")
	{
		animation.Stop();
		animation.Play("DIE");
		isAlive = false;
	}
}

void Monster::SetTileMap(TileMap* map, int width)
{
	tileMap = map;
	tileWidth = width;
}

bool Monster::IsCollisoinTile(int index)
{
	if (index > 4)
	{
		return true;
	}
}
