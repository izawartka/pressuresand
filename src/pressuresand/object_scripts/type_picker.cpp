#include "type_picker.h"

TypePicker::TypePicker(WorldView* worldView)
{
	mp_worldView = worldView;
}

TypePicker::~TypePicker()
{
}

void TypePicker::init()
{
	RZUF3_EventsManager* eventsManager = g_scene->getEventsManager();
	_ADD_LISTENER_CL(eventsManager, CellTypesLoad, USER);

	createItems();
}

void TypePicker::deinit()
{
	RZUF3_EventsManager* eventsManager = g_scene->getEventsManager();
	_REMOVE_LISTENER_CL(eventsManager, CellTypesLoad, USER);

	removeItems();
}

void TypePicker::onCellTypesLoad(USER_CellTypesLoadEvent* event)
{
	removeItems();
	createItems();
}

void TypePicker::removeItems()
{
	for (TypePickerItem*& item : m_items)
	{
		g_scene->removeObject(item->getObject()->getName());
		delete item;
	}

	m_items.clear();
}

void TypePicker::createItems()
{
	if(g_cellTypesLoader == nullptr || g_cellTypesLoader->isLoaded() == false) return;

	int cellTypeCount = g_cellTypesLoader->getCellTypesCount();

	int index = 0;
	for (int i = 0; i < cellTypeCount; i++)
	{
		CELLBF_File* cellTypeDef = g_cellTypesLoader->getCellTypeDef(i);
		if(cellTypeDef->info.display == false) continue;

		std::string objName = "cell_type_" + std::to_string(i);
		TypePickerItem* item = new TypePickerItem(this, i, cellTypeDef->info.name);
		RZUF3_ObjectDefinition objDef;
		objDef.name = objName;
		objDef.parentName = m_object->getName();
		objDef.pos = RZUF3_Pos(0, index * PICKER_FULL_HEIGHT);
		objDef.scripts = { item };
		RZUF3_Object* object = g_scene->addObject(objDef);
		if (object) object->init();

		m_items.push_back(item);
		index++;
	}
}
