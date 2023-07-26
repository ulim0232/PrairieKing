#include "stdafx.h"
#include "Stats.h"
#include "DataTableMgr.h"
#include "StatsTable.h"
#include "ResourceMgr.h"

void Stats::Init()
{
	SpriteGo::Init();
	SetOrigin(Origins::MC);
	sprite.setScale({ 2.f, 2.f });
}

void Stats::Reset()
{
	level = 0;
	tex = RESOURCE_MGR.GetTexture(textureId);
}

void Stats::Update(float dt)
{
	SpriteGo::Update(dt);
}

void Stats::SetType(StatsTypes t)
{
	type = t;
	const StatsInfo& info = DATATABLE_MGR.Get<StatsTable>(DataTable::Ids::Stats)->Get(t);

	name = info.name;
	costs[0] = info.price_1;
	costs[1] = info.price_2;
	costs[2] = info.price_3;

	textures[0] = info.textureId_1;
	textures[1] = info.textureId_2;
	textures[2] = info.textureId_3;
}

void Stats::SetLevel(int lev)
{
	level = lev;

	tex = RESOURCE_MGR.GetTexture(textures[lev]);
	sprite.setTexture(*tex);
	currentCost = costs[lev];
}

int Stats::getCost()
{
	return costs[level];
}
