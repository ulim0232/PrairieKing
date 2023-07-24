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

	/*--�ؽ��� ����--*/
	legAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("tables/cowboy/Move.csv"));
	legAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("tables/cowboy/Idle.csv"));
	headAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("tables/cowboy/Die.csv"));
	headAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("tables/cowboy/Revive.csv"));

	headAnimation.SetTarget(&head);
	legAnimation.SetTarget(&leg);

	//����Ʈ �������� ������
	//head.setScale(2.0f, 2.0f); 
	//leg.setScale(2.0f, 2.0f);

	head.setScale(5.0f, 2.8125f);
	leg.setScale(5.0f, 2.8125f);

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
	poolBullets.OnCreate = [ptr](Bullet* bullet)//�����Լ�, ���ٽ�
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
	/*���� �ʱ�ȭ*/
	isDie = false;
	isSpeedUp = false; //Ŀ��
	isShotGun = false; //���
	isMachineGun = false; //�ӽŰ�
	isWheel = false; // ����
	speed = 150.f;

	/*�ؽ��� �ʱ�ȭ*/
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

	/*�Ѿ� ����*/
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

	//�밢�� �̵� �ӵ� �������� ����
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

		/*---�̵�---*/
		float magnitude = Utils::Magnitude(direction);
		if (magnitude > 1.f)
		{
			direction /= magnitude;
		}
		velocity = direction * speed;

		newPosition = position + velocity * dt;
		hitBox.setPosition(newPosition);

		//float leftX = tileMap->vertexArray.getBounds().left;

		for (auto tile : tileMap->tiles)
		{
			//float tileL = 384 + (tileSize.x * tile.x);
			//float tileT = 104 + (tileSize.y * tile.y);

			float tileL = (tileSize.x * tile.x);
			float tileT = (tileSize.y * tile.y);
			sf::FloatRect tileRect(tileL, tileT, tileSize.x, tileSize.y);

			if (tileRect.intersects(hitBox.getGlobalBounds(), intersection))
			{
				//�̵��� ����
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

		//�ִϸ��̼�
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

		/*---�Ѿ� �߻�---*/
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
						angleStep = 15.f; // �Ѿ� ������ ����
						startAngle = -angleStep * (bulletCount - 1) / 2.f;
					}
					if (isWheel)
					{
						bulletCount = 8;
						angleStep = 45.f; // �Ѿ� ������ ����
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
						SceneGame* sceneGame = dynamic_cast<SceneGame*>(scene); //c++�� ����ȯ ������
						if (sceneGame != nullptr)
						{
							bullet->SetMonsterList(sceneGame->GetMonsterList());
							sceneGame->AddGo(bullet);
							//bullet->sortLayer = 101;
							
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
					SceneGame* sceneGame = dynamic_cast<SceneGame*>(scene); //c++�� ����ȯ ������
					if (sceneGame != nullptr)
					{
						bullet->SetMonsterList(sceneGame->GetMonsterList());
						sceneGame->AddGo(bullet);
						//bullet->sortLayer = 101;
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
	///Ŀ��
	if (isSpeedUp)
	{
		timerCoffee += dt;
	}
	if (isSpeedUp && timerCoffee >= itmeDuration) //Ŀ�� ����
	{
		cout << "speed down" << endl;
		speed = 150.f;
		isSpeedUp = false;
		timerCoffee = 0.f;
	}
	//�����
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
	//����
	if (isWheel)
	{
		timerWheel += dt;
	}
	if (isWheel && timerWheel >= itmeDuration)
	{
		isWheel = false;
		timerWheel = 0.f;
	}
	//����
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

//���� ���x
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
	return (index != 2 && index != 3 && index != 4); // 2, 3, 4�� �̵� ������ Ÿ���� �ε������ ����
}

void CowBoy::CowBoyDie()
{
	isDie = true;
	//�߰��� ����� ����
	if (footStep.getStatus() == sf::SoundSource::Status::Playing)
	{
		footStep.stop();
	}
	
	//die �ִϸ��̼� ���
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
		speed = 2000.f;
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
