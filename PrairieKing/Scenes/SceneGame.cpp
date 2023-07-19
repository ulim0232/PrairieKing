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

	/*----���� ��ġ ����, ���� �ʿ�----*/
	//��
	monsterSpawnPosTop.push_back({ 240, 16 });
	monsterSpawnPosTop.push_back({ 272, 16 });
	monsterSpawnPosTop.push_back({ 304, 16 });

	//�Ʒ�
	monsterSpawnPosBottom.push_back({ 240, 496 });
	monsterSpawnPosBottom.push_back({ 272, 496 });
	monsterSpawnPosBottom.push_back({ 304, 496 });

	//����
	monsterSpawnPosLeft.push_back({ 16, 240 });
	monsterSpawnPosLeft.push_back({ 16, 272 });
	monsterSpawnPosLeft.push_back({ 16, 304 });

	//������
	monsterSpawnPosRight.push_back({ 496, 240 });
	monsterSpawnPosRight.push_back({ 496, 272 });
	monsterSpawnPosRight.push_back({ 496, 304 });
}



void SceneGame::Init()
{
	Release();

	auto size = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = size * 0.5f;
	
	worldView.setSize(size);
	worldView.setCenter(centerPos);

	uiView.setSize(size);
	uiView.setCenter(centerPos);

	/*----bgm ����----*/
	stage1BgmBuffer.loadFromFile("sounds/OverWorld.wav");
	stage1Bgm.setBuffer(stage1BgmBuffer);
	stage1Bgm.setLoop(true);

	/*----�� ����----*/
	tileMap1 = (TileMap*)AddGo(new TileMap("graphics/maps/map_sheet_stage1.png", "TileMap1"));
	tileMap2 = (TileMap*)AddGo(new TileMap("graphics/maps/map_sheet_stage1.png", "TileMap2"));
	tileMap3 = (TileMap*)AddGo(new TileMap("graphics/maps/map_sheet_stage1.png", "TileMap3"));
	tileMap4 = (TileMap*)AddGo(new TileMap("graphics/maps/map_sheet_stage1.png", "TileMap4"));
	tileMap5 = (TileMap*)AddGo(new TileMap("graphics/maps/map_sheet_stage1.png", "TileMap5"));
	tileMap1->sortLayer = -1;

	/*---�÷��̾� ����----*/
	cowBoy = (CowBoy*)AddGo(new CowBoy("cowBoy"));

	/*----UI����----*/
	coinUI = (SpriteGo*)AddGo(new SpriteGo("graphics/UIs/coin1.png", "coinUI"));
	itemUI = (SpriteGo*)AddGo(new SpriteGo("graphics/UIs/itemUI.png", "itemUI"));
	lifeUI = (SpriteGo*)AddGo(new SpriteGo("graphics/UIs/life.png", "lifeUI"));
	shotUI = (SpriteGo*)AddGo(new SpriteGo("graphics/UIs/shot.png", "shotUI"));
	timerUI = (SpriteGo*)AddGo(new SpriteGo("graphics/UIs/timerUI.png", "timerUI"));
	keyUI = (SpriteGo*)AddGo(new SpriteGo("graphics/UIs/controlKey.png", "keyUI"));

	coinUI->sprite.setScale({ 2.f, 2.f });
	itemUI->sprite.setScale({ 2.f, 2.f });
	lifeUI->sprite.setScale({ 2.f, 2.f });
	shotUI->sprite.setScale({ 2.f, 2.f });
	timerUI->sprite.setScale({ 2.f, 2.f });
	keyUI->sprite.setScale({ 2.f, 2.f });

	coinUI->sortLayer = 100;
	itemUI->sortLayer = 100;
	lifeUI->sortLayer = 100;
	shotUI->sortLayer = 100;
	timerUI->sortLayer = 100;
	keyUI->sortLayer = 100;

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

	for (auto go : gameObjects)
	{
		go->Init();
	}

	/*----�� ����----*/
	tileMap1->Load("maps/stage1-1.csv");
	tileMap1->SetOrigin(Origins::MC);
	tileMap1->SetPosition(centerPos);

	tileMap2->Load("maps/stage1-2.csv");
	tileMap2->SetOrigin(Origins::MC);
	tileMap2->SetPosition(centerPos);
	tileMap2->SetActive(false);

	tileMap3->Load("maps/stage1-3.csv");
	tileMap3->SetOrigin(Origins::MC);
	tileMap3->SetPosition(centerPos);
	tileMap3->SetActive(false);

	tileMap4->Load("maps/stage1-4.csv");
	tileMap4->SetOrigin(Origins::MC);
	tileMap4->SetPosition(centerPos);
	tileMap4->SetActive(false);

	tileMap5->Load("maps/stage1-5.csv");
	tileMap5->SetOrigin(Origins::MC);
	tileMap5->SetPosition(centerPos);
	tileMap5->SetActive(false);

	/*---�÷��̾� ����----*/
	cowBoy->sortLayer = 2;

	/*----UI����----*/
	coinUI->SetOrigin(Origins::MC);
	itemUI->SetOrigin(Origins::MC);
	lifeUI->SetOrigin(Origins::MC);
	shotUI->SetOrigin(Origins::MC);
	timerUI->SetOrigin(Origins::MC);
	keyUI->SetOrigin(Origins::MC);

	/*----���� ����----*/
	monsterPool.OnCreate = [this](Monster* monster)
	{
		//Monster::Types monsterType = (Monster::Types)(Utils::RandomRange(0, Monster::TotalTypes) + 1); //1���� ����
		Monster::Types monsterType = (Monster::Types)Utils::RandomRange(4, 6);
		monster->SetType(monsterType);
		monster->SetCowboy(cowBoy);
		monster->SetPool(&monsterPool);
		monster->SetTileMap(tileMap1, 32);
		monster->sortLayer = 2; //�÷��̾�� ����
	};
	monsterPool.Init();

}

void SceneGame::Release()
{
	monsterPool.Release();

	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
}

void SceneGame::Enter()
{
	auto size = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = size * 0.5f;

	stage1Bgm.play();
	stage1Bgm.setVolume(40.f);

	Scene::Enter();

	worldView.setCenter(tileMap1->GetPosition());
	uiView.setCenter(tileMap1->GetPosition());
	cout << tileMap1->GetPosition().x << ", "<<tileMap1->GetPosition().y << endl;

	lifeCount = 3;
	coinCount = 0;
	isGameOver = false;

	/*----UI����----*/
	sf::Vector2f mapPosition = tileMap1->GetPosition();
	sf::Vector2f tileSize = tileMap1->GetTileSize();

	itemUI->SetPosition(mapPosition.x - tileSize.x / 2 - 25, mapPosition.y - tileSize.y / 2 + 22);
	lifeUI->SetPosition(itemUI->GetPosition().x - 30, itemUI->GetPosition().y + 40);
	coinUI->SetPosition(lifeUI->GetPosition().x, lifeUI->GetPosition().y + 30);
	shotUI->SetPosition(mapPosition.x + tileSize.x / 2 + 10, mapPosition.y - tileSize.y / 2 + 10);
	timerUI->SetPosition(mapPosition.x - tileSize.x / 2 + 9, mapPosition.y - tileSize.y / 2 - 20);
	keyUI->SetPosition(mapPosition.x, mapPosition.y + tileSize.y / 2 + 55);

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

	left = RESOURCE_MGR.GetTexture("graphics/players/Player_left.png");
	right = RESOURCE_MGR.GetTexture("graphics/players/Player_right.png");
	back = RESOURCE_MGR.GetTexture("graphics/players/Player_back.png");
	front = RESOURCE_MGR.GetTexture("graphics/players/Player_front.png");
	origin = RESOURCE_MGR.GetTexture("graphics/players/Player_stand.png");

	blinkTimeCheck = false;

}

void SceneGame::Exit()
{
	stage1Bgm.stop();

	ClearObjectPool(monsterPool);
	cowBoy->Reset();
	
	Scene::Exit();
}

void SceneGame::Update(float dt)
{
	Scene::Update(dt);
	
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Escape))
	{
		SCENE_MGR.ChangeScene(SceneId::GameOver);
	}

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num1))
	{
		tileMap1->SetActive(true);
		tileMap2->SetActive(false);
		tileMap3->SetActive(false);
		tileMap4->SetActive(false);
		tileMap5->SetActive(false);
		cowBoy->SetTileMap(tileMap1, 32);
		for (auto monster : monsterPool.GetUseList())
		{
			monster->SetTileMap(tileMap1, 32);
		}
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num2))
	{
		tileMap1->SetActive(false);
		tileMap2->SetActive(true);
		tileMap3->SetActive(false);
		tileMap4->SetActive(false);
		tileMap5->SetActive(false);
		cowBoy->SetTileMap(tileMap2, 32);
		for (auto monster : monsterPool.GetUseList())
		{
			monster->SetTileMap(tileMap2, 32);
		}
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num3))
	{
		tileMap1->SetActive(false);
		tileMap2->SetActive(false);
		tileMap3->SetActive(true);
		tileMap4->SetActive(false);
		tileMap5->SetActive(false);
		cowBoy->SetTileMap(tileMap3, 32);
		for (auto monster : monsterPool.GetUseList())
		{
			monster->SetTileMap(tileMap3, 32);
		}
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num4))
	{
		tileMap1->SetActive(false);
		tileMap2->SetActive(false);
		tileMap3->SetActive(false);
		tileMap4->SetActive(true);
		tileMap5->SetActive(false);
		cowBoy->SetTileMap(tileMap4, 32);
		for (auto monster : monsterPool.GetUseList())
		{
			monster->SetTileMap(tileMap4, 32);
		}
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num5))
	{
		tileMap1->SetActive(false);
		tileMap2->SetActive(false);
		tileMap3->SetActive(false);
		tileMap4->SetActive(false);
		tileMap5->SetActive(true);
		cowBoy->SetTileMap(tileMap5, 32);
		cowBoy->SetPosition(tileMap5->GetPosition().x, tileMap5->GetPosition().y - 20.f);
		for (auto monster : monsterPool.GetUseList())
		{
			monster->SetTileMap(tileMap5, 32);
		}
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
		BlinkCowboy();
		timerR += dt;
		if (timerR >= reviveLimit)
		{
			timerR = 0.f;
			isTimerRunning = true;
			cowBoy->SetActive(true);
		}
	}

	 //5�ʰ� ������ ���� ��ȯ���� ��ȯ �ʿ�
	if (currentTime >= timeLimit)
	{
		currentTime = 0.0f;
		isTimerRunning = false;
		timerGauge->rectangle.setSize({ 0, 10 });
	}


	//�÷��̾� �ؽ��� ����
	if (INPUT_MGR.GetKey(sf::Keyboard::Right))
	{
		if (right != nullptr)
		{
			cowBoy->head.setTexture(*right);
		}
	}
	if (INPUT_MGR.GetKey(sf::Keyboard::Left))
	{
		cowBoy->head.setTexture(*left);
	}
	if (INPUT_MGR.GetKey(sf::Keyboard::Up))
	{
		cowBoy->head.setTexture(*back);
	}
	if (INPUT_MGR.GetKey(sf::Keyboard::Down))
	{
		cowBoy->head.setTexture(*front);
	}
	if (INPUT_MGR.GetKeyUp(sf::Keyboard::Down) ||
		INPUT_MGR.GetKeyUp(sf::Keyboard::Up) ||
		INPUT_MGR.GetKeyUp(sf::Keyboard::Right) ||
		INPUT_MGR.GetKeyUp(sf::Keyboard::Left))
	{
		cowBoy->head.setTexture(*origin);
	}

	//���� ����
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num8))
	{
		int num = Utils::RandomRange(1, 3);
		SpawnMonster(num);
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
		spawnPosList = &monsterSpawnPosBottom;
		break;
	case 2:
		spawnPosList = &monsterSpawnPosLeft;
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
		monster->SetPosition(spawnPosList->at(i).x + 384, spawnPosList->at(i).y + 104);
		AddGo(monster);
	}
}

void SceneGame::OnDieMonster(Monster* monster)
{
	//monster ���ο��� �ϴ� ������ ����
	//RemoveGo(monster);
	//monsterPool.Return(monster);
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
		cowBoy->Reset();
		lifeCount--;
		TextGo* findText = (TextGo*)FindGo("lifeTxt");
		findText->text.setString("X " + to_string(lifeCount));
		for (auto monster : monsterPool.GetUseList())
		{
			monster->OnDie();
		}
		//monsterPool.Clear();
	}
}

const list<Monster*>* SceneGame::GetMonsterList() const
{
	return &monsterPool.GetUseList();
}

void SceneGame::BlinkCowboy()
{
	if (!blinkTimeCheck && clock.getElapsedTime() >= blinkTime)
	{
		clock.restart();
		blinkTimeCheck = true;
		cowBoy->SetActive(false);
	}

	if (clock.getElapsedTime() >= blinkTime)
	{
		clock.restart();
		cowBoy->SetActive(true);
		blinkTimeCheck = false;
	}
}
