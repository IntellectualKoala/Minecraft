#pragma once

#include "Positions.h"
#include "Block.h"

struct ChunkVertex {
	glm::vec3 pos;

	static VkVertexInputBindingDescription GetBindingDescription() {
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(ChunkVertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 1> GetAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 1> attributeDescriptions{};

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(ChunkVertex, pos);

		return attributeDescriptions;
	}
};

typedef std::pair<std::vector<ChunkVertex>, std::vector<uint32_t>> ChunkMesh;

enum class ChunkState : uint8_t {
	Ungenerated,
	GeneratedMesh,
	Buffered,
	Removed
};

void BufferMesh(std::vector<ChunkVertex> vertices, std::vector<uint32_t> indices);

class Chunk {
private:
	std::vector<Block> m_Blocks;
	ChunkMesh m_Mesh;
	ChunkState m_State;

	constexpr uint16_t PositionToIndex(const ChunkPosition& position) const {
		assert(position.x >= 0 && position.x <= Chunk::CHUNK_WIDTH
			&& position.y >= 0 && position.y <= Chunk::CHUNK_HEIGHT
			&& position.z >= 0 && position.z <= Chunk::CHUNK_DEPTH);

		return (position.x << 12) | (position.z << 8) | position.y;
	}

public:
	static constexpr uint8_t CHUNK_WIDTH = 16, CHUNK_DEPTH = 16;
	static constexpr uint16_t CHUNK_HEIGHT = 256;

	Chunk();

	void Generate();
	void GenerateMesh(ChunkLocation location);
	void BufferMesh();

	void SetBlock(const ChunkPosition& position, const Block& block);
	const Block* GetBlock(const ChunkPosition& position) const;

	inline const ChunkState& GetState() const {
		return m_State;
	}

	inline void SetRemoved() {
		m_State = ChunkState::Removed;
	}
};
