#include "world_brush.h"

WorldBrush::WorldBrush()
{
	m_brushSize = 3;
	m_cellType = 0;
}

WorldBrush::~WorldBrush()
{

}

void WorldBrush::drawPoint(int cx, int cy)
{
	if(m_world == nullptr) return;

	int radius = m_brushSize / 2;
	for (int x = -radius; x <= radius; x++)
	{
		for (int y = -radius; y <= radius; y++)
		{
			if (cy + y % 2 == 0) continue;
			if(x * x + y * y > radius * radius) continue;
			m_world->addCell(cx + x, cy + y, m_cellType);
		}
	}
}

void WorldBrush::drawLine(int x1, int y1, int x2, int y2)
{
	if(m_world == nullptr) return;

	int dx = abs(x2 - x1);
	int sx = x1 < x2 ? 1 : -1;
	int dy = -abs(y2 - y1);
	int sy = y1 < y2 ? 1 : -1;
	int err = dx + dy;

	while (true)
	{
		drawPoint(x1, y1);

		if (x1 == x2 && y1 == y2) break;

		int e2 = 2 * err;
		if (e2 >= dy)
		{
			err += dy;
			x1 += sx;
		}
		if (e2 <= dx)
		{
			err += dx;
			y1 += sy;
		}
	}
}
