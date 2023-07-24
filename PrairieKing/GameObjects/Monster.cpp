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
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip(bombClipPath));

	animation.SetTarget(&sprite);
	SetOrigin(Origins::MC);
	//sprite.setScale({ 5.0f, 2.8125f });
	sprite.setScale({ 2.f, 2.f });

	hitBox.setSize(sf::Vector2f(boxSize));
	//hitBox.setSize({ 60.f, 40.f });
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
			if (isCollision)
			{
				if(collTimer == 0)
				{
					sf::Vector2f tileMapSize = tileMap->GetTileMapSize();
					sf::Vector2f random(Utils::RandomRange(0.f, tileMapSize.x), Utils::RandomRange(0.f, tileMapSize.y));
					sf::Vector2f destination(random.x + tileMapLT.x, random.y + tileMapLT.y);

					absX = abs(destination.x - position.x);
					absY = abs(destination.y - position.y);

					distance = Utils::Distance(destination, position);
					direction = Utils::Normalize(destination - position);
				}
				collTimer += dt;
			}
			else if (!isCollision && collTimer == 0)
			{
				sf::Vector2f cowboyPos = cowboy->GetPosition();
				absX = abs(cowboyPos.x - position.x);
				absY = abs(cowboyPos.y - position.y);

				distance = Utils::Distance(cowboyPos, position);
				direction = Utils::Normalize(cowboyPos - position);
			}
			if (absX > absY) //x���� �̵�
			{
				direction.y = 0;
			}
			else if (absX < absY) //y���� �̵�
			{
				direction.x = 0;
			}
			if (distance > 5.f) //���� �Ÿ��� ��������� �������� ó��
			{
				velocity = direction * speed;
				sf::Vector2f newPos = position + velocity * dt;
				hitBox.setPosition(newPos);

				for (auto tile : tileMap->tiles)
				{
					float tileL = tileMapLT.x + (tileSize.x * tile.x);
					float tileT = tileMapLT.y + (tileSize.y * tile.y);

					sf::FloatRect tileRect(tileL, tileT, tileSize.x, tileSize.y);

					if (tileRect.intersects(hitBox.getGlobalBounds(), intersection))
					{
						if (IsCollisoinTile(tile.texIndex))
						{
							hitBox.setOutlineColor(sf::Color::Red);
							newPos = position;
							isCollision = true;
						}
						else
						{
							hitBox.setOutlineColor(sf::Color::Cyan);
						}
					}
				}
				position = newPos;
				SetPosition(position);
				hitBox.setPosition(position);
			}
			if (collTimer >= collTime)
			{
				collTimer = 0.f;
				isCollision = false;
			}

		}
		if (sprite.getGlobalBounds().intersects(cowboy->GetHitBox().getGlobalBounds()))
		{
			Scene* scene = SCENE_MGR.GetCurrScene();
			SceneGame* sceneGame = dynamic_cast<SceneGame*>(scene); //c++�� ����ȯ ������
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

void Monster::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
	window.draw(hitBox);
}

void Monster::SetType(Types t)
{
	monsterType = t;
	const MonsterInfo& info = DATATABLE_MGR.Get<MonsterTable>(DataTable::Ids::Monster)->Get(t);

	speed = info.speed;
	maxHp = info.maxHp;
	idleClipPath = info.idleClipPath;
	dieClipPath = info.dieClipPath;
	bombClipPath = info.bombClipPath;
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
			OnDie();
			Scene* scene = SCENE_MGR.GetCurrScene();
			SceneGame* sceneGame = dynamic_cast<SceneGame*>(scene); //c++�� ����ȯ ������
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

void Monster::OnBoom()
{
	if (animation.GetCurrentClipId() != "BOMB")
	{
		animation.Stop();
		animation.Play("BOMB");
		isAlive = false;
		timer = 2.f;
	}
}

void Monster::SetTileMap(TileMap* map, int width)
{
	tileMap = map;
	tileWidth = width;

	tileMapLT.x = tileMap->vertexArray.getBounds().left;
	tileMapLT.y = tileMap->vertexArray.getBounds().top;
}

bool Monster::IsCollisoinTile(int index)
{
	/*if (index > 4)
	{
		return true;
	}*/

	return (index > 4);
}
