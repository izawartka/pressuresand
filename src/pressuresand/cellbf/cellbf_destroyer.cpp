#include "cellbf_destroyer.h"

void CELLBF_Destroyer::destroy(CELLBF_File& cellFile)
{
	// header
	delete[] cellFile.header;

	// info block
	delete[] cellFile.info.name;

	// mixing block
	for (uint16_t i = 0; i < cellFile.mixing.mixingsCount; i++)
	{
		delete[] cellFile.mixing.mixings[i].trigger;
		delete[] cellFile.mixing.mixings[i].result;
	}
	delete[] cellFile.mixing.mixings;
}
