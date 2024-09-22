#pragma once
#include "object_scripts.h"
#include "../world.h"
#include "../world_brush.h"

class WorldView : public RZUF3_ObjectScript {
public:
	WorldView(World* world);
	~WorldView();

	void init();
	void deinit();

	void setWorld(World* world);
	WorldBrush* getBrush() { return &m_brush; }

private:
	void onMouseDown(RZUF3_MouseDownEvent* event);
	void onMouseUp(RZUF3_MouseUpEvent* event);
	void onMouseMove(RZUF3_MouseMoveEvent* event);
	void onDraw(RZUF3_DrawEvent* event);
	void onUpdate(RZUF3_UpdateEvent* event);

	World* mp_world;

	RZUF3_Clickable* m_clickable = nullptr;
	World* m_world = nullptr;
	bool m_isDrawing = false;
	int m_lastX = 0;
	int m_lastY = 0;
	WorldBrush m_brush;

	_DECLARE_LISTENER(MouseDown)
	_DECLARE_LISTENER(MouseMove)
	_DECLARE_LISTENER(MouseUp)
	_DECLARE_LISTENER(Draw)
	_DECLARE_LISTENER(Update)
};