#pragma once
#include "object_scripts.h"
#include "../world.h"
#include "../events/celltypes_load.h"

class WorldView;
class TypePickerItem;

class TypePicker : public RZUF3_ObjectScript {
public:
	TypePicker(WorldView* worldView);
	~TypePicker();

	void init();
	void deinit();

	WorldView* getWorldView() { return mp_worldView; }

private:
	void onCellTypesLoad(USER_CellTypesLoadEvent* event);

	void removeItems();
	void createItems();

	WorldView* mp_worldView;

	std::vector<TypePickerItem*> m_items;

	_DECLARE_LISTENER(CellTypesLoad)
};