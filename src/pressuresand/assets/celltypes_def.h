#pragma once
#include "../common.h"
#include "../cellbf/cellbf_structs.h"

class CellTypesDef : public RZUF3_Asset {
public:
	static CellTypesDef* getInstance(std::string dirpath);

	bool load();
	void unload();
	void* getContent();
	int getContentSize();

private:
	CELLBF_File* loadFile(std::string filepath);
	void assignIDs();
	uint16_t getID(std::string name, bool forAssigning = false);

	std::vector<CELLBF_File*> m_cellFiles;
	std::unordered_map<std::string, uint16_t> m_cellIDs;
	uint16_t m_nextID = 0;
};