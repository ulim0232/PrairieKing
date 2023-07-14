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
	float h = INPUT_MGR.GetAxis(Axis::Horizontal);
	float v = INPUT_MGR.GetAxis(Axis::Vertical);

	//이동
	velocity.x = h * speed;
	velocity.y = v * speed;
	position += velocity * dt;
	/*if (!wallBounds.contains(position))
	{
		position =

		::Clamp(position, wallBoundsLT, wallBoundsRB);
	}*/

	SetPosition(position);

	//애니메이션
	if (legAnimation.GetCurrentClipId() == "Idle")
	{
		if (h != 0 || v != 0)
		{
			legAnimation.Play("Move");
		}
	}
	else if (legAnimation.GetCurrentClipId() == "Move")
	{
		if (h == 0.f && v == 0.f)
		{
			legAnimation.Play("Idle");
		}
	}
}

void CowBoy::Draw(sf::RenderWindow& window)
{
	window.draw(head);
	window.draw(leg);
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
