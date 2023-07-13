#pragma once
#include "Scene.h"

class VertexArrayGo;
class Player;
class Player2;
class TileMap;

class SceneGame : public Scene
{
protected:
	sf::SoundBuffer stage1BgmBuffer;
	sf::Sound stage1Bgm;
	Player* player;

	TileMap* tileMap = nullptr;
public:
	SceneGame();
	virtual ~SceneGame() override = default;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
};

