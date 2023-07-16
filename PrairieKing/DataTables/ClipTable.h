#pragma once
#include "DataTable.h"

struct ClipInfo
{
	string id;
	int fps;
	AnimationLoopTypes loopType;
	int Xsize;
	int Ysize;
	int frameCount;
	int column;
	int row;
};
class ClipTable : public DataTable
{
protected:
	std::vector<std::unordered_map<string, ClipInfo>> tables;
	//unordered_map<string, ClipInfo> table;

public:
	ClipTable() : DataTable(DataTable::Ids::Clip) {};
	virtual ~ClipTable() override = default;

	virtual bool Load() override;
	virtual void Release() override;

	const ClipInfo& Get(const string& id, MotionId mId) const;

};

