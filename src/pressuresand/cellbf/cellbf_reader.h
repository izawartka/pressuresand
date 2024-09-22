#pragma once
#include "../common.h"
#include "cellbf_structs.h"

class CELLBF_Reader {
public:
	static bool readFile(std::ifstream& file, CELLBF_File& cellFile);

	static bool readInfoBlock(std::ifstream& file, CELLBF_InfoBlock& infoBlock);
	static bool readMixDef(std::ifstream& file, CELLBF_MixDef& mixDef);
	static bool readMixingBlock(std::ifstream& file, CELLBF_MixingBlock& mixingBlock);
};