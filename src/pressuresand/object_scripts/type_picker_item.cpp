#include "type_picker_item.h"

TypePickerItem::TypePickerItem(TypePicker* typePicker, int id, std::string name)
{
	mp_typePicker = typePicker;
	mp_id = id;
	mp_name = name;
}

TypePickerItem::~TypePickerItem()
{

}

void TypePickerItem::init()
{
	addClickable();
	setType(mp_id, mp_name);

	RZUF3_EventsManager* objEventsManager = m_object->getEventsManager();
	_ADD_LISTENER(objEventsManager, MouseDown);

	RZUF3_EventsManager* eventsManager = g_scene->getEventsManager();
	_ADD_LISTENER(eventsManager, Draw);
}

void TypePickerItem::deinit()
{
	RZUF3_EventsManager* objEventsManager = m_object->getEventsManager();
	_REMOVE_LISTENER(objEventsManager, MouseDown);

	RZUF3_EventsManager* eventsManager = g_scene->getEventsManager();
	_REMOVE_LISTENER(eventsManager, Draw);

	removeText();
	removeClickable();
}

void TypePickerItem::setType(int id, std::string name)
{
	if (m_textRenderer != nullptr) removeText();

	m_id = id;
	m_name = name;

	addText();
}

void TypePickerItem::removeText()
{
	if (m_textRenderer == nullptr) return;

	m_object->removeScript(m_textRenderer);
	delete m_textRenderer;
	m_textRenderer = nullptr;
}

void TypePickerItem::addText()
{
	if (m_textRenderer != nullptr) return;

	RZUF3_TextRendererOptions textOptions;
	textOptions.style.fontFilepath = "assets/fonts/roboto-regular.ttf";
	textOptions.alignment = RZUF3_Align_TopLeft;
	textOptions.dstRect.x = PICKER_PADDING;
	textOptions.dstRect.y = PICKER_PADDING;
	textOptions.style.size = 16;
	textOptions.text = "cell_type_" + m_name;

	m_textRenderer = new RZUF3_TextRenderer(textOptions);
	m_object->addScript(m_textRenderer);
}

void TypePickerItem::removeClickable()
{
	if (m_clickable == nullptr) return;

	m_object->removeScript(m_clickable);
	delete m_clickable;
	m_clickable = nullptr;
}

void TypePickerItem::addClickable()
{
	if (m_clickable != nullptr) return;

	RZUF3_ClickableOptions clickableOptions = {
		{0, 1, PICKER_FULL_WIDTH, PICKER_FULL_HEIGHT-2}
	};

	m_clickable = new RZUF3_Clickable(clickableOptions);
	m_object->addScript(m_clickable);
}

void TypePickerItem::onMouseDown(RZUF3_MouseDownEvent* event)
{
	if (event->getButton() != SDL_BUTTON_LEFT) return;

	mp_typePicker->getWorldView()->getBrush()->setCellType(m_id);
}

void TypePickerItem::onDraw(RZUF3_DrawEvent* event)
{
	bool selected = mp_typePicker->getWorldView()->getBrush()->getCellType() == m_id;
	if (!selected) return;

	SDL_Rect rect = {
		0,
		0,
		PICKER_FULL_WIDTH,
		PICKER_FULL_HEIGHT
	};

	g_renderer->setColor(PICKER_SELECTED_COLOR);
	g_renderer->setAlign(RZUF3_Align_TopLeft);
	g_renderer->fillRect(m_object, rect);
}
