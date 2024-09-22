#pragma once
#include "common.h"
#include "cellbf/cellbf_structs.h"

class Cell {
public:
	Cell(int x, int y, uint16_t cellType);

	void setPos(int x, int y);

	uint16_t getType() const { return m_cellType; }
	CELLBF_File* getDefinition();
	int getX() const { return m_x; }
	int getY() const { return m_y; }

	void update(long tick);

private:
	bool updateSandGravity(uint16_t density);
	void updateFluid(uint16_t density);
	bool updateMixing(CELLBF_MixDef* mixDef);

	int m_cellType;
	int m_x, m_y;
	long m_tick = 0;
};