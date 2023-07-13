#include "stdafx.h"
#include "Framework.h"
#include "SceneTitle.h"
#include "GameObject.h"
#include "SpriteGo.h"
#include "InputMgr.h"
#include "SceneMgr.h"

SceneTitle::SceneTitle()
	:Scene(SceneId::Title)
{
	resourceListPath = "tables/SceneTitleResourceList.csv";
}

void SceneTitle::Init()
{
	Release();

	auto size = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = size * 0.5f;

	worldView.setSize(size);
	worldView.setCenter(centerPos);

	titleIcon = (SpriteGo*)AddGo(new SpriteGo("graphics/title.png", "titleIcon"));
	titleIcon->sprite.setScale(4.f, 4.f);
	titleIcon->SetOrigin(Origins::TC);
	titleIcon->sortLayer = -1;
	titleIcon->SetPosition(centerPos.x, size.y);
	//titleIcon->SetPosition(centerPos.x, centerPos.y * 0.6f);

	titleBgmBuffer.loadFromFile("sounds/Title.wav");
	titleBgm.setBuffer(titleBgmBuffer);
	titleBgm.setLoop(true);

	for (auto go : gameObjects)
	{
		go->Init();
	}
}

void SceneTitle::Release()
{
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
}

void SceneTitle::Enter()
{
	auto size = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = size * 0.5f;

	uiView.setSize(size);
	uiView.setCenter(centerPos);

	Scene::Enter();

	titleBgm.play();
	titleBgm.setVolume(40.f);

}

void SceneTitle::Exit()
{
	Scene::Exit();
}

void SceneTitle::Update(float dt)
{
	Scene::Update(dt);
	sf::Vector2f centerPos = FRAMEWORK.GetWindowSize() * 0.5f;

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Space))
	{
		SCENE_MGR.ChangeScene(SceneId::Game);
	}
	
	if (titleIcon->GetPosition().y >= centerPos.y * 0.6f)
	{
		float speed = 100.0f;
		titleIcon->SetPosition(titleIcon->GetPosition().x, titleIcon->GetPosition().y - speed * dt);
	}
	//titleIcon->SetPosition(centerPos.x, centerPos.y * 0.9);
}

void SceneTitle::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
