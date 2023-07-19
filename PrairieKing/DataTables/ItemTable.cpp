#include "stdafx.h"
#include "ItemTable.h"
#include "rapidcsv.h"

bool ItemTable::Load()
{
	string filename = "tables/ItemTable.csv";

	rapidcsv::Document doc(filename);

	auto keys = doc.GetRow<string>(0);
	
	auto ids = doc.GetColumn<int>(0);
	auto names = doc.GetColumn<string>(1);
	auto textureIds = doc.GetColumn<string>(2);

	for (int i = 0; i < ids.size(); i++)
	{
		ItemInfo iInfo
		{
			(Item::ItemTypes)ids[i], names[i], textureIds[i]
		};
		table.insert({ (Item::ItemTypes)ids[i] , iInfo });
	}

	return true;
}

void ItemTable::Release()
{
	table.clear();
}

const ItemInfo& ItemTable::Get(Item::ItemTypes id) const
{
	auto find = table.find(id);
	if (find == table.end()) //존재하지 않을 때
	{
		cout << "ERR: Failed to Data Table Load" << endl;
		exit(-1);
	}
	return find->second;
}
