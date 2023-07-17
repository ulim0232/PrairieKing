#include "stdafx.h"
#include "CowBoy.h"
#include "SpriteGo.h"
#include "InputMgr.h"
#include "Framework.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "SceneGame.h"


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
	headAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("tables/Die.csv"));
	
	legAnimation.SetTarget(&leg);
	headAnimation.SetTarget(&head);

	//sf::Texture* tex = RESOURCE_MGR.GetTexture("graphics/players/Player_stand.png");
	texture = new sf::Texture();
	texture->loadFromFile("graphics/players/Player_stand.png");
	head.setTexture(*texture);

	head.setScale(2.0f, 2.0f);
	leg.setScale(2.0f, 2.0f);

	hitBox.setSize(sf::Vector2f(boxSize));
	Utils::SetOrigin(hitBox, Origins::BC);
	hitBox.setFillColor(sf::Color::Transparent);
	hitBox.setOutlineThickness(1);
	hitBox.setOutlineColor(sf::Color::Cyan);
	hitBox.setPosition(sf::Vector2f(static_cast<int>(position.x), static_cast<int>(position.y)));

	tileBox.setSize(sf::Vector2f(boxSize));
	Utils::SetOrigin(tileBox, Origins::TL);
	tileBox.setFillColor(sf::Color::Transparent);
	tileBox.setOutlineThickness(2);
	tileBox.setOutlineColor(sf::Color::Black);

	ObjectPool<Bullet>* ptr = &poolBullets;
	poolBullets.OnCreate = [ptr](Bullet* bullet)//무명함수, 람다식
	{
		bullet->textureId = "graphics/UIs/shot.png";
		bullet->pool = ptr;
	};

	poolBullets.Init();

}

void CowBoy::Release()
{
	delete texture;
	poolBullets.Release();
}

void CowBoy::Reset()
{
	legAnimation.SetTarget(&leg);
	leg.setColor(sf::Color::White);
	headAnimation.Stop();
 	head.setTexture(*texture);
	head.setTextureRect({ 0, 0, (int)texture->getSize().x, (int)texture->getSize().y });

	legAnimation.Play("Idle");
	SetPosition(FRAMEWORK.GetWindowSize().x / 2, FRAMEWORK.GetWindowSize().y / 2);
	

	for (auto bullet : poolBullets.GetUseList())
	{
		SCENE_MGR.GetCurrScene()->RemoveGo(bullet);
	}

	poolBullets.Clear();
}

void CowBoy::Update(float dt)
{
	legAnimation.Update(dt);
	headAnimation.Update(dt);
	//이동
	
	//대각선 이동 속도 보정으로 수정
	direction.x = INPUT_MGR.GetAxis(Axis::Horizontal);
	direction.y = INPUT_MGR.GetAxis(Axis::Vertical);

	float magnitude = Utils::Magnitude(direction);
	if (magnitude > 1.f)
	{
		direction /= magnitude;
	}
	velocity = direction * speed;

	newPosition = position + velocity * dt;
	hitBox.setPosition(newPosition);

	for (auto tile : tileMap->tiles)
	{
		float tileL = 384 + (tileSize.x * tile.x);
		float tileT = 104 + (tileSize.y * tile.y);
		sf::FloatRect tileRect(tileL, tileT, tileSize.x, tileSize.y);

		if (tileRect.intersects(hitBox.getGlobalBounds(), intersection))
		{
			float overlapX = std::abs(intersection.width) * ((velocity.x > 0) ? -1.0f : 1.0f);
			float overlapY = std::abs(intersection.height) * ((velocity.y > 0) ? -1.0f : 1.0f);
			if (IsCollisoinTile(tile.texIndex))
			{
				hitBox.setOutlineColor(sf::Color::Red);
				if (std::abs(overlapX) < std::abs(overlapY))
				{
					newPosition.x = position.x + overlapX;
				}
				else
				{
					newPosition.y = position.y + overlapY;
				}
			}
			else
			{
				hitBox.setOutlineColor(sf::Color::Cyan);
			}
		}
	}

	position = newPosition;

	//if (!IsTileCollision()) //충돌이 없는 경우
	//{
	//	position = newPosition;
	//}
	//else
	//{
	//	//이동 거리 조정
	//	float overlapX = std::abs(intersection.width) * ((velocity.x > 0) ? -1.0f : 1.0f);
	//	float overlapY = std::abs(intersection.height) * ((velocity.y > 0) ? -1.0f : 1.0f);
	//
	//	for (auto tile : tileMap->tiles)
	//	{
	//		float tileL = 384 + (tileSize.x * tile.x);
	//		float tileT = 104 + (tileSize.y * tile.y);
	//		sf::FloatRect tileRect(tileL, tileT, tileSize.x, tileSize.y);
	//
	//		if (tileRect.intersects(hitBox.getGlobalBounds(), intersection))
	//		{
	//			if (IsCollisoinTile(tile.texIndex))
	//			{
	//				if (std::abs(overlapX) < std::abs(overlapY))
	//				{
	//					newPosition.x = position.x + overlapX;
	//				}
	//				else
	//				{
	//					newPosition.y = position.y + overlapY;
	//				}
	//			}
	//		}
	//	}
	//
	//	position = newPosition;
	//}
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
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num8))
	{
		leg.setColor(sf::Color::Transparent);
		headAnimation.Play("Die");
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num9))
	{
		Reset();
	}

	/*---총알 발사---*/
	if(!rebound)
	{
		if (INPUT_MGR.GetKey(sf::Keyboard::Left) ||
			INPUT_MGR.GetKey(sf::Keyboard::Up) ||
			INPUT_MGR.GetKey(sf::Keyboard::Down) ||
			INPUT_MGR.GetKey(sf::Keyboard::Right))
		{
			look.x = INPUT_MGR.GetAxisRaw(Axis::HorizontalArrow);
			look.y = INPUT_MGR.GetAxisRaw(Axis::VerticalArrow);

			magnitude = Utils::Magnitude(look);
			if (magnitude > 1.f)
			{
				look /= magnitude;
			}

			Bullet* bullet = poolBullets.Get();
			bullet->SetTileMapBound(tileMap->vertexArray.getBounds());
			sf::Vector2f fireP(GetPosition().x, GetPosition().y - 10.f);
			bullet->Fire(fireP, look, 400.f);

			Scene* scene = SCENE_MGR.GetCurrScene();
			SceneGame* sceneGame = dynamic_cast<SceneGame*>(scene); //c++의 형변환 연산자
			if (sceneGame != nullptr)
			{
				sceneGame->AddGo(bullet);
			}
			SCENE_MGR.GetCurrScene()->AddGo(bullet);
			rebound = true;
		}
	}
	else
	{
		timer += dt;
		if (timer >= shotDelay)
		{
			timer = 0.f;
			rebound = false;
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
bool CowBoy::IsTileCollision()
{
	for (auto tile : tileMap->tiles)
	{
		float tileL = 384 + (tileSize.x * tile.x);
		float tileT = 104 + (tileSize.y * tile.y);
		sf::FloatRect tileRect(tileL, tileT, tileSize.x, tileSize.y);

		if (tileRect.intersects(hitBox.getGlobalBounds(), intersection))
		{
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
	return (index != 2 && index != 3 && index != 4); // 2, 3, 4가 이동 가능한 타일의 인덱스라고 가정
}

void CowBoy::CowBoyDie()
{
	leg.setColor(sf::Color::Transparent);
	headAnimation.Play("Die");
}
