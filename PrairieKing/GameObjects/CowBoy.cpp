#include "stdafx.h"
#include "CowBoy.h"
#include "SpriteGo.h"
#include "InputMgr.h"
#include "Framework.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "SceneGame.h"
#include "AnimationController.h"


CowBoy::CowBoy(const std::string& n)
	: GameObject(n)
{
}

CowBoy::~CowBoy()
{
}

void CowBoy::Init()
{
	//Utils::SetOrigin(head, Origins::BC);
	//Utils::SetOrigin(leg, Origins::TC);

	/*--텍스쳐 세팅--*/
	legAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("tables/cowboy/Move.csv"));
	legAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("tables/cowboy/Idle.csv"));
	headAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("tables/cowboy/Die.csv"));
	headAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("tables/cowboy/Revive.csv"));

	headAnimation.SetTarget(&head);
	legAnimation.SetTarget(&leg);

	head.setScale(2.0f, 2.0f);
	leg.setScale(2.0f, 2.0f);

	left = RESOURCE_MGR.GetTexture("graphics/players/Player_left.png");
	right = RESOURCE_MGR.GetTexture("graphics/players/Player_right.png");
	back = RESOURCE_MGR.GetTexture("graphics/players/Player_back.png");
	front = RESOURCE_MGR.GetTexture("graphics/players/Player_front.png");
	origintex = RESOURCE_MGR.GetTexture("graphics/players/Player_stand.png");


	/*--set hitbox--*/
	hitBox.setSize(sf::Vector2f(boxSize));
	Utils::SetOrigin(hitBox, Origins::BC);
	hitBox.setFillColor(sf::Color::Transparent);
	hitBox.setOutlineThickness(1);
	hitBox.setOutlineColor(sf::Color::Cyan);
	hitBox.setPosition(sf::Vector2f(static_cast<int>(position.x), static_cast<int>(position.y)));

	/*--set tile box--*/
	tileBox.setSize(sf::Vector2f(boxSize));
	Utils::SetOrigin(tileBox, Origins::TL);
	tileBox.setFillColor(sf::Color::Transparent);
	tileBox.setOutlineThickness(2);
	tileBox.setOutlineColor(sf::Color::Black);

	/*--set bullet pool--*/
	ObjectPool<Bullet>* ptr = &poolBullets;
	poolBullets.OnCreate = [ptr](Bullet* bullet)//무명함수, 람다식
	{
		bullet->textureId = "graphics/UIs/shot.png";
		bullet->pool = ptr;
	};
	poolBullets.Init();

	/*---set audio---*/
	footStepBuffer.loadFromFile("sounds/Footstep.wav");
	footStep.setBuffer(footStepBuffer);
	footStep.setLoop(true);

}

void CowBoy::Release()
{
	poolBullets.Release();
}

void CowBoy::Reset()
{
	/*변수 초기화*/
	isDie = false;
	isSpeedUp = false; //커피
	isShotGun = false; //삿건
	isMachineGun = false; //머신건
	isWheel = false; // 바퀴
	speed = 150.f;

	/*텍스쳐 초기화*/
	if (headAnimation.IsPlaying())
	{
		headAnimation.Stop();
	}
	texture = RESOURCE_MGR.GetTexture("graphics/players/Player_stand.png");
	if (texture != nullptr)
	{
		head.setTexture(*texture);
	}
	head.setTextureRect({ 0, 0, (int)texture->getSize().x, (int)texture->getSize().y });

	leg.setColor(sf::Color::White);
	legAnimation.Play("Idle");

	SetPosition(FRAMEWORK.GetWindowSize().x / 2, FRAMEWORK.GetWindowSize().y / 2);

	/*총알 삭제*/
	for (auto bullet : poolBullets.GetUseList())
	{
		SCENE_MGR.GetCurrScene()->RemoveGo(bullet);
	}
	//poolBullets.Clear();
}

void CowBoy::Update(float dt)
{
	legAnimation.Update(dt);
	headAnimation.Update(dt);

	//이동

	//대각선 이동 속도 보정으로 수정
	if (!isDie)
	{
		direction.x = INPUT_MGR.GetAxis(Axis::Horizontal);
		direction.y = INPUT_MGR.GetAxis(Axis::Vertical);

		/*audio*/
		if (direction.x == 0 && direction.y == 0)
		{
			if (footStep.getStatus() == sf::SoundSource::Status::Playing)
			{
				footStep.stop();
			}
		}
		if (direction.x != 0 || direction.y != 0)
		{
			if (footStep.getStatus() != sf::SoundSource::Status::Playing)
			{
				footStep.play();
				footStep.setVolume(100.f);
			}
		}

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
				//이동량 보정
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
		if (INPUT_MGR.GetKey(sf::Keyboard::Left))
		{
			head.setTexture(*left);
		}
		if (INPUT_MGR.GetKey(sf::Keyboard::Up))
		{
			head.setTexture(*back);
		}
		if (INPUT_MGR.GetKey(sf::Keyboard::Down))
		{
			head.setTexture(*front);
		}
		if (INPUT_MGR.GetKeyUp(sf::Keyboard::Down) ||
			INPUT_MGR.GetKeyUp(sf::Keyboard::Up) ||
			INPUT_MGR.GetKeyUp(sf::Keyboard::Right) ||
			INPUT_MGR.GetKeyUp(sf::Keyboard::Left))
		{
			head.setTexture(*origintex);
		}

		/*---총알 발사---*/
		if (!rebound)
		{
			if (INPUT_MGR.GetKey(sf::Keyboard::Left) ||
				INPUT_MGR.GetKey(sf::Keyboard::Up) ||
				INPUT_MGR.GetKey(sf::Keyboard::Down) ||
				INPUT_MGR.GetKey(sf::Keyboard::Right))
			{
				look.x = INPUT_MGR.GetAxisRaw(Axis::HorizontalArrow);
				look.y = INPUT_MGR.GetAxisRaw(Axis::VerticalArrow);

				float magnitude = Utils::Magnitude(look);
				if (magnitude > 1.f)
				{
					look /= magnitude;
				}
				if(isShotGun || isWheel)
				{
					if (isShotGun)
					{
						bulletCount = 3;
						angleStep = 15.f; // 총알 사이의 각도
						startAngle = -angleStep * (bulletCount - 1) / 2.f;
					}
					if (isWheel)
					{
						bulletCount = 8;
						angleStep = 45.f; // 총알 사이의 각도
						startAngle = -angleStep * (bulletCount - 1) / 2.f;
					}

					for (int i = 0; i < bulletCount; i++)
					{
						float radian = (startAngle + angleStep * i) * M_PI / 180;

						sf::Vector2f fireDir;
						fireDir.x = look.x * cos(radian) - look.y * sin(radian);
						fireDir.y = look.x * sin(radian) + look.y * cos(radian);

						Bullet* bullet = poolBullets.Get();
						bullet->SetTileMapBound(tileMap->vertexArray.getBounds());

						sf::Vector2f fireP(GetPosition().x, GetPosition().y - 10.f);
						bullet->Fire(fireP, fireDir, shotSpeed);

						Scene* scene = SCENE_MGR.GetCurrScene();
						SceneGame* sceneGame = dynamic_cast<SceneGame*>(scene); //c++의 형변환 연산자
						if (sceneGame != nullptr)
						{
							bullet->SetMonsterList(sceneGame->GetMonsterList());
							sceneGame->AddGo(bullet);
						}
					}
					rebound = true;
				}
				else
				{
					Bullet* bullet = poolBullets.Get();
					bullet->SetTileMapBound(tileMap->vertexArray.getBounds());
					sf::Vector2f fireP(GetPosition().x, GetPosition().y - 10.f);
					bullet->Fire(fireP, look, shotSpeed);
				
					Scene* scene = SCENE_MGR.GetCurrScene();
					SceneGame* sceneGame = dynamic_cast<SceneGame*>(scene); //c++의 형변환 연산자
					if (sceneGame != nullptr)
					{
						bullet->SetMonsterList(sceneGame->GetMonsterList());
						sceneGame->AddGo(bullet);
					}
					SCENE_MGR.GetCurrScene()->AddGo(bullet);
					rebound = true;
				}
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
	///커피
	if (isSpeedUp)
	{
		timerCoffee += dt;
	}
	if (isSpeedUp && timerCoffee >= itmeDuration) //커피 종료
	{
		cout << "speed down" << endl;
		speed = 150.f;
		isSpeedUp = false;
		timerCoffee = 0.f;
	}
	//기관총
	if (isMachineGun)
	{
		timerMuchine += dt;
	}
	if (isMachineGun && timerMuchine >= itmeDuration)
	{
		isMachineGun = false;
		shotDelay = 0.3f;
		timerMuchine = 0.f;
	}
	//바퀴
	if (isWheel)
	{
		timerWheel += dt;
	}
	if (isWheel && timerWheel >= itmeDuration)
	{
		isWheel = false;
		timerWheel = 0.f;
	}
	//샷건
	if (isShotGun)
	{
		timerShot += dt;
	}
	if (isShotGun && timerShot >= itmeDuration)
	{
		isShotGun = false;
		timerShot = 0.f;
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

//현재 사용x
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
	isDie = true;
	//발걸음 오디오 중지
	if (footStep.getStatus() == sf::SoundSource::Status::Playing)
	{
		footStep.stop();
	}
	
	//die 애니메이션 출력
	if(!headAnimation.IsPlaying())
	{
		leg.setColor(sf::Color::Transparent);
		headAnimation.Play("Die");
	}
}

sf::RectangleShape CowBoy::GetHitBox()
{
	return hitBox;
}


void CowBoy::SetIsDie(bool is)
{
	isDie = is;
}

void CowBoy::TakeItem(Item::ItemTypes type)
{
	if (type == Item::ItemTypes::Coffee && !isSpeedUp)
	{
		speed = 200.f;
		isSpeedUp = true;
		cout << "speed up" << endl;
	}
	if (type == Item::ItemTypes::Shotgun)
	{
		isShotGun = true;
		cout << "use shotgun" << endl;
	}
	if (type == Item::ItemTypes::MuchineGun)
	{
		isMachineGun = true;
		shotDelay = 0.15f;
		cout << "use MuchineGun" << endl;
	}
	if (type == Item::ItemTypes::Wheel)
	{
		isWheel = true;
		cout << "use wheel" << endl;
	}
}
