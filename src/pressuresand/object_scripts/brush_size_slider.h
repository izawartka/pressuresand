#pragma once
#include "object_scripts.h"

#include "../world_brush.h"

class BrushSizeSlider : public RZUF3_ObjectScript {
public:
	BrushSizeSlider(WorldBrush* brush);
	~BrushSizeSlider();

	void init();
	void deinit();

private:
	void onUIValueChange(RZUF3_UIValueChangeEvent* event);

	WorldBrush* mp_brush;

	_DECLARE_LISTENER(UIValueChange);
};