#include "stdafx.h"
#include "SceneMgr.h"
#include "SceneGame.h"
#include "SceneTitle.h"
#include "DataTableMgr.h"
#include "StringTable.h"
#include "SceneEnding.h"

void SceneMgr::Init()
{
	if (!scenes.empty())
	{
		Release();
	}
	scenes.push_back(new SceneTitle());
	scenes.push_back(new SceneGame());
	scenes.push_back(new SceneOver());

	for (auto scene : scenes)
	{
		scene->Init();
	}

	currentSceneId = startSceneId;
	currentScene = scenes[(int)currentSceneId];
	currentScene->Enter();
}

void SceneMgr::Release()
{
	if (scenes.empty())
	{
		return;
	}

	for (auto scene : scenes)
	{
		//scene->Release();
		delete scene;
	}
	scenes.clear();

	currentSceneId = SceneId::None;
	currentScene = nullptr;
}

void SceneMgr::UpdateEvent(float dt)
{
	currentScene->Update(dt);
}

void SceneMgr::Draw(sf::RenderWindow& window)
{
	currentScene->Draw(window);
}

void SceneMgr::ChangeScene(SceneId id)
{
	currentScene->Exit();
	currentSceneId = id;
	currentScene = scenes[(int)currentSceneId];
	currentScene->Enter();
}

Scene* SceneMgr::GetCurrScene() const
{
	return currentScene;
}
