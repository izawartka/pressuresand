#include "world.h"
#include "cellbf/cellbf_structs.h"

World* g_world = nullptr;

World::World()
{
	if (g_world != nullptr) throw std::logic_error("Only one instance of World can be present at a time");
	g_world = this;

	for (int x = 0; x < WORLD_WIDTH; x++) {
		for (int y = 0; y < WORLD_HEIGHT; y++) {
			m_cells[x][y] = new Cell(x, y, 0);
		}
	}
}

World::~World()
{
	for (int x = 0; x < WORLD_WIDTH; x++) {
		for (int y = 0; y < WORLD_HEIGHT; y++) {
			if (m_cells[x][y] != nullptr) delete m_cells[x][y];
		}
	}
}

Cell* World::getCells()
{
	return m_cells[0][0];
}

Cell* World::getCell(int x, int y) const
{
	if (checkOutsideWorld(x, y)) return nullptr;
	return m_cells[x][y];
}

bool World::isCellSolid(int x, int y, int baseDensity) const
{
	if (checkOutsideWorld(x, y)) return true;
	Cell* cell = getCell(x, y);
	if (cell == nullptr) return false;
	
	CELLBF_File* def = cell->getDefinition();
	return def->info.solid && (!def->info.gravity || def->info.density >= baseDensity);
}

Cell** World::getCellsRect(int x, int y, int width, int height) const
{
	Cell** cells = new Cell * [width * height];

	for (int cx = 0; cx < width; cx++) {
		for (int cy = 0; cy < height; cy++) {
			cells[cx + cy * width] = getCell(x + cx, y + cy);
		}
	}

	return cells;
}

bool World::swapCells(int x1, int y1, int x2, int y2)
{
	if (checkOutsideWorld(x1, y1)) return false;
	if (checkOutsideWorld(x2, y2)) return false;

	Cell* cell1 = getCell(x1, y1);
	Cell* cell2 = getCell(x2, y2);

	cell1->setPos(x2, y2);
	cell2->setPos(x1, y1);

	m_cells[x1][y1] = cell2;
	m_cells[x2][y2] = cell1;

	return true;
}

Cell* World::addCell(int x, int y, uint16_t type)
{
	Cell* oldCell = getCell(x, y);
	if (oldCell == nullptr) return nullptr;
	if (oldCell->getType() == type) return oldCell;

	removeCell(x, y);

	if(type == 0) return nullptr;

	Cell* cell = new Cell(x, y, type);
	m_cells[x][y] = cell;
	m_occupiedCells.push_back(cell);

	return cell;
}

void World::removeCell(int x, int y)
{
	if (checkOutsideWorld(x, y)) return;
	Cell* cell = m_cells[x][y];
	if (cell->getType() == 0) return;

	m_occupiedCells.remove(cell);
	delete cell;
	m_cells[x][y] = new Cell(x, y, 0);
}

std::list<Cell*>& World::getOccupiedCells()
{
	return m_occupiedCells;
}

void World::update()
{
	m_tick++;

	for (int x = 0; x < WORLD_WIDTH; x++) {
		for (int y = 0; y < WORLD_HEIGHT; y++) {
			m_cells[x][y]->update(m_tick);
		}
	}
}

bool World::checkOutsideWorld(int x, int y)
{
	return x < 0 || x >= WORLD_WIDTH ||
		y < 0 || y >= WORLD_HEIGHT;
}
