#include "stdafx.h"
#include "rapidcsv.h"
#include "MonsterTable.h"

bool MonsterTable::Load()
{
	string filename = "tables/monster/MonsterTable.csv";

	rapidcsv::Document doc(filename);

	auto keys = doc.GetRow<string>(0);

	auto names = doc.GetColumn<string>(0);
	auto ids = doc.GetColumn<int>(1);
	auto idle_clips = doc.GetColumn<string>(2);
	auto die_clips = doc.GetColumn<string>(3);
	auto speeds = doc.GetColumn<float>(4);
	auto maxhps = doc.GetColumn<int>(5);

	for (int i = 0; i < ids.size(); i++)
	{
		MonsterInfo minfo
		{
			(Monster::Types)ids[i], names[i], idle_clips[i], die_clips[i], speeds[i], maxhps[i]
		};
		table.insert({ (Monster::Types)ids[i] , minfo });
	}

	return true;
}

void MonsterTable::Release()
{
	table.clear();
}

const MonsterInfo& MonsterTable::Get(Monster::Types id) const
{
	auto find = table.find(id);
	if (find == table.end()) //존재하지 않을 때
	{
		cout << "ERR: Failed to Data Table Load" << endl;
		exit(-1);
	}
	return find->second;
}
