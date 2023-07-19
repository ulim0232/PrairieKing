#include "stdafx.h"
#include "SceneEnding.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "Framework.h"
#include "GameObject.h"
#include "TextGo.h"
#include "DataTableMgr.h"
#include "StringTable.h"

SceneOver::SceneOver()
	:Scene(SceneId::GameOver)
{
	resourceListPath = "tables/scripts/SceneOverResourceList.csv";
}

void SceneOver::Init()
{
	Release();

	titleM = (TextGo*)AddGo(new TextGo("TitleM", "fonts/angel.ttf"));
	restartM = (TextGo*)AddGo(new TextGo("RestartM", "fonts/angel.ttf"));
	exitM = (TextGo*)AddGo(new TextGo("ExitM", "fonts/angel.ttf"));
	cursorM = (TextGo*)AddGo(new TextGo("cursorM", "fonts/angel.ttf"));
	
	for (auto go : gameObjects)
	{
		go->Init();
	}
}

void SceneOver::Release()
{
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
}

void SceneOver::Enter()
{
	Scene::Enter();

	auto size = FRAMEWORK.GetWindowSize();
	centerPos = size * 0.5f;

	worldView.setSize(size);
	worldView.setCenter(centerPos);

	stringTable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);

	titleM->SetText(stringTable->GetUni("TITLE", Languages::KOR), 80, sf::Color::White, Origins::MC, 1, centerPos.x, centerPos.y * 0.6);
	restartM->SetText(stringTable->GetUni("RESTART", Languages::KOR), 60, sf::Color::White, Origins::ML, 1, centerPos.x, centerPos.y);
	exitM->SetText(stringTable->GetUni("EXIT", Languages::KOR), 60, sf::Color::White, Origins::ML, 1, centerPos.x, centerPos.y * 1.2);
	cursorM->SetText(stringTable->Get("CURSOR", Languages::KOR), 50, sf::Color::White, Origins::MR, 1, centerPos.x, centerPos.y);
}

void SceneOver::Exit()
{
	Scene::Exit();
}

void SceneOver::Update(float dt)
{
	Scene::Update(dt);

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Up))
	{
		cursorM->SetPosition(centerPos.x, centerPos.y);
		mode = Mode::Restart;
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Down))
	{
		cursorM->SetPosition(centerPos.x, centerPos.y * 1.2);
		mode = Mode::Exit;
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Enter))
	{
		if (mode == Mode::Restart)
		{
			SCENE_MGR.ChangeScene(SceneId::Game);
		}
		else
		{
			window.close();
		}
	}
	
}

void SceneOver::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

