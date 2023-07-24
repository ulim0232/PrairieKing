#include "stdafx.h"
#include "Item.h"
#include "CowBoy.h"
#include "SceneGame.h"
#include "SceneMgr.h"
#include "DataTableMgr.h"
#include "ItemTable.h"

Item::Item(const string& textureId, const string& n)
	:SpriteGo(textureId, n)
{
}

Item::~Item()
{
}

void Item::SetType(ItemTypes t)
{
	type = t;
	const ItemInfo& info = DATATABLE_MGR.Get<ItemTable>(DataTable::Ids::Item)->Get(t);

	textureId = info.textureId;
}

Item::ItemTypes Item::GetType() const
{
	return type;
}

void Item::Init()
{
	SpriteGo::Init();
	SetOrigin(Origins::MC);
	sprite.setScale({ 5.0f, 2.8125f });
	isSpawn = false;
}

void Item::Release()
{
	SpriteGo::Release();
}

void Item::Reset()
{
	SpriteGo::Reset();
	timer = 0.f;
}

void Item::SetPlayer(CowBoy* p)
{
	cowboy = p;
}

void Item::SetIsSpawn(bool is)
{
	isSpawn = is;
}

void Item::blink()
{
	if (!blinkTimeCheck && clock.getElapsedTime() >= blinkTime)
	{
		clock.restart();
		blinkTimeCheck = true;
		sprite.setColor(sf::Color::Transparent);
	}

	if (clock.getElapsedTime() >= blinkTime)
	{
		clock.restart();
		sprite.setColor(sf::Color::White);
		blinkTimeCheck = false;
	}
}

void Item::Update(float dt)
{
	SpriteGo::Update(dt);

	if (isSpawn)
	{
		timer += dt;
	}
	if (timer >= 7.f)
	{
		blink();

		//10초동안 아이템 먹지 않으면 삭제
		if (timer >= spawnLimit)
		{
			Scene* scene = SCENE_MGR.GetCurrScene();
			SceneGame* sceneGame = dynamic_cast<SceneGame*>(scene); //c++의 형변환 연산자
			if (sceneGame != nullptr)
			{
				sceneGame->RemoveItem(this);
			}
			timer = 0.f;
			sprite.setColor(sf::Color::White);
		}
	}
	
	if (/*cowboy->isAlive && */sprite.getGlobalBounds().intersects(cowboy->GetHitBox().getGlobalBounds()) && isSpawn)
	{
		Scene* scene = SCENE_MGR.GetCurrScene();
		SceneGame* sceneGame = dynamic_cast<SceneGame*>(scene); //c++의 형변환 연산자
		if (sceneGame != nullptr)
		{
			sceneGame->TakeItem(this);
		}
	}
}

void Item::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}
