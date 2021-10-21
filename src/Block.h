#pragma once

#include "pch.h"

#define BLOCK_TYPES(X) \
    X(Air, BLOCK_DATA_TRANSPERENT_BIT, std::nullopt)	\
    X(Grass, BLOCK_DATA_SOLID_BIT, std::array { 0, 0, 0, 0, 1, 2 })   \
    X(Dirt, BLOCK_DATA_SOLID_BIT, std::array { 2, 2, 2, 2, 2, 2 })   \
	X(Stone, BLOCK_DATA_SOLID_BIT, std::array { 3, 3, 3, 3, 3, 3 })	\
	X(Cobblestone, BLOCK_DATA_SOLID_BIT, std::array { 4, 4, 4, 4, 4, 4 })	\
	X(Bedrock, BLOCK_DATA_SOLID_BIT, std::array { 5, 5, 5, 5, 5, 5 })	\
	X(Wood, BLOCK_DATA_SOLID_BIT, std::array { 6, 6, 6, 6, 6, 6 })	\
	X(Log, BLOCK_DATA_SOLID_BIT, std::array { 7, 7, 7, 7, 8, 8 })	\
	X(Leaves, BLOCK_DATA_SOLID_BIT, std::array { 9, 9, 9, 9, 9, 9 })	\
    X(Glass, BLOCK_DATA_SOLID_BIT | BLOCK_DATA_TRANSPERENT_BIT, std::array { 10, 10, 10, 10, 10, 10 })	\

enum class BlockType : uint8_t {
#define X(name, ...) name,
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
	const std::optional<std::array<int, 6>> faces;
};

struct Block {
private:
	static constexpr BlockTypeData BLOCK_TYPE_DATA[] = {
#define X(name, data, ...) { #name, data, __VA_ARGS__ },
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
