#pragma once
#include "SpriteGo.h"
class Stats : public SpriteGo
{
public:
	enum class StatsTypes
	{
		Boots,
		Gun,
		Shot,
	};
protected:

	int level = 0;
	int costs[3];
	int currentCost = 0;

	string name;
	string textures[3];
	StatsTypes type;
	sf::Texture* tex = nullptr;

public:
	Stats(const string& textureId = "", const string& n = "")
		:SpriteGo(textureId, n) {}
	virtual ~Stats() override { Release(); }

	virtual void Init() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;

	void SetType(StatsTypes t);
	void SetLevel(int lev);
	int getCost();
};

