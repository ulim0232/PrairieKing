#include "stdafx.h"
#include "SceneGame.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "GameObject.h"
#include "SpriteGo.h"
#include "Framework.h"
#include "VertexArrayGo.h"
#include "TileMap.h"
#include "TextGo.h"
#include "RectangleGo.h"
#include "CowBoy.h"
#include "Monster.h"
#include "Merchant.h"
#include "Stats.h"

SceneGame::SceneGame() : Scene(SceneId::Game)
{
	//csv������ ���ؼ� �ε��ϴ� ������ ����
	resourceListPath = "tables/scripts/SceneGameResourceList.csv";

	timerDecreaseRate = 1.0f / timeLimit;
	timerDecreaseAmount = 482.0f * timerDecreaseRate;
}

void SceneGame::Init()
{
	Release();

	size = FRAMEWORK.GetWindowSize();
	centerPos = size * 0.5f;
	
	worldView.setSize({ 512, 512 });
	worldView.setCenter(centerPos);

	uiView.setSize(size);
	uiView.setCenter(centerPos);

	/*----�� ����----*/
	tileMap1 = (TileMap*)AddGo(new TileMap("graphics/maps/map_sheet_stage1.png", "TileMap1"));
	tileMap2 = (TileMap*)AddGo(new TileMap("graphics/maps/map_sheet_stage1.png", "TileMap2"));
	tileMap3 = (TileMap*)AddGo(new TileMap("graphics/maps/map_sheet_stage1.png", "TileMap3"));
	tileMap4 = (TileMap*)AddGo(new TileMap("graphics/maps/map_sheet_stage1.png", "TileMap4"));
	tileMap5 = (TileMap*)AddGo(new TileMap("graphics/maps/map_sheet_stage1.png", "TileMap5"));
	tileMap1->sortLayer = -1;
	tileMap2->sortLayer = -1;

	/*---�÷��̾� ����----*/
	cowBoy = (CowBoy*)AddGo(new CowBoy("cowBoy"));
	cowBoy->sortLayer = 4;

	/*--���� ����--*/
	merchant = (Merchant*)AddGo(new Merchant("Merchant"));
	merchant->sortLayer = 3;
	
	boots = (Stats*)AddGo(new Stats("graphics/stats/upBoots1.png", "boots"));
	gun = (Stats*)AddGo(new Stats("graphics/stats/upGun1.png", "gun"));
	shot = (Stats*)AddGo(new Stats("graphics/stats/upShot1.png", "shot"));
	boots->sortLayer = 3;
	gun->sortLayer = 3;
	shot->sortLayer = 3;

	/*----UI����----*/
	coinUI = (SpriteGo*)AddGo(new SpriteGo("graphics/UIs/coin1.png", "coinUI"));
	itemUI = (SpriteGo*)AddGo(new SpriteGo("graphics/UIs/itemUI.png", "itemUI"));
	lifeUI = (SpriteGo*)AddGo(new SpriteGo("graphics/UIs/life.png", "lifeUI"));
	shotUI = (SpriteGo*)AddGo(new SpriteGo("graphics/UIs/shot.png", "shotUI"));
	timerUI = (SpriteGo*)AddGo(new SpriteGo("graphics/UIs/timerUI.png", "timerUI"));
	keyUI = (SpriteGo*)AddGo(new SpriteGo("graphics/UIs/controlKey.png", "keyUI"));
	pickedItemUI = (SpriteGo*)AddGo(new SpriteGo("graphics/items/coffee.png", "pickedItemUI"));
	arrow = (SpriteGo*)AddGo(new SpriteGo("graphics/UIs/arrow.png", "arrowUI"));
	merchantTable = (SpriteGo*)AddGo(new SpriteGo("graphics/players/merchant_table.png", "table"));

	sf::Vector2f sizeRect(10.f, 10.f);
	AddGo(new RectangleGo(sizeRect, "center"));

	coinUI->sprite.setScale({ 2.f, 2.f });
	itemUI->sprite.setScale({ 2.f, 2.f });
	lifeUI->sprite.setScale({ 2.f, 2.f });
	shotUI->sprite.setScale({ 2.f, 2.f });
	timerUI->sprite.setScale({ 2.f, 2.f });
	keyUI->sprite.setScale({ 2.f, 2.f }); 
	pickedItemUI->sprite.setScale({ 2.f, 2.f });
	arrow->sprite.setScale({ 2.f, 2.f });
	merchantTable->sprite.setScale(2.f, 2.f);

	coinUI->sortLayer = 100;
	itemUI->sortLayer = 100;
	lifeUI->sortLayer = 100;
	shotUI->sortLayer = 100;
	timerUI->sortLayer = 100;
	keyUI->sortLayer = 100;
	pickedItemUI->sortLayer = 100;
	merchantTable->sortLayer = 2;
	arrow->sortLayer = 4;

	pickedItemUI->SetActive(false);
	arrow->SetActive(false);
	merchantTable->SetActive(false);

	/*----�ؽ�Ʈ ����----*/
	coinTxt = (TextGo*)AddGo(new TextGo("coinTxt", "fonts/angel.ttf"));
	std::string ss = "X " + to_string(coinCount);
	coinTxt->SetText(ss, 25, sf::Color::White, Origins::ML, 100, 0, 0);
	coinTxt->text.setOutlineThickness(0.5f);
	coinTxt->text.setOutlineColor(sf::Color::White);

	lifeTxt = (TextGo*)AddGo(new TextGo("lifeTxt", "fonts/angel.ttf"));

	bootsTxt = (TextGo*)AddGo(new TextGo("bootsTxt", "fonts/angel.ttf"));
	gunTxt = (TextGo*)AddGo(new TextGo("gunTxt", "fonts/angel.ttf"));
	shotTxt = (TextGo*)AddGo(new TextGo("shotTxt", "fonts/angel.ttf"));

	/*---Ÿ�̸� ������ ����----*/
	
	timerGauge = (RectangleGo*)AddGo(new RectangleGo(timersize, "timerGauge"));
	timerGauge->SetOrigin(Origins::ML);
	timerGauge->rectangle.setFillColor(sf::Color::Green);
	timerGauge->sortLayer = 100;

	for (auto go : gameObjects)
	{
		go->Init();
	}

	/*----�� ����----*/
	tileMap1->Load("maps/stage1-1.csv");
	tileMap1->SetOrigin(Origins::MC);
	tileMap1->SetPosition(centerPos);

	//tileMap2->Load("maps/stage1-2.csv");
	//tileMap2->SetOrigin(Origins::MC);
	//tileMap2->SetPosition(centerPos.x, centerPos.y + tileMap1->GetTileMapSize().y);

	//tileMap3->Load("maps/stage1-3.csv");
	//tileMap3->SetOrigin(Origins::MC);
	//tileMap3->SetPosition(centerPos);

	//tileMap4->Load("maps/stage1-4.csv");
	//tileMap4->SetOrigin(Origins::MC);
	//tileMap4->SetPosition(centerPos);

	//tileMap5->Load("maps/stage1-5.csv");
	//tileMap5->SetOrigin(Origins::MC);
	//tileMap5->SetPosition(centerPos);


	/*----UI����----*/
	coinUI->SetOrigin(Origins::MC);
	itemUI->SetOrigin(Origins::MC);
	lifeUI->SetOrigin(Origins::MC);
	shotUI->SetOrigin(Origins::MC);
	timerUI->SetOrigin(Origins::MC);
	keyUI->SetOrigin(Origins::MC);
	pickedItemUI->SetOrigin(Origins::MC);
	arrow->SetOrigin(Origins::MC);
	merchantTable->SetOrigin(Origins::TL);

	/*----���� ����----*/
	monsterPool.OnCreate = [this](Monster* monster)
	{
		//Monster::Types monsterType = (Monster::Types)(Utils::RandomRange(0, Monster::TotalTypes) + 1); //1���� ����
		Monster::Types monsterType = (Monster::Types)Utils::RandomRange(4, 6);
		monster->SetType(monsterType);
		monster->SetCowboy(cowBoy);
		monster->SetPool(&monsterPool);
		monster->SetTileMap(tileMap1,32);
		monster->sortLayer = 2;
	};
	monsterPool.Init();

	/*----������ ����----*/
	itemPool.OnCreate = [this](Item* item)
	{
		Item::ItemTypes itemtype = (Item::ItemTypes)Utils::RandomRange(0, Item::TotalTypes);
		item->SetType(itemtype);
		item->SetPlayer(cowBoy);
		item->sortLayer = 2;
	};
	itemPool.Init();

	/*---���� ����---*/
	boots->SetType(Stats::StatsTypes::Boots);
	boots->SetLevel(0);
	boots->SetOrigin(Origins::MC);
	gun->SetType(Stats::StatsTypes::Gun);
	gun->SetLevel(0);
	gun ->SetOrigin(Origins::MC);
	shot->SetType(Stats::StatsTypes::Shot);
	shot->SetLevel(0);
	shot->SetOrigin(Origins::MC);

	/*---���� �ؽ�Ʈ ����---*/
	ss = to_string(boots->getCost());
	bootsTxt->SetText(ss, 25, sf::Color::Black, Origins::TC, 3, 0, 0);
	bootsTxt->text.setOutlineThickness(0.5f);
	bootsTxt->text.setOutlineColor(sf::Color::Black);
	bootsTxt->SetOrigin(Origins::TC);

	ss = to_string(gun->getCost());
	gunTxt->SetText(ss, 25, sf::Color::Black, Origins::TC, 3, 0, 0);
	gunTxt->text.setOutlineThickness(0.5f);
	gunTxt->text.setOutlineColor(sf::Color::Black);
	gunTxt->SetOrigin(Origins::TC);

	ss = to_string(shot->getCost());
	shotTxt->SetText(ss, 25, sf::Color::Black, Origins::TC, 3, 0, 0);
	shotTxt->text.setOutlineThickness(0.5f);
	shotTxt->text.setOutlineColor(sf::Color::Black);
	shotTxt->SetOrigin(Origins::TC);
}

void SceneGame::Release()
{
	monsterPool.Release();
	itemPool.Release();

	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
}

void SceneGame::Enter()
{
	size = FRAMEWORK.GetWindowSize();
	centerPos = size * 0.5f;

	Scene::Enter();

	/*----����� ����----*/
	stage1Bgm.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sounds/OverWorld.wav"));
	stage1Bgm.setLoop(true);

	pDieSound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sounds/PlayerDie.wav"));
	pDieSound.setLoop(false);
	pDieSound.setVolume(50.f);

	stage1Bgm.play();
	stage1Bgm.setVolume(40.f);

	/*---����Ʈ ����---*/
	float viewRatioY = 0.7f;
	float viewRatioX = 0.4f;

	worldView.setViewport(sf::FloatRect(0.5f - viewRatioX / 2.f, 0.5f - viewRatioY / 2.f, viewRatioX, viewRatioY));
	worldView.setCenter(tileMap1->GetPosition());
	
	uiView.setCenter(tileMap1->GetPosition());

	/*--���� �ʱ�ȭ*/
	lifeCount = 3;
	coinCount = 0;
	isGameOver = false;
	pickedItem = nullptr;
	roundClear = false;
	haveItem = false;
	clearRound = 0;
	bootsLevel = 0;
	gunLevel = 0;
	shotLevel = 0;
	isUpgrade = false;

	/*----UI����----*/
	sf::Vector2f mapPosition = tileMap1->GetPosition();
	sf::Vector2f tileSize = tileMap1->GetTileMapSize();
	sf::Vector2f sizeT = tileMap1->GetTileSize();

	itemUI->SetPosition(mapPosition.x - tileSize.x / 2 - 25, mapPosition.y - tileSize.y / 2 + 22);
	lifeUI->SetPosition(itemUI->GetPosition().x - 30, itemUI->GetPosition().y + 40);
	coinUI->SetPosition(lifeUI->GetPosition().x, lifeUI->GetPosition().y + 30);
	shotUI->SetPosition(mapPosition.x + tileSize.x / 2 + 10, mapPosition.y - tileSize.y / 2 + 10);
	timerUI->SetPosition(mapPosition.x - tileSize.x / 2 + 9, mapPosition.y - tileSize.y / 2 - 20);
	keyUI->SetPosition(mapPosition.x, mapPosition.y + tileSize.y / 2 + 55);
	pickedItemUI->SetPosition(itemUI->GetPosition());
	arrow->SetPosition(mapPosition.x, mapPosition.y + tileSize.y / 2 - 48);
	merchantTable->SetPosition(mapPosition.x - tileSize.x / 2 + sizeT.x * 7, mapPosition.y - tileSize.y / 2 + sizeT.y * 6);
	
	/*----���� ����----*/
	boots->SetPosition(merchantTable->GetPosition().x + 21.f, merchantTable->GetPosition().y + 20.f);
	gun->SetPosition(merchantTable->GetPosition().x + 63.f, merchantTable->GetPosition().y + 20.f);
	shot->SetPosition(merchantTable->GetPosition().x + 105.f, merchantTable->GetPosition().y + 20.f);

	boots->SetActive(false);
	gun->SetActive(false);
	shot->SetActive(false);

	/*----txt ����----*/
	coinTxt->SetPosition(coinUI->GetPosition().x + 20, coinUI->GetPosition().y - 20);
	string ss = "X " + to_string(lifeCount);
	lifeTxt->SetText(ss, 25, sf::Color::White, Origins::ML, 100, 0, 0);
	lifeTxt->text.setOutlineThickness(0.5f);
	lifeTxt->text.setOutlineColor(sf::Color::White);
	lifeTxt->SetPosition(lifeUI->GetPosition().x + 20, lifeUI->GetPosition().y - 20);

	bootsTxt->SetPosition(boots->GetPosition().x - 8.f, boots->GetPosition().y + 10.f);
	gunTxt->SetPosition(gun->GetPosition().x - 8.f, gun->GetPosition().y + 10.f);
	shotTxt->SetPosition(shot->GetPosition().x - 8.f, shot->GetPosition().y + 10.f);

	bootsTxt->SetActive(false);
	gunTxt->SetActive(false);
	shotTxt->SetActive(false);

	/*---timer ����----*/
	timerGauge->rectangle.setSize(timersize);
	timerGauge->SetPosition(timerUI->GetPosition().x + 21, timerUI->GetPosition().y);
	isTimerRunning = true;
	currentTime = 0.0f;
	timerM = 0.0f;
	
	/*---�÷��̾� ����----*/
	Utils::SetOrigin(cowBoy->head, Origins::MC);
	Utils::SetOrigin(cowBoy->leg, Origins::TC);
	cowBoy->SetPosition(tileMap1->GetPosition());
	cowBoy->SetTileMap(tileMap1, 32);

	merchant->SetTileMap(tileMap1);
	merchant->Reset();
	merchant->SetActive(false);

	/*----�� ����----*/
	//tileMap1->SetActive(true);
	//tileMap2->SetActive(true); 
	//tileMap3->SetActive(false);
	//tileMap4->SetActive(false);
	//tileMap5->SetActive(false);

	currentMap = tileMap1;

	/*----���� ����----*/
	for (auto monster : monsterPool.GetPool())
	{
		monster->SetTileMap(tileMap1, 32);
	}

	/*---���� ���� ��ġ ����----*/
	SetSpawnMonsterPos(tileMap1);

	RectangleGo* rect = (RectangleGo*)FindGo("center");
	rect->sortLayer = 5;
	rect->SetOrigin(Origins::MC);
	rect->SetPosition(tileMap1->GetPosition());
	rect->rectangle.setFillColor(sf::Color::Magenta);
}

void SceneGame::Exit()
{
	stage1Bgm.stop();

	ClearObjectPool(monsterPool);
	ClearObjectPool(itemPool);
	cowBoy->Reset();
	
	Scene::Exit();
}

void SceneGame::Update(float dt)
{
	Scene::Update(dt);
	//worldView.setCenter(cowBoy->GetPosition());

	//cout << cowBoy->GetPosition().x << ", " << cowBoy->GetPosition().y << endl;

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Escape))
	{
		SCENE_MGR.ChangeScene(SceneId::GameOver);
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num0))
	{
		UseNiza();
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num1))
	{
		SpawnMonster(1);
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num2))
	{
		GetCoin();
	}
	//������ ���
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Space) && haveItem)
	{
		if (pickedItem != nullptr)
		{
			if (pickedItem->GetType() == Item::ItemTypes::Nuke)
			{
				UseNuke();
			}
			else if (pickedItem->GetType() == Item::ItemTypes::NinzaBox)
			{
				UseNiza();
			}
			else
			{
				cowBoy->TakeItem(pickedItem->GetType());
			}
			pickedItemUI->SetActive(false);
			haveItem = false;
		}
	}
	if (currentTime >= timeLimit)
	{
		isTimerRunning = false;
		currentTime = 0.0f;
		timerGauge->rectangle.setSize({ 0, 10 });
		
	}
	/*--Ÿ�̸� ������ ����--*/
	if(isTimerRunning)
	{
		currentTime += dt;
		float decreaseWidth = timerDecreaseAmount * currentTime;
		float newWidth = initualWidth - decreaseWidth;
		timerGauge->rectangle.setSize(sf::Vector2f(newWidth, timerGauge->rectangle.getSize().y));
		timerM += dt;
		if (timerM >= mosterSpawnLimit)
		{
			int num = Utils::RandomRange(1, 3);
			SpawnMonster(num);
			timerM = 0.f;
		}
	}
	else
	{
		if(!roundClear && currentTime > 0) //�÷��̾ �׾��� ��
		{
			timerR += dt;
			if (stage1Bgm.getStatus() == sf::SoundSource::Status::Playing)
			{
				stage1Bgm.stop();
			}
			if (pDieSound.getStatus() == sf::SoundSource::Status::Stopped && timerR < 2.f)
			{
				pDieSound.play();
			}
			OnDieCowBoy();
			
			if (timerR >= reviveLimit)
			{
				timerR = 0.f;
				OnReviveCowBoy();
				isTimerRunning = true;
			}
		}
		else
		{
			if (monsterPool.GetUseList().empty() && !roundClear)
			{
				sf::Vector2f sizeT = currentMap->GetTileSize();
				merchant->SetTileMap(currentMap);
				merchant->SetSpawnPosition();
				merchantTable->SetPosition(currentMap->vertexArray.getBounds().left + sizeT.x * 7, currentMap->vertexArray.getBounds().top + sizeT.y * 6);
				
				boots->SetPosition(merchantTable->GetPosition().x + 21.f, merchantTable->GetPosition().y + 20.f);
				gun->SetPosition(merchantTable->GetPosition().x + 63.f, merchantTable->GetPosition().y + 20.f);
				shot->SetPosition(merchantTable->GetPosition().x + 105.f, merchantTable->GetPosition().y + 20.f);

				bootsTxt->SetPosition(boots->GetPosition().x - 8.f, boots->GetPosition().y + 10.f);
				gunTxt->SetPosition(gun->GetPosition().x - 8.f, gun->GetPosition().y + 10.f);
				shotTxt->SetPosition(shot->GetPosition().x - 8.f, shot->GetPosition().y + 10.f);

				roundClear = true;
				switch (currentStage)
				{
				case 1:
					currentMap = tileMap2;
					currentStage = 2;
					break;
				case 2:
					currentMap = tileMap3;
					currentStage = 3;
					break;
				case 3:
					currentMap = tileMap4;
					currentStage = 4;
					break;
				case 4:
					currentMap = tileMap5;
					currentStage = 5;
					break;
				}
			}
		}
		/*��Ȱ �� �÷��̾� blink �߰� �ʿ�
		if (timerR == 0.f)
		{
			timerM += dt;
			BlinkCowboy();
			if (timerM >= reviveLimit)
			{
				timerM = 0.f;
				isTimerRunning = true;
				cowBoy->SetActive(true);
			}
		}*/
	}
	if (roundClear) //���� Ŭ����
	{
		//������ ����
		list<Item*> items = itemPool.GetUseList();
		if (!items.empty())
		{
			for (auto item : items)
			{
				RemoveItem(item);
			}
		}
		//���� ����
		list<Monster*> monsters = monsterPool.GetUseList();
		if (!monsters.empty())
		{
			for (auto monster : monsters)
			{
				RemoveGo(monster);
				monsterPool.Return(monster);
			}
		}
		//�� ����
		if (!currentMap->isLood)
		{
			switch (currentStage)
			{
			case 2:
				currentMap->Load("maps/stage1-2.csv");
				currentMap->SetOrigin(Origins::MC);
				currentMap->SetPosition(tileMap1->GetPosition().x, tileMap1->GetPosition().y + tileMap1->GetTileMapSize().y);
				break;
			case 3:
				currentMap->Load("maps/stage1-3.csv");
				currentMap->SetOrigin(Origins::MC);
				currentMap->SetPosition(tileMap2->GetPosition().x, tileMap2->GetPosition().y + tileMap2->GetTileMapSize().y);
				break;
			case 4:
				currentMap->Load("maps/stage1-4.csv");
				currentMap->SetOrigin(Origins::MC);
				currentMap->SetPosition(tileMap3->GetPosition().x, tileMap3->GetPosition().y + tileMap3->GetTileMapSize().y);
				break;
			case 5:
				currentMap->Load("maps/stage1-5.csv");
				currentMap->SetOrigin(Origins::MC);
				currentMap->SetPosition(tileMap4->GetPosition().x, tileMap4->GetPosition().y + tileMap4->GetTileMapSize().y);
				break;
			default:
				break;
			}
			//���� ���� ���� ����
			SetSpawnMonsterPos(currentMap);

			//�÷��̾�, ���� tilemap ����
			cowBoy->SetTileMap(currentMap, 32);
			for (auto monster : monsterPool.GetPool())
			{
				monster->SetTileMap(currentMap, 32);
			}
			arrow->SetActive(true);
			clearRound++;
			if (clearRound > 2)
			{
				clearRound = 1;
				merchant->isArrival = false;
				merchant->SetActive(false);
				merchantTable->SetActive(false);
				boots->SetActive(false);
				gun->SetActive(false);
				shot->SetActive(false);
				bootsTxt->SetActive(false);
				gunTxt->SetActive(false);
				shotTxt->SetActive(false);
			}
		}
		if(clearRound==2)
		{
			merchant->SetActive(true);
			if (!merchantTable->GetActive() && merchant->isArrival)
			{
				merchantTable->SetActive(true);
				boots->SetActive(true);
				gun->SetActive(true);
				shot->SetActive(true);
				bootsTxt->SetActive(true);
				gunTxt->SetActive(true);
				shotTxt->SetActive(true);
			}
			if (cowBoy->GetHitBox().getGlobalBounds().intersects(boots->sprite.getGlobalBounds()) && 
				boots->GetActive() && coinCount >= boots->getCost() && !isUpgrade)
			{
				coinCount -= boots->getCost();
				boots->SetLevel(++bootsLevel);
				TextGo* findText = (TextGo*)FindGo("coinTxt");
				findText->text.setString("X " + to_string(coinCount));
				if (bootsLevel < 3)
				{
					cowBoy->StatsUpgrade(Stats::StatsTypes::Boots);
				}
				else
				{
					GetLife();
				}
				isUpgrade = true;

			}
			else if (cowBoy->GetHitBox().getGlobalBounds().intersects(gun->sprite.getGlobalBounds()) &&
				gun->GetActive() && coinCount >= gun->getCost() && !isUpgrade)
			{
				coinCount -= gun->getCost();
				gun->SetLevel(++gunLevel);
				TextGo* findText = (TextGo*)FindGo("coinTxt");
				findText->text.setString("X " + to_string(coinCount));
				cowBoy->StatsUpgrade(Stats::StatsTypes::Gun);
				isUpgrade = true;
			}
			else if (cowBoy->GetHitBox().getGlobalBounds().intersects(shot->sprite.getGlobalBounds()) &&
				shot->GetActive() && coinCount >= shot->getCost() && !isUpgrade)
			{
				coinCount -= shot->getCost();
				shot->SetLevel(++shotLevel);
				TextGo* findText = (TextGo*)FindGo("coinTxt");
				findText->text.setString("X " + to_string(coinCount));
				cowBoy->StatsUpgrade(Stats::StatsTypes::Shot);
				isUpgrade = true;
			}
		}
		//�÷��̾� ��ġ �̵�
		if (arrow->sprite.getGlobalBounds().intersects(cowBoy->GetHitBox().getGlobalBounds()))
		{
			roundChange = true;
			arrow->SetPosition(currentMap->GetPosition().x, currentMap->GetPosition().y + currentMap->GetTileMapSize().y / 2 - currentMap->GetTileSize().y);
			arrow->SetActive(false);
		}
		if(roundChange)
		{
			//cout << "boy: " << cowBoy->GetPosition().y << endl;
			//cout << "map: " << currentMap->GetPosition().y << endl;
			if (cowBoy->GetPosition().y < currentMap->GetPosition().y)
			{
				cowBoy->RoundClearMove(dt);
			}
			else
			{
				//���� �� delete �߰� �ʿ�
				roundChange = false;
				timerGauge->rectangle.setSize(timersize);
			}
		}
		if(cowBoy->GetPosition().y >= currentMap->GetPosition().y)
		{
			currentTime += dt;
		}
		//���� �� �̵�
		if (worldView.getCenter().y <= currentMap->GetPosition().y && roundChange)
		{
			worldView.setCenter(worldView.getCenter().x, worldView.getCenter().y + 300 * dt);
		}
		//3�� �� �����
		if (currentTime >= 3.0f)
		{
			currentTime = 0.f;
			roundClear = false;
			isTimerRunning = true;
			isUpgrade = false;
		}
	}
	if (isGameOver)
	{
		SCENE_MGR.ChangeScene(SceneId::GameOver);
	}
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

void SceneGame::SpawnMonster(int count)
{
	int num = Utils::RandomRange(0, 4);
	vector<sf::Vector2f>* spawnPosList = nullptr;
	switch (num)
	{
	case 0:
		spawnPosList = &monsterSpawnPosTop;
		break;
	case 1:
		spawnPosList = &monsterSpawnPosLeft;
		break;
	case 2:
		spawnPosList = &monsterSpawnPosBottom; 
		break;
	case 3:
		spawnPosList = &monsterSpawnPosRight;
		break;
	default:
		cout << "ERR:: spawnmonster" << endl;
		break;
	}
	for (int i = 0; i < count; i++)
	{
		Monster* monster = monsterPool.Get();
		monster->SetPosition(spawnPosList->at(i).x, spawnPosList->at(i).y);
		cout << monster->GetPosition().x << "," << monster->GetPosition().y << endl;
		AddGo(monster);
	}
}

void SceneGame::OnDieMonster(Monster* monster)
{
	//������ ����
	int random = Utils::RandomRange(0, 9);
	if (random >= 0)
	{
		Item* item = itemPool.Get();
		item->SetPosition(monster->GetPosition());
		item->SetIsSpawn(true);
		AddGo(item);
	}
}

void SceneGame::OnDieCowBoy()
{
	if (lifeCount <= 0)
	{
		isGameOver = true;
	}
	else
	{
		isTimerRunning = false;
		cowBoy->CowBoyDie();
		const list<Monster*> monsters = monsterPool.GetUseList();
		if(!monsters.empty())
		{
			for (auto monster : monsters)
			{
				monster->OnDie();
			}
		}
		//������ ����
		list<Item*> items = itemPool.GetUseList();
		if (!items.empty())
		{
			for (auto item : items)
			{
				RemoveItem(item);
			}
		}
	}
}

void SceneGame::OnReviveCowBoy()
{
	lifeCount--;
	TextGo* findText = (TextGo*)FindGo("lifeTxt");
	findText->text.setString("X " + to_string(lifeCount));
	cowBoy->Reset();
	cowBoy->SetPosition(currentMap->GetPosition());
	if (pDieSound.getStatus() == sf::SoundSource::Status::Playing)
	{
		pDieSound.stop();
	}
	stage1Bgm.play();
}

const list<Monster*>* SceneGame::GetMonsterList() const
{
	return &monsterPool.GetUseList();
}

void SceneGame::SetSpawnMonsterPos(TileMap* tilemap)
{
	if (!monsterSpawnPosBottom.empty())
	{
		monsterSpawnPosBottom.clear();
	}
	if (!monsterSpawnPosTop.empty())
	{
		monsterSpawnPosTop.clear();
	}
	if (!monsterSpawnPosLeft.empty())
	{
		monsterSpawnPosLeft.clear();
	}
	if (!monsterSpawnPosRight.empty())
	{
		monsterSpawnPosRight.clear();
	}
	sf::Vector2f sizeT = tilemap->GetTileSize();
	sf::Vector2f tileMapLT(tilemap->vertexArray.getBounds().left, tilemap->vertexArray.getBounds().top);
	for (auto tile : tilemap->tiles)
	{
		if (tile.x == 0 && tile.texIndex < 2)
		{
			monsterSpawnPosLeft.push_back({ tileMapLT.x + sizeT.x / 2.f, tile.y * sizeT.y + sizeT.y / 2.f + tileMapLT.y });
		}
		if (tile.y == 0 && tile.texIndex < 2)
		{
			monsterSpawnPosTop.push_back({ tileMapLT.x + tile.x * sizeT.x + sizeT.x / 2, sizeT.y / 2 + tileMapLT.y });
		}
		if (tile.x == 15 && tile.texIndex < 2)
		{
			monsterSpawnPosRight.push_back({ tileMapLT.x + tile.x * sizeT.x + sizeT.x / 2, tile.y * sizeT.y + sizeT.y / 2.f + tileMapLT.y });
		}
		if (tile.y == 15 && tile.texIndex < 2)
		{
			monsterSpawnPosBottom.push_back({ tileMapLT.x + tile.x * sizeT.x + sizeT.x / 2, tile.y * sizeT.y + sizeT.y / 2.f + tileMapLT.y });
		}
	}
}

//void SceneGame::BlinkCowboy()
//{
//	if (!blinkTimeCheck && clock.getElapsedTime() >= blinkTime)
//	{
//		clock.restart();
//		blinkTimeCheck = true;
//		cowBoy->SetActive(false);
//	}
//
//	if (clock.getElapsedTime() >= blinkTime)
//	{
//		clock.restart();
//		cowBoy->SetActive(true);
//		blinkTimeCheck = false;
//	}
//}

void SceneGame::TakeItem(Item* item)
{
	if (item->GetType() == Item::ItemTypes::Coin)
	{
		GetCoin();
	}
	else if (item->GetType() == Item::ItemTypes::Life)
	{
		GetLife();
	}
	else
	{
		if (!haveItem)
		{
			pickedItem = item;

			string texId = item->textureId;
			sf::Texture* texture = RESOURCE_MGR.GetTexture(texId);
			pickedItemUI->sprite.setTexture(*texture);

			pickedItemUI->SetActive(true);
			haveItem = true;
		}
		else
		{
			if (item->GetType() == Item::ItemTypes::Nuke)
			{
				UseNuke();
			}
			else if (item->GetType() == Item::ItemTypes::NinzaBox)
			{
				UseNiza();
			}
			else
			{
				cowBoy->TakeItem(item->GetType());
			}
		}
	}
	RemoveItem(item);
}

void SceneGame::GetCoin()
{
	coinCount++;
	TextGo* findText = (TextGo*)FindGo("coinTxt");
	findText->text.setString("X " + to_string(coinCount));
}

void SceneGame::GetLife()
{
	lifeCount++;
	TextGo* findText = (TextGo*)FindGo("lifeTxt");
	findText->text.setString("X " + to_string(lifeCount));
}

void SceneGame::RemoveItem(Item* item)
{
	item->SetIsSpawn(false);
	RemoveGo(item); //����Ʈ���� ����
	itemPool.Return(item);
}

void SceneGame::UseNiza()
{
	int tileNum;

	//�̵� ������ Ÿ���� ���� ������ ����
	do {
		tileNum = Utils::RandomRange(0, currentMap->tiles.size() + 1);
	} while (currentMap->tiles[tileNum].texIndex < 2 || currentMap->tiles[tileNum].texIndex > 4);

	sf::Vector2f newPos(currentMap->vertexArray.getBounds().left + currentMap->tiles[tileNum].x * 32.f + 16.f,
		currentMap->vertexArray.getBounds().top + currentMap->tiles[tileNum].y * 32.f + 32.f);
	cowBoy->SetPosition(newPos.x, newPos.y - 4.f);
	cout << "new: " << newPos.x << ", " << newPos.y <<","<<tileNum <<"," << currentMap->tiles[tileNum].texIndex << endl;
}

void SceneGame::UseNuke()
{
	for (auto monster : monsterPool.GetUseList())
	{
		monster->OnBoom();
	}
}

bool SceneGame::GetRoundChange()
{
	return roundChange;
}
