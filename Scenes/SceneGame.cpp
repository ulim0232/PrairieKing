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

SceneGame::SceneGame() : Scene(SceneId::Game)
{
	//csv������ ���ؼ� �ε��ϴ� ������ ����
	resourceListPath = "tables/SceneGameResourceList.csv";
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
	tileMap1 = (TileMap*)AddGo(new TileMap("graphics/map_sheet_stage1.png", "TileMap1"));
	tileMap2 = (TileMap*)AddGo(new TileMap("graphics/map_sheet_stage1.png", "TileMap2"));
	tileMap3 = (TileMap*)AddGo(new TileMap("graphics/map_sheet_stage1.png", "TileMap3"));
	tileMap4 = (TileMap*)AddGo(new TileMap("graphics/map_sheet_stage1.png", "TileMap4"));
	tileMap5 = (TileMap*)AddGo(new TileMap("graphics/map_sheet_stage1.png", "TileMap5"));

	for (auto go : gameObjects)
	{
		go->Init();
	}

	/*----�� ����----*/
	tileMap1->Load("maps/stage1-1.csv");
	tileMap1->SetOrigin(Origins::MC);

	tileMap2->Load("maps/stage1-2.csv");
	tileMap2->SetOrigin(Origins::MC);
	tileMap2->SetActive(false);

	tileMap3->Load("maps/stage1-3.csv");
	tileMap3->SetOrigin(Origins::MC);
	tileMap3->SetActive(false);

	tileMap4->Load("maps/stage1-4.csv");
	tileMap4->SetOrigin(Origins::MC);
	tileMap4->SetActive(false);

	tileMap5->Load("maps/stage1-5.csv");
	tileMap5->SetOrigin(Origins::MC);
	tileMap5->SetActive(false);
}

void SceneGame::Release()
{
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
}

void SceneGame::Exit()
{
	stage1Bgm.stop();
	Scene::Exit();
}

void SceneGame::Update(float dt)
{
	Scene::Update(dt);

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num1))
	{
		tileMap1->SetActive(true);
		tileMap2->SetActive(false);
		tileMap3->SetActive(false);
		tileMap4->SetActive(false);
		tileMap5->SetActive(false);
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num2))
	{
		tileMap1->SetActive(false);
		tileMap2->SetActive(true);
		tileMap3->SetActive(false);
		tileMap4->SetActive(false);
		tileMap5->SetActive(false);
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num3))
	{
		tileMap1->SetActive(false);
		tileMap2->SetActive(false);
		tileMap3->SetActive(true);
		tileMap4->SetActive(false);
		tileMap5->SetActive(false);
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num4))
	{
		tileMap1->SetActive(false);
		tileMap2->SetActive(false);
		tileMap3->SetActive(false);
		tileMap4->SetActive(true);
		tileMap5->SetActive(false);
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num5))
	{
		tileMap1->SetActive(false);
		tileMap2->SetActive(false);
		tileMap3->SetActive(false);
		tileMap4->SetActive(false);
		tileMap5->SetActive(true);
	}
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
