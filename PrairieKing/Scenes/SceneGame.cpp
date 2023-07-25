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

	/*----����� ����----*/
	stage1BgmBuffer.loadFromFile("sounds/OverWorld.wav");
	stage1Bgm.setBuffer(stage1BgmBuffer);
	stage1Bgm.setLoop(true);

	pDieSoundBuffer.loadFromFile("sounds/PlayerDie.wav");
	pDieSound.setBuffer(pDieSoundBuffer);
	pDieSound.setLoop(false);

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
	cowBoy->sortLayer = 3;

	/*----UI����----*/
	coinUI = (SpriteGo*)AddGo(new SpriteGo("graphics/UIs/coin1.png", "coinUI"));
	itemUI = (SpriteGo*)AddGo(new SpriteGo("graphics/UIs/itemUI.png", "itemUI"));
	lifeUI = (SpriteGo*)AddGo(new SpriteGo("graphics/UIs/life.png", "lifeUI"));
	shotUI = (SpriteGo*)AddGo(new SpriteGo("graphics/UIs/shot.png", "shotUI"));
	timerUI = (SpriteGo*)AddGo(new SpriteGo("graphics/UIs/timerUI.png", "timerUI"));
	keyUI = (SpriteGo*)AddGo(new SpriteGo("graphics/UIs/controlKey.png", "keyUI"));
	pickedItemUI = (SpriteGo*)AddGo(new SpriteGo("graphics/items/coffee.png", "pickedItemUI"));

	coinUI->sprite.setScale({ 2.f, 2.f });
	itemUI->sprite.setScale({ 2.f, 2.f });
	lifeUI->sprite.setScale({ 2.f, 2.f });
	shotUI->sprite.setScale({ 2.f, 2.f });
	timerUI->sprite.setScale({ 2.f, 2.f });
	keyUI->sprite.setScale({ 2.f, 2.f }); 
	pickedItemUI->sprite.setScale({ 2.f, 2.f });

	coinUI->sortLayer = 100;
	itemUI->sortLayer = 100;
	lifeUI->sortLayer = 100;
	shotUI->sortLayer = 100;
	timerUI->sortLayer = 100;
	keyUI->sortLayer = 100;
	pickedItemUI->sortLayer = 100;

	pickedItemUI->SetActive(false);

	/*----�ؽ�Ʈ ����----*/
	coinTxt = (TextGo*)AddGo(new TextGo("coinTxt", "fonts/angel.ttf"));
	std::string ss = "X " + to_string(coinCount);
	coinTxt->SetText(ss, 25, sf::Color::White, Origins::ML, 100, 0, 0);
	coinTxt->text.setOutlineThickness(0.5f);
	coinTxt->text.setOutlineColor(sf::Color::White);

	lifeTxt = (TextGo*)AddGo(new TextGo("lifeTxt", "fonts/angel.ttf"));

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

	stage1Bgm.play();
	stage1Bgm.setVolume(40.f);

	Scene::Enter();

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

	/*----UI����----*/
	sf::Vector2f mapPosition = tileMap1->GetPosition();
	//sf::Vector2f tileSize(512, 512);
	sf::Vector2f tileSize = tileMap1->GetTileMapSize();

	itemUI->SetPosition(mapPosition.x - tileSize.x / 2 - 25, mapPosition.y - tileSize.y / 2 + 22);
	lifeUI->SetPosition(itemUI->GetPosition().x - 30, itemUI->GetPosition().y + 40);
	coinUI->SetPosition(lifeUI->GetPosition().x, lifeUI->GetPosition().y + 30);
	shotUI->SetPosition(mapPosition.x + tileSize.x / 2 + 10, mapPosition.y - tileSize.y / 2 + 10);
	timerUI->SetPosition(mapPosition.x - tileSize.x / 2 + 9, mapPosition.y - tileSize.y / 2 - 20);
	keyUI->SetPosition(mapPosition.x, mapPosition.y + tileSize.y / 2 + 55);
	pickedItemUI->SetPosition(itemUI->GetPosition());
	
	/*----txt ����----*/
	coinTxt->SetPosition(coinUI->GetPosition().x + 20, coinUI->GetPosition().y - 20);
	string ss = "X " + to_string(lifeCount);
	lifeTxt->SetText(ss, 25, sf::Color::White, Origins::ML, 100, 0, 0);
	lifeTxt->text.setOutlineThickness(0.5f);
	lifeTxt->text.setOutlineColor(sf::Color::White);
	lifeTxt->SetPosition(lifeUI->GetPosition().x + 20, lifeUI->GetPosition().y - 20);

	/*---timer ����----*/
	timerGauge->rectangle.setSize(timersize);
	timerGauge->SetPosition(timerUI->GetPosition().x + 21, timerUI->GetPosition().y);
	isTimerRunning = true;
	currentTime = 0.0f;
	timerM = 0.0f;
	
	/*---�÷��̾� ����----*/
	Utils::SetOrigin(cowBoy->head, Origins::BC);
	Utils::SetOrigin(cowBoy->leg, Origins::TC);
	cowBoy->SetPosition(tileMap1->GetPosition());
	cowBoy->SetTileMap(tileMap1, 32);

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
		currentTime = 50000.f;
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
		currentTime = 0.0f;
		isTimerRunning = false;
		timerGauge->rectangle.setSize({ 0, 10 });
		if (monsterPool.GetUseList().empty())
		{
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
	/*--Ÿ�̸� ������ ����--*/
	if(isTimerRunning)
	{
		//currentTime += dt;
		float decreaseWidth = timerDecreaseAmount * currentTime;
		float newWidth = initualWidth - decreaseWidth;
		timerGauge->rectangle.setSize(sf::Vector2f(newWidth, timerGauge->rectangle.getSize().y));
		timerM += dt;
		if (timerM >= mosterSpawnLimit)
		{
			int num = Utils::RandomRange(1, 3);
			//SpawnMonster(num);
			timerM = 0.f;
		}
	}
	else
	{
		if(!roundClear) //�÷��̾ �׾��� ��
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

	 //���ѽð��� ����Ǹ� ���� ��ȯ���� ��ȯ �ʿ�
	
	if (roundClear) //���� Ŭ����
	{
		//������ ����
		list<Item*> items = itemPool.GetUseList();
		if (!items.empty())
		{
			for (auto item : items)
			{
				item->SetIsSpawn(false);
				RemoveGo(item);
				itemPool.Return(item);
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
		}
		//Ÿ�̸� �ʱ�ȭ
		timerGauge->rectangle.setSize(timersize);
		
		//�÷��̾� ��ġ �̵�
		if (cowBoy->GetPosition().y < currentMap->GetPosition().y)
		{
			cowBoy->RoundClearMove(dt);
		}
		else
		{
			//���� �� delete �߰� �ʿ�
			currentTime += dt;
		}
		if (worldView.getCenter().y <= currentMap->GetPosition().y)
		{
			worldView.setCenter(worldView.getCenter().x, worldView.getCenter().y + 200 * dt);
		}
		//3�� �� �����
		if (currentTime >= 3.0f)
		{
			currentTime = 0.f;
			roundClear = false;
			isTimerRunning = true;
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
				item->SetIsSpawn(false);
				RemoveGo(item);
				itemPool.Return(item);
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
		GetGoin();
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

void SceneGame::GetGoin()
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

bool SceneGame::GetRoundClear()
{
	return roundClear;
}
