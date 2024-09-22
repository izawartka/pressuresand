#include "celltypes_def.h"
#include "../cellbf/cellbf_reader.h"
#include "../cellbf/cellbf_destroyer.h"
#include "celltypes_def.h"

CellTypesDef* CellTypesDef::getInstance(std::string dirpath)
{
	CellTypesDef* instance = new CellTypesDef();
	instance->m_filepath = dirpath;
	return instance;
}

bool CellTypesDef::load()
{
	if (!std::filesystem::exists(m_filepath))
	{
		spdlog::error("CELL directory not found: {}", m_filepath);
		return false;
	}

	for (const auto& entry : std::filesystem::directory_iterator(m_filepath))
	{
		std::string filepath = entry.path().string();

		CELLBF_File* cellFile = loadFile(filepath);
		if (cellFile != nullptr)
		{
			m_cellFiles.push_back(cellFile);
		}
	}

	if (m_cellFiles.empty())
	{
		spdlog::error("No CELL files found in: {}", m_filepath);
		return false;
	}

	assignIDs();

	spdlog::info("Loaded CELL files from: {}", m_filepath);

	return true;
}

void CellTypesDef::unload()
{
	for (CELLBF_File*& cellFile : m_cellFiles)
	{
		CELLBF_Destroyer::destroy(*cellFile);
		delete cellFile;
	}

	m_cellFiles.clear();

	spdlog::info("Unloaded CELL files from: {}", m_filepath);
}

void* CellTypesDef::getContent()
{
	return m_cellFiles.data();
}

int CellTypesDef::getContentSize()
{
	return m_cellFiles.size();
}

CELLBF_File* CellTypesDef::loadFile(std::string filepath)
{
	std::ifstream file(filepath, std::ios::binary);

	if (!file.is_open())
	{
		spdlog::error("Failed to open CELL file: {}", filepath);
		return nullptr;
	}

	CELLBF_File* cellFile = new CELLBF_File();

	if (!CELLBF_Reader::readFile(file, *cellFile))
	{
		return nullptr;
	}

	spdlog::info("Loaded CELL file: {}", filepath);

	return cellFile;
}

void CellTypesDef::assignIDs()
{
	for (CELLBF_File*& cellFile : m_cellFiles)
	{
		int id = getID(cellFile->info.name, true);
		cellFile->info.id = id;
		if (id == -1) {
			m_cellFiles.erase(std::remove(m_cellFiles.begin(), m_cellFiles.end(), cellFile), m_cellFiles.end());
			CELLBF_Destroyer::destroy(*cellFile);
		}
	}

	for (CELLBF_File*& cellFile : m_cellFiles)
	{
		uint16_t mixingsCount = cellFile->mixing.mixingsCount;
		for (int i = 0; i < mixingsCount; i++)
		{
			CELLBF_MixDef& mixDef = cellFile->mixing.mixings[i];

			mixDef.triggerId = getID(mixDef.trigger);
			mixDef.resultId = getID(mixDef.result);
		}
	}

	spdlog::info("Assigned IDs to cell types");
}

uint16_t CellTypesDef::getID(std::string name, bool forAssigning)
{
	std::unordered_map<std::string, uint16_t>::const_iterator it = m_cellIDs.find(name);

	if (it != m_cellIDs.end())
	{
		if (forAssigning)
		{
			spdlog::error("Cell type already exists: {}", name);
			return -1;
		}

		return it->second;
	}

	if (!forAssigning)
	{
		spdlog::error("Cell type not found: {}", name);
		return -1;
	}

	m_cellIDs[name] = m_nextID;
	return m_nextID++;
}
