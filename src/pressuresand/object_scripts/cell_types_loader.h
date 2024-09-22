#pragma once
#include "object_scripts.h"
#include "../cellbf/cellbf_structs.h"

class CellTypesLoader : public RZUF3_ObjectScript {
public:
	CellTypesLoader(std::string dirpath);
	~CellTypesLoader();

	void init();
	void deinit();

	CELLBF_File* getCellTypeDef(int id) { return m_cellFiles[id]; }
	int getCellTypesCount() const { return m_cellFilesCount; }
	bool isLoaded() const { return m_loaded; }

private:
	void removeTypes();
	void createTypes();

	std::string mp_dirpath;

	bool m_loaded = false;
	CELLBF_File** m_cellFiles = nullptr;
	int m_cellFilesCount = 0;
};

extern CellTypesLoader* g_cellTypesLoader;