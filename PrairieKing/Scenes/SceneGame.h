#pragma once
#include "Scene.h"
#include "ObjectPool.h"

class VertexArrayGo;
class CowBoy;
class TileMap;
class SpriteGo;
class TextGo;
class RectangleGo;
class Monster;

class SceneGame : public Scene
{
protected:
	sf::SoundBuffer stage1BgmBuffer;
	sf::Sound stage1Bgm;

	CowBoy* cowBoy;
	ObjectPool<Monster> monsterPool;

	sf::Texture* left = nullptr;
	sf::Texture* right = nullptr;
	sf::Texture* back = nullptr;
	sf::Texture* front = nullptr;
	sf::Texture* origin = nullptr;
	/*---map---*/
	TileMap* tileMap1 = nullptr;
	TileMap* tileMap2 = nullptr;
	TileMap* tileMap3 = nullptr;
	TileMap* tileMap4 = nullptr;
	TileMap* tileMap5 = nullptr;

	/*---UI---*/
	SpriteGo* coinUI;
	SpriteGo* itemUI;
	SpriteGo* lifeUI;
	SpriteGo* shotUI;
	SpriteGo* timerUI;
	SpriteGo* keyUI;

	TextGo* coinTxt;
	TextGo* lifeTxt;

	RectangleGo* timerGauge;

	int lifeCount = 3;
	int coinCount = 0;

	float timerDecreaseRate;
	float timerDecreaseAmount;

	float currentTime = 0;
	float initualWidth = 482.f;
	float timeLimit = 5.0f;

	bool isTimerRunning = true;
	vector<sf::Vector2f> monsterSpawnPos;

public:
	SceneGame();
	virtual ~SceneGame() override = default;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SpawnMonster(int count);

	template <typename T>
	void ClearObjectPool(ObjectPool<T>& pool);

	void OnDieMonster(Monster* monster); //좀비가 죽었을 때 씬에서 해야할 일
	const list<Monster*>* GetMonsterList() const;

};

template<typename T>
inline void SceneGame::ClearObjectPool(ObjectPool<T>& pool)
{
	for (auto obj : pool.GetUseList())
	{
		RemoveGo(obj);
	}
	pool.Clear();
}