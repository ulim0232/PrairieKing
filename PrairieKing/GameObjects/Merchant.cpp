#include "stdafx.h"
#include "Merchant.h"
#include "ResourceMgr.h"
#include "TileMap.h"
#include "CowBoy.h"

void Merchant::Init()
{
	SpriteGo::Init();

	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("tables/merchant/Move.csv"));
	animation.SetTarget(&sprite);

	SetOrigin(Origins::TC);

	sprite.setScale(2.f, 2.f);

	hitBox.setSize(sf::Vector2f(30, 30));
	Utils::SetOrigin(hitBox, Origins::TC);
	hitBox.setFillColor(sf::Color::Cyan);
	hitBox.setPosition(sf::Vector2f(static_cast<int>(position.x), static_cast<int>(position.y)));
}

void Merchant::Reset()
{
	if(!animation.IsPlaying())
	{
		animation.Play("Move");
	}
	SetPosition(0, 0);
}

void Merchant::Update(float dt)
{
	if (isActive && !isArrival)
	{
		if (!animation.IsPlaying())
		{
			animation.Play("Move");
		}
		direction = { 0,1 }; 
		position += direction * speed * dt;
		SetPosition(position);
	}
	if (GetPosition().y >= tileMapLT.y + tileSize.y * 5)
	{
		isArrival = true;

		if (animation.IsPlaying())
		{
			animation.Stop();
		}
	}
	hitBox.setPosition(position);
	/*if (isArrival)
	{
		sf::Vector2f cowboyPos = cowboy->GetPosition();
		direction = Utils::Normalize(cowboyPos - position);
		float angle = std::atan2(direction.x, direction.y);
		float degree = angle * 180.f / M_PI;

		if (degree < 45.f) //왼쪽
		{

		}
		else if (degree > 135.f) //오른쪽
		{
			
		}
		else //정면
		{

		}
	}*/
}


void Merchant::SetTileMap(TileMap* map)
{
	tileMap = map;

	tileMapLT.x = map->vertexArray.getBounds().left;
	tileMapLT.y = map->vertexArray.getBounds().top;

	tileSize = map->GetTileSize();
}

void Merchant::SetCowboy(CowBoy* p)
{
	cowboy = p;
}

void Merchant::SetSpawnPosition()
{
	SetPosition(tileMapLT.x + tileSize.x * 8.f + 16.f, tileMapLT.y);
}
