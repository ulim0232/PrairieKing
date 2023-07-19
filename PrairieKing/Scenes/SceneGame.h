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
	float timeLimit = 30.0f;

	bool isTimerRunning = true;
	vector<sf::Vector2f> monsterSpawnPosTop;
	vector<sf::Vector2f> monsterSpawnPosBottom;
	vector<sf::Vector2f> monsterSpawnPosLeft;
	vector<sf::Vector2f> monsterSpawnPosRight;

	bool isGameOver = false;
	sf::Vector2f timersize = { 482, 10 };

	float mosterSpawnLimit = 3.f;
	float timerM = 0.0f;

	float reviveLimit = 3.f;
	float timerR = 0.0f;
	sf::Clock clock;
	const sf::Time blinkTime = sf::seconds(0.2f);
	bool blinkTimeCheck = false;

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
	void OnDieCowBoy();
	void OnReviveCowBoy();
	const list<Monster*>* GetMonsterList() const;

	void BlinkCowboy();
	

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