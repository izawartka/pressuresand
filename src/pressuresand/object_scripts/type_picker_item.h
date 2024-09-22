#pragma once
#include "object_scripts.h"
#include "../world.h"

class TypePicker;

class TypePickerItem : public RZUF3_ObjectScript {
public:
	TypePickerItem(TypePicker* typePicker, int id, std::string name);
	~TypePickerItem();

	void init();
	void deinit();

	void setType(int id, std::string name);

private:
	void removeText();
	void addText();
	void removeClickable();
	void addClickable();

	void onMouseDown(RZUF3_MouseDownEvent* event);
	void onDraw(RZUF3_DrawEvent* event);

	RZUF3_TextRenderer* m_textRenderer = nullptr;
	RZUF3_Clickable* m_clickable = nullptr;
	TypePicker* mp_typePicker;
	int mp_id;
	std::string mp_name;

	int m_id = 0;
	std::string m_name = "";

	_DECLARE_LISTENER(MouseDown)
	_DECLARE_LISTENER(Draw)
};