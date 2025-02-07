#include "brush_size_slider.h"

BrushSizeSlider::BrushSizeSlider(WorldBrush* brush)
{
	mp_brush = brush;
}

BrushSizeSlider::~BrushSizeSlider()
{
}

void BrushSizeSlider::init()
{
	RZUF3_EventsManager* objEventsManager = m_object->getEventsManager();
	_ADD_LISTENER(objEventsManager, UIValueChange);
}

void BrushSizeSlider::deinit()
{
	RZUF3_EventsManager* objEventsManager = m_object->getEventsManager();
	_REMOVE_LISTENER(objEventsManager, UIValueChange);
}

void BrushSizeSlider::onUIValueChange(RZUF3_UIValueChangeEvent* event)
{
	if (event->getTypeIndex() != typeid(int)) return;
	if (event->getValue() == nullptr) return;

	int value = *(int*)event->getValue();
	int brushSize = value * 2 + 1;
	mp_brush->setBrushSize(brushSize);
}
