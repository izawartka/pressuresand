#include "cellbf_reader.h"

// example:
// _READ(infoBlock.version);
// file.read(reinterpret_cast<char*>(&infoBlock.version), sizeof(infoBlock.version));
#define _READ(x) file.read(reinterpret_cast<char*>(&x), sizeof(x))

// example:
// _READ_STR(cellFile.header, 6);
// file.read(cellFile.header, 6);
#define _READ_STR(x, y) \
    x = new char[y + 1]; \
	x[y] = 0; \
    file.read(x, y)

// example:
// _READ_ARR(cellFile.mixing.mixings, cellFile.mixing.mixingsCount);
// file.read(reinterpret_cast<char*>(viewDef.mixings), viewDef.mixingsCount * sizeof(uint16_t));
#define _READ_ARR(x, y) file.read(reinterpret_cast<char*>(x), y * sizeof(x[0]))

// example:
// _READ_COLOR(infoBlock.color);
// uint8_t color[3];
// file.read(reinterpret_cast<char*>(color), 3);
// infoBlock.color = {color[0], color[1], color[2], 255};
#define _READ_COLOR(x) \
	{uint8_t color[3]; \
	file.read(reinterpret_cast<char*>(color), 3); \
	x = {color[0], color[1], color[2], 255};}

bool CELLBF_Reader::readFile(std::ifstream& file, CELLBF_File& cellFile) {
    _READ_STR(cellFile.header, 6);
    _READ(cellFile.size);

    if (strcmp(cellFile.header, "CELLBF") != 0) {
		spdlog::error("CELL file: invalid header");
		return false;
	}

    uint32_t currentPos = 10;

    while (currentPos < cellFile.size) {
        char header[5] = {0};
        file.read(header, 4);
        uint32_t blockSize = 0; 
        _READ(blockSize);

        if (strcmp(header, "INFO") == 0) {
            readInfoBlock(file, cellFile.info);
            if (cellFile.info.version != CELLBF_VERSION) {
                spdlog::error("CELL file {}: unsupported version {}", cellFile.info.name, cellFile.info.version);
                return false;
            }
        }
        else if (strcmp(header, "MIXS") == 0) {
            readMixingBlock(file, cellFile.mixing);
		} else {
			spdlog::warn("CELL file {}: unknown block {}", cellFile.info.name, header);
			file.seekg(blockSize, std::ios::cur);
		}

		currentPos += blockSize + 8;

        int32_t tellg = file.tellg();
        if (currentPos != tellg) {
            spdlog::error("CELL file {}: invalid block {}", cellFile.info.name, header);
            return false;
		}
	}

    return true;
}

bool CELLBF_Reader::readInfoBlock(std::ifstream& file, CELLBF_InfoBlock& infoBlock) {
    _READ(infoBlock.version);
    _READ(infoBlock.nameLength);
    _READ_STR(infoBlock.name, infoBlock.nameLength);
    _READ(infoBlock.display);
    _READ(infoBlock.solid);
    _READ(infoBlock.gravity);
    _READ(infoBlock.fluid);
    _READ(infoBlock.density);
    _READ_COLOR(infoBlock.color);
    _READ_COLOR(infoBlock.colorVariation);

    return true;
}

bool CELLBF_Reader::readMixDef(std::ifstream& file, CELLBF_MixDef& mixDef)
{
    _READ(mixDef.triggerLength);
    _READ_STR(mixDef.trigger, mixDef.triggerLength);
    _READ(mixDef.resultLength);
    _READ_STR(mixDef.result, mixDef.resultLength);
    _READ(mixDef.chance);
    _READ(mixDef.reverse);

    return true;
}

bool CELLBF_Reader::readMixingBlock(std::ifstream& file, CELLBF_MixingBlock& mixingBlock)
{
    _READ(mixingBlock.mixingsCount);
	mixingBlock.mixings = new CELLBF_MixDef[mixingBlock.mixingsCount];
    for (int i = 0; i < mixingBlock.mixingsCount; i++) {
		readMixDef(file, mixingBlock.mixings[i]);
	}

	return true;
}
