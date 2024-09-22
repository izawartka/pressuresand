#include "world_view.h"

WorldView::WorldView(World* world)
{
	mp_world = world;
}

WorldView::~WorldView()
{

}

void WorldView::init()
{
	m_isDrawing = false;
	setWorld(mp_world);

	RZUF3_ClickableOptions options = {
		{0, 0, WORLD_WIDTH, WORLD_HEIGHT},
		false
	};
	m_clickable = new RZUF3_Clickable(options);
	m_object->addScript(m_clickable);

	RZUF3_EventsManager* objEventsManager = m_object->getEventsManager();
	_ADD_LISTENER(objEventsManager, MouseDown);
	_ADD_LISTENER(objEventsManager, MouseUp);
	_ADD_LISTENER(objEventsManager, MouseMove);

	RZUF3_EventsManager* eventsManager = g_scene->getEventsManager();
	_ADD_LISTENER(eventsManager, Draw);
	_ADD_LISTENER(eventsManager, Update);
}

void WorldView::deinit()
{
	RZUF3_EventsManager* objEventsManager = m_object->getEventsManager();
	_REMOVE_LISTENER(objEventsManager, MouseDown);
	_REMOVE_LISTENER(objEventsManager, MouseUp);
	_REMOVE_LISTENER(objEventsManager, MouseMove);

	RZUF3_EventsManager* eventsManager = g_scene->getEventsManager();
	_REMOVE_LISTENER(eventsManager, Draw);
	_REMOVE_LISTENER(eventsManager, Update);

	m_object->removeScript(m_clickable);
	delete m_clickable;
	m_clickable = nullptr;

	setWorld(nullptr);
}

void WorldView::setWorld(World* world)
{
	m_world = world;
	m_brush.setWorld(world);
}

void WorldView::onMouseDown(RZUF3_MouseDownEvent* event)
{
	if (event->getButton() != SDL_BUTTON_LEFT) return;
	m_isDrawing = true;

	m_lastX = event->getX();
	m_lastY = event->getY();

	m_brush.drawPoint(m_lastX, m_lastY);
}

void WorldView::onMouseUp(RZUF3_MouseUpEvent* event)
{
	if (event->getButton() != SDL_BUTTON_LEFT) return;

	m_isDrawing = false;
}

void WorldView::onMouseMove(RZUF3_MouseMoveEvent* event)
{
	if(!m_isDrawing) return;

	m_brush.drawLine(
		m_lastX, m_lastY, 
		event->getX(), event->getY()
	);

	m_lastX = event->getX();
	m_lastY = event->getY();
}

void WorldView::onDraw(RZUF3_DrawEvent* event)
{
	if (!m_world) return;

	g_renderer->setAlign(RZUF3_Align_TopLeft);
	for each (Cell* cell in m_world->getOccupiedCells())
	{
		SDL_Color color = cell->getDefinition()->info.color;
		if(color.a == 0) continue;

		g_renderer->setColor(color);
		SDL_Rect rect = {
			cell->getX(),
			cell->getY(),
			1,
			1
		};

		g_renderer->fillRect(m_object, rect);
	}
}

void WorldView::onUpdate(RZUF3_UpdateEvent* event)
{
	if (!m_world) return;

	m_world->update();

	if (m_isDrawing)
	{
		m_brush.drawPoint(m_lastX, m_lastY);
	}
}
