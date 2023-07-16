#include "stdafx.h"
#include "ClipTable.h"
#include "rapidcsv.h"

bool ClipTable::Load()
{
	vector<string> filenames; //data table의 경로 저장
	filenames.push_back("tables/IdleClipTable.csv");
	filenames.push_back("tables/MoveClipTable.csv");

	for (int i = 0; i < tables.size(); i++)
	{
		rapidcsv::Document doc(filenames[i]);
		auto keys = doc.GetRow<string>(0);
		auto ids = doc.GetColumn<string>(0);
		auto fpss = doc.GetColumn<int>(1);
		auto loopTypes = doc.GetColumn<int>(2);
		auto Xsizes = doc.GetColumn<int>(3);
		auto Ysizes = doc.GetColumn<int>(4);
		auto frameCounts = doc.GetColumn<int>(5);
		auto columns = doc.GetColumn<float>(6);
		auto rows = doc.GetColumn<int>(7);

		for (int j = 0; j < fpss.size(); j++)
		{
			ClipInfo cInfo
			{
				ids[j], fpss[j], (AnimationLoopTypes)loopTypes[j], Xsizes[j], Ysizes[j], frameCounts[j], columns[j], rows[j]
			};
			tables[i].insert({ ids[j] , cInfo });
		}
	}
	return true;
	/*string filename = "tables/ClipTable.csv";
	rapidcsv::Document doc(filename);

	auto keys = doc.GetRow<string>(0);
	auto ids = doc.GetColumn<string>(0);
	auto fpss = doc.GetColumn<int>(1);
	auto loopTypes = doc.GetColumn<int>(2);
	auto Xsizes = doc.GetColumn<int>(3);
	auto Ysizes = doc.GetColumn<int>(4);
	auto frameCounts = doc.GetColumn<int>(5);
	auto columns = doc.GetColumn<float>(6);
	auto rows = doc.GetColumn<int>(7);

	for (int i = 0; i < ids.size(); i++)
	{
		ClipInfo cInfo
		{
			ids[i], fpss[i], (AnimationLoopTypes)loopTypes[i], Xsizes[i], Ysizes[i], frameCounts[i], columns[i], rows[i]
		};
		table.insert({ ids[i] , cInfo });
	}
	return true;*/
}

void ClipTable::Release()
{
	for (auto table : tables)
	{
		table.clear();
	}
}

const ClipInfo& ClipTable::Get(const string& id, MotionId mId) const
{
	auto& table = tables[(int)mId];
	auto find = table.find(id);

	if (find == table.end()) //존재하지 않을 때
	{
		cout << "ERR: Failed to Data Table Load" << endl;
		exit(-1);
	}
	return find->second;
}
