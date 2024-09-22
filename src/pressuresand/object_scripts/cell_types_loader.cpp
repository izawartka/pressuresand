#include "cell_types_loader.h"
#include "../assets/celltypes_def.h"
#include "../events/celltypes_load.h"

CellTypesLoader* g_cellTypesLoader = nullptr;

CellTypesLoader::CellTypesLoader(std::string dirpath) 
{
	if (g_cellTypesLoader != nullptr)
	{
		spdlog::error("Only one instance of CellTypesLoader can be present at a time");
		return;
	}
	g_cellTypesLoader = this;

	mp_dirpath = dirpath;
}

CellTypesLoader::~CellTypesLoader() 
{
	g_cellTypesLoader = nullptr;
}

void CellTypesLoader::init()
{
	createTypes();
}

void CellTypesLoader::deinit()
{
	removeTypes();
}

void CellTypesLoader::removeTypes()
{
	if (m_cellFiles == nullptr) return;

	m_cellFiles = nullptr;
	m_cellFilesCount = 0;
	m_loaded = false;

	RZUF3_AssetsManager* assetsManager = g_scene->getAssetsManager();
	assetsManager->removeAsset(mp_dirpath);
}

void CellTypesLoader::createTypes()
{
	if (m_cellFiles != nullptr) return;

	RZUF3_AssetsManager* assetsManager = g_scene->getAssetsManager();
	RZUF3_AssetDefinition assetDef;
	assetDef.filepath = mp_dirpath;
	assetDef.factory = CellTypesDef::getInstance;
	CellTypesDef* cellTypesDef = (CellTypesDef*)assetsManager->addAsset(assetDef);
	
	if (cellTypesDef == nullptr)
	{
		spdlog::error("Cell types from {} could not be loaded", mp_dirpath);
		return;
	}

	m_cellFiles = (CELLBF_File**)cellTypesDef->getContent();
	m_cellFilesCount = cellTypesDef->getContentSize();
	m_loaded = true;

	USER_CellTypesLoadEvent* loadEvent = new USER_CellTypesLoadEvent();
	g_scene->getEventsManager()->dispatchEvent(loadEvent);
	delete loadEvent;
}