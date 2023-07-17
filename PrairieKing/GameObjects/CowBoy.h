#pragma once
#include "GameObject.h"
#include "AnimationController.h"
#include "TileMap.h"

class SpriteGo;

class CowBoy : public GameObject
{
protected:
	AnimationController legAnimation;

	float gapLegToHead = 4.f;
	float speed = 100.f; //100
	int tileWidth = 0;

	sf::Vector2f look; //�ٶ󺸴� ����
	sf::Vector2f direction;
	sf::Vector2f velocity; //dir*dt

	sf::Texture* left = nullptr;
	sf::Texture* right = nullptr;
	sf::Texture* back = nullptr;
	sf::Texture* front = nullptr;
	sf::Texture* origintex = nullptr;

	TileMap* tileMap;
	sf::RectangleShape hitBox;
	sf::RectangleShape tileBox;
	sf::Vector2i boxSize = { 28.f, 28.f };
	sf::Vector2i tileSize = { 32.f, 32.f };
	sf::FloatRect intersection;
	sf::Vector2f newPosition;

	bool iscoll = false;
public:
	sf::Sprite head;
	sf::Sprite leg;

	CowBoy(const std::string& n = "");
	virtual ~CowBoy() override;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	virtual void SetPosition(const sf::Vector2f& p);
	virtual void SetPosition(float x, float y);

	virtual void SetOrigin(Origins origin);
	virtual void SetOrigin(float x, float y);

	void SetTileMap(TileMap* map, int width);
	bool IsTileCollision();
	bool IsCollisoinTile(int index);
};

