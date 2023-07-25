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

class SceneGame : public Scene
{
protected:
	/*---sound---*/
	sf::Sound stage1Bgm;
	sf::Sound pDieSound;

	/*--Ǯ--*/
	ObjectPool<Monster> monsterPool;
	ObjectPool<Item> itemPool;

	vector<sf::Vector2f> monsterSpawnPosTop;
	vector<sf::Vector2f> monsterSpawnPosBottom;
	vector<sf::Vector2f> monsterSpawnPosLeft;
	vector<sf::Vector2f> monsterSpawnPosRight;

	/*---�÷��̾�---*/
	CowBoy* cowBoy;

	/*---map---*/
	TileMap* tileMap1 = nullptr;
	TileMap* tileMap2 = nullptr;
	TileMap* tileMap3 = nullptr;
	TileMap* tileMap4 = nullptr;
	TileMap* tileMap5 = nullptr;
	TileMap* currentMap = nullptr;
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

	Item* pickedItem = nullptr;
	int lifeCount = 3;
	int coinCount = 0;

	TextGo* coinTxt;
	TextGo* lifeTxt;

	RectangleGo* timerGauge;
	sf::Vector2f timersize = { 482, 10 };
	float timerDecreaseRate;
	float timerDecreaseAmount;
	float currentTime = 0;
	float initualWidth = 482.f;
	float timeLimit = 5.f; //30

	bool isTimerRunning = true;
	bool isCowBoyDie = false;

	float mosterSpawnLimit = 3.f; //3�ʸ��� ���� ����
	float reviveLimit = 3.f; //�÷��̾� ��� �� 3���� �����

	float timerM = 0.0f; //���� ������ ���
	float timerR = 0.0f; //�÷��̾� ��Ȱ�� ���

	bool roundClear = false; //���� Ŭ���� ����
	bool isGameOver = false;
	bool haveItem = false;
	bool roundChange = false;

	sf::Vector2f size;
	sf::Vector2f centerPos;

	int clearRound = 0;

	//sf::Clock clock;
	//const sf::Time blinkTime = sf::seconds(0.2f);
	//bool blinkTimeCheck = false;
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
	const list<Monster*>* GetMonsterList() const;
	void SetSpawnMonsterPos(TileMap* tilemap);

	void OnDieMonster(Monster* monster); //���� �׾��� �� ������ �ؾ��� ��
	void OnDieCowBoy(); 
	void OnReviveCowBoy();
	//void BlinkCowboy();

	void TakeItem(Item* item);
	void GetGoin();
	void GetLife();
	void RemoveItem(Item* item);
	void UseNiza();
	void UseNuke();

	template <typename T>
	void ClearObjectPool(ObjectPool<T>& pool);
	bool GetRoundChange();
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