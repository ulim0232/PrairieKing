#include "stdafx.h"
#include "StatsTable.h"
#include "rapidcsv.h"

bool StatsTable::Load()
{
	string filename = "tables/statsTable.csv";

	rapidcsv::Document doc(filename);

	auto keys = doc.GetRow<string>(0);

	auto ids = doc.GetColumn<int>(0);
	auto names = doc.GetColumn<string>(1);
	auto textureIds_1 = doc.GetColumn<string>(2);
	auto textureIds_2 = doc.GetColumn<string>(3);
	auto textureIds_3 = doc.GetColumn<string>(4);
	auto prices_1 = doc.GetColumn<int>(5);
	auto prices_2 = doc.GetColumn<int>(6);
	auto prices_3 = doc.GetColumn<int>(7);

	for (int i = 0; i < ids.size(); i++)
	{
		StatsInfo sInfo
		{
			(Stats::StatsTypes)ids[i], names[i], textureIds_1[i], textureIds_2[i], textureIds_3[i],
			prices_1[i],prices_2[i],prices_3[i]
		};
		table.insert({ (Stats::StatsTypes)ids[i] , sInfo });
	}

	return true;
}

void StatsTable::Release()
{
	table.clear();
}

const StatsInfo& StatsTable::Get(Stats::StatsTypes id) const
{
	auto find = table.find(id);
	if (find == table.end()) //존재하지 않을 때
	{
		cout << "ERR: Failed to Data Table Load" << endl;
		exit(-1);
	}
	return find->second;
}
