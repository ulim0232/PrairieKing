#pragma once
#include "DataTable.h"
#include "Monster.h"

struct MonsterInfo
{
	Monster::Types monsterType;
	string name;
	string idleClipPath;
	string dieClipPath;
	float speed;
	int maxHp;

};
class MonsterTable : public DataTable
{
protected:
	unordered_map<Monster::Types, MonsterInfo> table;

public:
	MonsterTable() : DataTable(DataTable::Ids::Monster) {};
	virtual ~MonsterTable() override = default;

	virtual bool Load() override;
	virtual void Release() override;

	const MonsterInfo& Get(Monster::Types id) const;


};

