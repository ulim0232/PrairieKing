#pragma once
#include "Scene.h"

class TextGo;
class StringTable;
class SceneOver : public Scene
{
protected:
	sf::SoundBuffer titleBgmBuffer;
	sf::Sound titleBgm;

	sf::Vector2f centerPos;

	TextGo* titleM;
	TextGo* restartM;
	TextGo* exitM;
	TextGo* cursorM;

	StringTable* stringTable = nullptr;

	enum class Mode
	{
		Restart,
		Exit
	};

	Mode mode;

public:
	SceneOver();
	virtual ~SceneOver() override = default;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
};

