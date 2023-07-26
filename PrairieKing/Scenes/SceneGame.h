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
class Item;
class Merchant;
class Stats;

class SceneGame : public Scene
{
protected:
	/*---sound---*/
	sf::Sound stage1Bgm;
	sf::Sound pDieSound;
	sf::Sound takeItemSound;
	sf::Sound getCoinSound;
	sf::Sound statsUpSound;
	sf::Sound mDieSound;


	/*--풀--*/
	ObjectPool<Monster> monsterPool;
	ObjectPool<Item> itemPool;

	vector<sf::Vector2f> monsterSpawnPosTop;
	vector<sf::Vector2f> monsterSpawnPosBottom;
	vector<sf::Vector2f> monsterSpawnPosLeft;
	vector<sf::Vector2f> monsterSpawnPosRight;

	/*---플레이어---*/
	CowBoy* cowBoy;

	/*--상인--*/
	Merchant* merchant;
	SpriteGo* merchantTable;

	Stats* boots;
	Stats* gun;
	Stats* shot;

	/*---map---*/
	TileMap* tileMap1 = nullptr;
	TileMap* tileMap2 = nullptr;
	TileMap* tileMap3 = nullptr;
	TileMap* tileMap4 = nullptr;
	TileMap* tileMap5 = nullptr;
	TileMap* tileMap6 = nullptr;
	TileMap* tileMap7 = nullptr;
	TileMap* tileMap8 = nullptr;
	TileMap* tileMap9 = nullptr;
	TileMap* tileMap10 = nullptr;
	TileMap* previusMap = nullptr;
	TileMap* currentMap = nullptr;
	//TileMap* nextStageMap = nullptr;
	int currentRound = 1;
	int currentStage = 1;

	/*---UI---*/
	SpriteGo* coinUI;
	SpriteGo* itemUI;
	SpriteGo* lifeUI;
	SpriteGo* shotUI;
	SpriteGo* timerUI;
	SpriteGo* keyUI; 
	SpriteGo* pickedItemUI;
	SpriteGo* arrow;
	SpriteGo* clearHeart;

	Item* pickedItem = nullptr;
	int lifeCount = 0;
	int coinCount = 0;

	TextGo* coinTxt;
	TextGo* lifeTxt;
	TextGo* bootsTxt;
	TextGo* gunTxt;
	TextGo* shotTxt;

	RectangleGo* timerGauge;
	sf::Vector2f timersize = { 482, 10 };
	float timerDecreaseRate;
	float timerDecreaseAmount;
	float currentTime = 0;
	float initualWidth = 482.f;
	float timeLimit = 5.f; //30
	float deadtime = 0.f;
	float cleartime = 0.f;

	bool isTimerRunning = true;
	bool isCowBoyDie = false;
	bool isClear = false;

	float mosterSpawnLimit = 3.f; //3초마다 몬스터 생성
	float reviveLimit = 3.f; //플레이어 사망 시 3초후 재생성

	float timerM = 0.0f; //몬스터 생성에 사용
	float timerR = 0.0f; //플레이어 부활에 사용

	bool roundClear = false; //라운드 클리어 설정
	bool isGameOver = false;
	bool haveItem = false;
	bool roundChange = false;
	bool isTimeOver = false;

	sf::Vector2f size;
	sf::Vector2f centerPos;

	int clearRound = 0; //2가 되면 스탯 업그레이드
	int bootsLevel = 0;
	int gunLevel = 0;
	int shotLevel = 0;
	bool isUpgrade = false;
	bool isArrive = false;
	bool isGameClear = false;

	//sf::Clock clock;
	//const sf::Time blinkTime = sf::seconds(0.2f);
	//bool blinkTimeCheck = false;
public:
	SceneGame();
	virtual ~SceneGame() override;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SpawnMonster(int count, int round);
	const list<Monster*>* GetMonsterList() const;
	void SetSpawnMonsterPos(TileMap* tilemap);

	void OnDieMonster(Monster* monster); //좀비가 죽었을 때 씬에서 해야할 일
	void OnDieCowBoy(); 
	void OnReviveCowBoy();
	//void BlinkCowboy();

	void TakeItem(Item* item);
	void GetCoin();
	void GetLife();
	void RemoveItem(Item* item);
	void UseNiza();
	void UseNuke();

	template <typename T>
	void ClearObjectPool(ObjectPool<T>& pool);
	bool GetRoundChange();
	void ChangeRound(int num);
	void ChangeMap();
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