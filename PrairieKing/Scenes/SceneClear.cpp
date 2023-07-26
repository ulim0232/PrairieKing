#include "stdafx.h"
#include "SceneClear.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "Framework.h"
#include "GameObject.h"
#include "SpriteGo.h"
#include "ResourceMgr.h"

SceneClear::SceneClear()
	:Scene(SceneId::Clear)
{
	resourceListPath = "tables/scripts/SceneClearResourceList.csv";
}

void SceneClear::Init()
{
	Release();

	auto size = FRAMEWORK.GetWindowSize();
	centerPos = size * 0.5f;

	worldView.setSize(size);
	worldView.setCenter(centerPos);

	/*---ending player ¼³Á¤---*/
	cowboy = (SpriteGo*)AddGo(new SpriteGo("graphics/players/Player_item.png", "cowboy"));
	cowboy->sprite.setScale(2.f, 2.f);
	cowboy->SetOrigin(Origins::TC);
	cowboy->sortLayer = 1;
	cowboy->SetPosition(centerPos);

	/*--item heart--*/
	clearHeart = (SpriteGo*)AddGo(new SpriteGo("graphics/items/clear.png", "heart"));
	clearHeart->sprite.setScale(2.f, 2.f);
	clearHeart->SetOrigin(Origins::BC);
	clearHeart->sortLayer = 1;
	clearHeart->SetPosition(centerPos);

	EndingIcon = (SpriteGo*)AddGo(new SpriteGo("graphics/UIs/ending.png", "ending"));
	EndingIcon->sprite.setScale(6.f, 6.f);
	EndingIcon->SetOrigin(Origins::TC);
	EndingIcon->sortLayer = 1;
	EndingIcon->SetPosition(centerPos.x, size.y);
	EndingIcon->SetActive(false);

	for (auto go : gameObjects)
	{
		go->Init();
	}
}

void SceneClear::Release()
{
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
}

void SceneClear::Enter()
{
	Scene::Enter();

	clearBgm.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sounds/gameClear.wav"));
	clearBgm.setLoop(true);
	clearBgm.play();
	clearBgm.setVolume(40.f);

	timer = 0.f;

}
void SceneClear::Exit()
{
	Scene::Exit();
	clearBgm.stop();
}

void SceneClear::Update(float dt)
{
	Scene::Update(dt);

	timer += dt;

	if (timer > 5.f)
	{
		cowboy->SetActive(false);
		clearHeart->SetActive(false);
		EndingIcon->SetActive(true);
		if (EndingIcon->GetPosition().y >= centerPos.y * 0.6f)
		{
			float speed = 100.0f;
			EndingIcon->SetPosition(EndingIcon->GetPosition().x, EndingIcon->GetPosition().y - speed * dt);
		}
	}
	if (timer > 35.f)
	{
		SCENE_MGR.ChangeScene(SceneId::GameOver);
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Enter))
	{
		SCENE_MGR.ChangeScene(SceneId::GameOver);
	}
}

void SceneClear::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
