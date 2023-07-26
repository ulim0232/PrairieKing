#include "stdafx.h"
#include "Bullet.h"
#include "Monster.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "TileMap.h"

Bullet::Bullet(const string& textureId, const string& n)
	:SpriteGo(textureId, n)
{
}

Bullet::~Bullet()
{
}

void Bullet::SetMonsterList(const list<Monster*>* list)
{
	monsters = list;
}

void Bullet::Fire(const sf::Vector2f pos, const sf::Vector2f direction, float speed)
{
	SetPosition(pos);
	
	this->direction = direction;
	this->speed = speed;

}

void Bullet::Init()
{
	SpriteGo::Init();
	SetOrigin(Origins::MC);
	sprite.setScale({ 1.5f, 1.5f });
}

void Bullet::Release()
{
	SpriteGo::Release();
}

void Bullet::Reset()
{
	SpriteGo::Reset();

	sprite.setRotation(0.f);
	SetPosition(0.f, 0.f);
	speed = 0.f;
	direction = { 0.f, 0.f };
}

void Bullet::Update(float dt)
{
	SpriteGo::Update(dt);

	//ȸ���� ����
	//�� �ٱ����� ������ ����
	if (!tileBounds.contains(position))
	{
		SCENE_MGR.GetCurrScene()->RemoveGo(this);
		pool->Return(this);
	}

	position += direction * speed * dt;
	SetPosition(position);
	
	if (monsters != nullptr)
	{
		for (Monster* monster : *monsters)
		{
			if (sprite.getGlobalBounds().intersects(monster->sprite.getGlobalBounds()) && monster->GetIsAllive())
			{
				monster->OnHitBullet(damage);
				//�Ѿ˰� ���� �浹�ϸ� �Ѿ� ����
				SCENE_MGR.GetCurrScene()->RemoveGo(this);
				pool->Return(this);
				break;
			}
		}
	}
	if (tileMap != nullptr)
	{
		float tileXIndex = (position.x - TileMapLT.x) / tileSize;
		float tileYIndex = (position.y - TileMapLT.y) / tileSize;
		int texIndex = tileMap->GetTileIndex(tileXIndex, tileYIndex);

		if (texIndex > 4)
		{
			SCENE_MGR.GetCurrScene()->RemoveGo(this);
			pool->Return(this);
		}
	}
	

}

void Bullet::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void Bullet::SetTileMapBound(const sf::FloatRect& bounds)
{
	tileBounds = bounds;
}

void Bullet::SetTileMap(TileMap* map)
{
	tileMap = map;
	TileMapLT.x = tileMap->vertexArray.getBounds().left;
	TileMapLT.y = tileMap->vertexArray.getBounds().top;
}
