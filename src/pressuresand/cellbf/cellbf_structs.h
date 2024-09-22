#pragma once
#include "../common.h"

#define CELLBF_VERSION 2

struct CELLBF_InfoBlock {
	uint16_t version;
	uint8_t nameLength;
	char* name;
	bool display;
	bool solid;
	bool gravity;
	bool fluid;
	uint16_t density;
	SDL_Color color;
	SDL_Color colorVariation;

	uint16_t id = -1;
};

struct CELLBF_MixDef {
	uint8_t triggerLength;
	char* trigger;
	uint8_t resultLength;
	char* result;
	uint16_t chance;
	bool reverse;

	uint16_t triggerId = -1;
	uint16_t resultId = -1;
};

struct CELLBF_MixingBlock {
	uint16_t mixingsCount;
	CELLBF_MixDef* mixings;
};

struct CELLBF_File {
	char* header;
	uint32_t size;
	CELLBF_InfoBlock info;
	CELLBF_MixingBlock mixing;
};