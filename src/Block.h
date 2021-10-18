#pragma once

#include "pch.h"

#define BLOCK_TYPES(X) \
    X(Air, BLOCK_DATA_TRANSPERENT_BIT)	\
    X(Grass, BLOCK_DATA_SOLID_BIT)   \
    X(Dirt, BLOCK_DATA_SOLID_BIT)   \
    X(Glass, BLOCK_DATA_SOLID_BIT | BLOCK_DATA_TRANSPERENT_BIT)	\
	X(Stone, BLOCK_DATA_SOLID_BIT)	\
	X(Bedrock, BLOCK_DATA_SOLID_BIT)

enum class BlockType : uint8_t {
#define X(name, data) name,
	BLOCK_TYPES(X)
#undef X
	Count
};

enum BlockTypeDataBits : uint8_t {
	BLOCK_DATA_NONE = 0b00000000,
	BLOCK_DATA_SOLID_BIT = 0b00000001,
	BLOCK_DATA_TRANSPERENT_BIT = 0b00000010
};

struct BlockTypeData {
	const char* name;
	uint8_t data;
};

struct Block {
private:
	static constexpr BlockTypeData BLOCK_TYPE_DATA[] = {
#define X(name, data) { #name, data },
	BLOCK_TYPES(X)
#undef X
	};

public:
	BlockType type;

	const BlockTypeData& GetBlockTypeData() const {
		return GetBlockTypeData(type);
	}

	static const BlockTypeData& GetBlockTypeData(BlockType type) {
		return BLOCK_TYPE_DATA[static_cast<uint8_t>(type)];
	}
};
