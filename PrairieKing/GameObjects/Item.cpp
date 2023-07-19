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
	sprite.setScale({ 2.f, 2.f });
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

void Item::Update(float dt)
{
	SpriteGo::Update(dt);

	if (isSpawn)
	{
		timer += dt;
	}
	if (timer >= spawnLimit)
	{
		Scene* scene = SCENE_MGR.GetCurrScene();
		SceneGame* sceneGame = dynamic_cast<SceneGame*>(scene); //c++의 형변환 연산자
		if (sceneGame != nullptr)
		{
			sceneGame->RemoveItem(this);
		}
		timer = 0.f;
	}
	if (/*cowboy->isAlive && */sprite.getGlobalBounds().intersects(cowboy->GetHitBox().getGlobalBounds()) && isSpawn)
	{
		cowboy->TakeItem(type);

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
