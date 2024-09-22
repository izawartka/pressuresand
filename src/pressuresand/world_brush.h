#pragma once
#include "common.h"
#include "world.h"

class WorldBrush {
public:
	WorldBrush();
	~WorldBrush();

	void setCellType(int cellType) { m_cellType = cellType; }
	void setBrushSize(int size) { m_brushSize = size; }
	void setWorld(World* world) { m_world = world; }

	int getCellType() const { return m_cellType; }
	int getBrushSize() const { return m_brushSize; }
	World* getWorld() const { return m_world; }

	void drawPoint(int x, int y);
	void drawLine(int x1, int y1, int x2, int y2);
private:

	World* m_world;
	int m_cellType;
	int m_brushSize;
};