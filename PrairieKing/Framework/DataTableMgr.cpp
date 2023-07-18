#include "stdafx.h"
#include "DataTableMgr.h"
#include "StringTable.h"
#include "ClipTable.h"
#include "MonsterTable.h"

void DataTableMgr::LoadAll()
{
	tables.insert({ DataTable::Ids::String, new StringTable() });
	tables.insert({ DataTable::Ids::Clip, new ClipTable() });
	tables.insert({ DataTable::Ids::Monster, new MonsterTable() });

	for (auto pair : tables)
	{
		if (!pair.second->Load())
		{
			std::cout << "ERR: DATA TABLE LOAD FAIL" << std::endl;
			exit(-1);
		}
	}
}

void DataTableMgr::ReleaseAll()
{
	for (auto pair : tables)
	{
		//pair.second->Release();
		delete pair.second;
	}
	tables.clear();
}
