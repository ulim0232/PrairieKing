#pragma once
#include "DataTable.h"
#include "Item.h"

struct ItemInfo
{
	Item::ItemTypes itemType;
	string name;
	string textureId;
};

class ItemTable :
    public DataTable
{
protected:
	unordered_map<Item::ItemTypes, ItemInfo> table;
public:
	ItemTable() : DataTable(DataTable::Ids::Item) {};
	virtual ~ItemTable() override = default;

	virtual bool Load() override;
	virtual void Release() override;

	const ItemInfo& Get(Item::ItemTypes id) const;
};

