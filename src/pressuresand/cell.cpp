#include "cell.h"
#include "world.h"
#include "object_scripts/cell_types_loader.h"

Cell::Cell(int x, int y, uint16_t cellType)
{
	m_x = x;
	m_y = y;
	m_cellType = cellType;
}

void Cell::setPos(int x, int y)
{
	m_x = x;
	m_y = y;
}

CELLBF_File* Cell::getDefinition()
{
	return g_cellTypesLoader->getCellTypeDef(m_cellType);
}

void Cell::update(long tick)
{
	if (m_tick >= tick) return;
	m_tick = tick;

	CELLBF_File* def = getDefinition();

	if (def->info.gravity) {
		bool falled = updateSandGravity(def->info.density);
		if (def->info.fluid && !falled) updateFluid(def->info.density);
	}

	uint16_t mixingsCount = def->mixing.mixingsCount;
	for (uint16_t i = 0; i < mixingsCount; i++) {
		updateMixing(&def->mixing.mixings[i]);
	}
}

bool Cell::updateSandGravity(uint16_t density)
{
	bool centerSolid = g_world->isCellSolid(m_x, m_y + 1, density);
	if (!centerSolid) {
		g_world->swapCells(m_x, m_y, m_x, m_y + 1);
		return true;
	}

	bool leftSolid = g_world->isCellSolid(m_x - 1, m_y + 1, density);
	bool rightSolid = g_world->isCellSolid(m_x + 1, m_y + 1, density);

	if (!leftSolid && !rightSolid) {
		if (rand() % 2 == 0) leftSolid = true;
		else rightSolid = true;
	}
	
	if (!leftSolid) {
		g_world->swapCells(m_x, m_y, m_x - 1, m_y + 1);
		return true;
	}
	
	if (!rightSolid) {
		g_world->swapCells(m_x, m_y, m_x + 1, m_y + 1);
		return true;
	}

	return false;
}

void Cell::updateFluid(uint16_t density)
{
	bool leftSolid = g_world->isCellSolid(m_x - 1, m_y, density);
	bool rightSolid = g_world->isCellSolid(m_x + 1, m_y, density);

	if (!leftSolid && !rightSolid) {
		if (rand() % 2 == 0) leftSolid = true;
		else rightSolid = true;
	}

	if (!leftSolid) {
		g_world->swapCells(m_x, m_y, m_x - 1, m_y);
		return;
	}

	if (!rightSolid) {
		g_world->swapCells(m_x, m_y, m_x + 1, m_y);
		return;
	}
}

bool Cell::updateMixing(CELLBF_MixDef* mixDef)
{
	if (rand() % 10000 >= mixDef->chance) return false;

	Cell* neighbours[4];
	neighbours[0] = g_world->getCell(m_x - 1, m_y);
	neighbours[1] = g_world->getCell(m_x + 1, m_y);
	neighbours[2] = g_world->getCell(m_x, m_y - 1);
	neighbours[3] = g_world->getCell(m_x, m_y + 1);

	for (int i = 0; i < 4; i++) {
		if (neighbours[i] == nullptr) continue;
		if (neighbours[i]->getType() != mixDef->triggerId) continue;

		int x = mixDef->reverse ? neighbours[i]->getX() : m_x;
		int y = mixDef->reverse ? neighbours[i]->getY() : m_y;

		g_world->addCell(x, y, mixDef->resultId);
		return true;
	}

	return false;
}
