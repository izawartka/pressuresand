#pragma once
#include "common.h"
#include "cell.h"

class World {
public:
	World();
	~World();

	Cell* getCells();
	Cell* getCell(int x, int y) const;
	bool isCellSolid(int x, int y, int baseDensity = 0) const;
	Cell** getCellsRect(int x, int y, int width, int height) const;
	bool swapCells(int x1, int y1, int x2, int y2);
	Cell* addCell(int x, int y, uint16_t type);
	void removeCell(int x, int y);
	std::list<Cell*>& getOccupiedCells();
	void update();

	static bool checkOutsideWorld(int x, int y);
private:
	Cell* m_cells[WORLD_WIDTH][WORLD_HEIGHT] = { nullptr };
	std::list<Cell*> m_occupiedCells;
	long m_tick = 0;
};

extern World* g_world;