#pragma once
#include "DataTable.h"
#include "Stats.h"

struct StatsInfo
{
	Stats::StatsTypes statsType;
	string name;
	string textureId_1;
	string textureId_2;
	string textureId_3;
	int price_1;
	int price_2;
	int price_3;
};

class StatsTable : public DataTable
{
protected:
	unordered_map<Stats::StatsTypes, StatsInfo> table;
public:
	StatsTable() : DataTable(DataTable::Ids::Item) {};
	virtual ~StatsTable() override = default;

	virtual bool Load() override;
	virtual void Release() override;

	const StatsInfo& Get(Stats::StatsTypes id) const;
};

