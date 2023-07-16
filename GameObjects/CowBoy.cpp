#include "stdafx.h"
#include "CowBoy.h"
#include "SpriteGo.h"
#include "InputMgr.h"
#include "Framework.h"
#include "ResourceMgr.h"


CowBoy::CowBoy(const std::string& n)
	: GameObject(n)
{
}

CowBoy::~CowBoy()
{
}

void CowBoy::Init()
{
	Utils::SetOrigin(head, Origins::BC);
	Utils::SetOrigin(leg, Origins::TC);

	legAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("tables/Move.csv"));
	legAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("tables/Idle.csv"));
	legAnimation.SetTarget(&leg);

	//sf::Texture* tex = RESOURCE_MGR.GetTexture("graphics/players/Player_stand.png");
	auto texture = new sf::Texture();
	texture->loadFromFile("graphics/players/Player_stand.png");
	head.setTexture(*texture);

	hitBox.setSize(boxSize);
	Utils::SetOrigin(hitBox, Origins::BC);
	hitBox.setFillColor(sf::Color::Transparent);
	hitBox.setOutlineThickness(1);
	hitBox.setOutlineColor(sf::Color::Cyan);
	hitBox.setPosition(position);

	tileBox.setSize(boxSize);
	Utils::SetOrigin(tileBox, Origins::TL);
	tileBox.setFillColor(sf::Color::Transparent);
	tileBox.setOutlineThickness(2);
	tileBox.setOutlineColor(sf::Color::Black);

}

void CowBoy::Release()
{
}

void CowBoy::Reset()
{
	legAnimation.Play("Idle");
	SetPosition(0.f, 0.f);
	head.setScale(2.0f, 2.0f);
	leg.setScale(2.0f, 2.0f);
}

void CowBoy::Update(float dt)
{
	legAnimation.Update(dt);
	

	//이동
	
	//float h = INPUT_MGR.GetAxis(Axis::Horizontal);
	//float v = INPUT_MGR.GetAxis(Axis::Vertical);
	//velocity.x = h * speed;
	//velocity.y = v * speed;
	
	//대각선 이동 속도 보정으로 수정
	direction.x = INPUT_MGR.GetAxis(Axis::Horizontal);
	direction.y = INPUT_MGR.GetAxis(Axis::Vertical);

	float magnitude = Utils::Magnitude(direction);
	if (magnitude > 1.f)
	{
		direction /= magnitude;
	}
	velocity = direction * speed;
	//position += velocity * dt;

	sf::Vector2f newPosition = position + velocity * dt;
	sf::Vector2f originalPosition = position;
	
	if (!IsTileCollision(newPosition)) //충돌이 없는 경우
	{
		position = newPosition;
	}
	SetPosition(position);
	hitBox.setPosition(position);

	//애니메이션
	if (legAnimation.GetCurrentClipId() == "Idle")
	{
		if (direction.x != 0 || direction.y != 0)
		{
			legAnimation.Play("Move");
		}
	}
	else if (legAnimation.GetCurrentClipId() == "Move")
	{
		if (direction.x == 0.f && direction.y == 0.f)
		{
			legAnimation.Play("Idle");
		}
	}
	if (INPUT_MGR.GetKey(sf::Keyboard::Right))
	{
		if (right != nullptr)
		{
			head.setTexture(*right);
		}
	}
}

void CowBoy::Draw(sf::RenderWindow& window)
{
	window.draw(head);
	window.draw(leg);
	window.draw(hitBox);
	window.draw(tileBox);
}

void CowBoy::SetPosition(const sf::Vector2f& p)
{
	GameObject::SetPosition(p);
	head.setPosition(p);
	leg.setPosition(p.x, p.y - gapLegToHead);
}

void CowBoy::SetPosition(float x, float y)
{
	GameObject::SetPosition(x, y);;
	head.setPosition(x, y);
	leg.setPosition(x, y);
}

void CowBoy::SetOrigin(Origins origin)
{
	GameObject::SetOrigin(origin);
	if (this->origin != Origins::CUSTOM)
	{
		Utils::SetOrigin(head, origin);
		Utils::SetOrigin(leg, origin);
	}
}

void CowBoy::SetOrigin(float x, float y)
{
	GameObject::SetOrigin(x, y);
	head.setOrigin(x, y);
	leg.setOrigin(x, y);
}

void CowBoy::SetTileMap(TileMap* map, int width)
{
	tileMap = map;
	tileWidth = width;
}

bool CowBoy::IsTileCollision(const sf::Vector2f p)
{
	for (auto tile : tileMap->tiles)
	{
		float tileL = 384 + (32 * tile.x);
		float tileT = 104 + (32 * tile.y);
		sf::FloatRect tileRect(tileL, tileT, 32, 32);
		if (tileRect.intersects(hitBox.getGlobalBounds()))
		{
			//cout << "collison with" << tile.texIndex << endl;
			tileBox.setPosition(tileL, tileT);
			if (IsCollisoinTile(tile.texIndex))
			{
				hitBox.setOutlineColor(sf::Color::Red);
				return true;
			}
			else
			{
				hitBox.setOutlineColor(sf::Color::Cyan);
			}
		}
	}
	return false;
}

bool CowBoy::IsCollisoinTile(int index)
{
	cout << index << endl;
	return (index != 2 && index != 3 && index != 4); // 2, 3, 4가 이동 가능한 타일의 인덱스라고 가정
	
	//if ((index == 2) || (index == 3) || (index == 4)) //이동 가능한 타일
	//{
	//	return 0;
	//}
	//else if (index == -1)
	//{
	//	cout << "something wrong" << endl;
	//}
	//else
	//{
	//	hitBox.setOutlineColor(sf::Color::Red);
	//	return 1;
	//}

}