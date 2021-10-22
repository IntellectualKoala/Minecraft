#pragma once

#include "Positions.h"
#include "Block.h"

class World;

struct BlockFaceVertex
{
	glm::vec3 position,
		perpendicularNormal1,
		perpendicularNormal2;

	glm::vec2 texcoords;
};

struct BlockFaceData
{
	std::array<BlockFaceVertex, 4> vertices;
	glm::vec3 normal;
};

static inline constexpr BlockFaceData FRONT_BLOCK_FACE_DATA = {
	{
		BlockFaceVertex { glm::vec3 { 0.0f, 1.0f, 1.0f }, glm::vec3 { -1.0f, 0.0f, 0.0f }, glm::vec3 { 0.0f, 1.0f, 0.0f }, glm::vec2 { 0.0f, 1.0f } },
		BlockFaceVertex { glm::vec3 { 0.0f, 0.0f, 1.0f }, glm::vec3 { -1.0f, 0.0f, 0.0f }, glm::vec3 { 0.0f, -1.0f, 0.0f }, glm::vec2 { 0.0f, 0.0f } },
		BlockFaceVertex { glm::vec3 { 1.0f, 0.0f, 1.0f }, glm::vec3 { 1.0f, 0.0f, 0.0f }, glm::vec3 { 0.0f, -1.0f, 0.0f }, glm::vec2 { 1.0f, 0.0f } },
		BlockFaceVertex { glm::vec3 { 1.0f, 1.0f, 1.0f }, glm::vec3 { 1.0f, 0.0f, 0.0f }, glm::vec3 { 0.0f, 1.0f, 0.0f }, glm::vec2 { 1.0f, 1.0f } }
	},
	glm::vec3 { 0.0f, 0.0f, 1.0f }
};

static inline constexpr BlockFaceData BACK_BLOCK_FACE_DATA = {
	{
		BlockFaceVertex { glm::vec3 { 1.0f, 0.0f, 0.0f }, glm::vec3 { 1.0f, 0.0f, 0.0f }, glm::vec3 { 0.0f, -1.0f, 0.0f }, glm::vec2 { 0.0f, 0.0f } },
		BlockFaceVertex { glm::vec3 { 0.0f, 0.0f, 0.0f }, glm::vec3 { -1.0f, 0.0f, 0.0f }, glm::vec3 { 0.0f, -1.0f, 0.0f }, glm::vec2 { 1.0f, 0.0f } },
		BlockFaceVertex { glm::vec3 { 0.0f, 1.0f, 0.0f }, glm::vec3 { -1.0f, 0.0f, 0.0f }, glm::vec3 { 0.0f, 1.0f, 0.0f }, glm::vec2 { 1.0f, 1.0f } },
		BlockFaceVertex { glm::vec3 { 1.0f, 1.0f, 0.0f }, glm::vec3 { 1.0f, 0.0f, 0.0f }, glm::vec3 { 0.0f, 1.0f, 0.0f }, glm::vec2 { 0.0f, 1.0f } }
	},
	glm::vec3 { 0.0f, 0.0f, -1.0f }
};

static inline constexpr BlockFaceData LEFT_BLOCK_FACE_DATA = {
	{
		BlockFaceVertex { glm::vec3 { 0.0f, 0.0f, 0.0f }, glm::vec3 { 0.0f, 0.0f, -1.0f }, glm::vec3 { 0.0f, -1.0f, 0.0f }, glm::vec2 { 0.0f, 0.0f } },
		BlockFaceVertex { glm::vec3 { 0.0f, 0.0f, 1.0f }, glm::vec3 { 0.0f, 0.0f, 1.0f }, glm::vec3 { 0.0f, -1.0f, 0.0f }, glm::vec2 { 1.0f, 0.0f } },
		BlockFaceVertex { glm::vec3 { 0.0f, 1.0f, 1.0f }, glm::vec3 { 0.0f, 0.0f, 1.0f }, glm::vec3 { 0.0f, 1.0f, 0.0f }, glm::vec2 { 1.0f, 1.0f } },
		BlockFaceVertex { glm::vec3 { 0.0f, 1.0f, 0.0f }, glm::vec3 { 0.0f, 0.0f, -1.0f }, glm::vec3 { 0.0f, 1.0f, 0.0f }, glm::vec2 { 0.0f, 1.0f } }
	},
	glm::vec3 { -1.0f, 0.0f, 0.0f }
};

static inline constexpr BlockFaceData RIGHT_BLOCK_FACE_DATA = {
	{
		BlockFaceVertex { glm::vec3 { 1.0f, 1.0f, 1.0f }, glm::vec3 { 0.0f, 0.0f, 1.0f }, glm::vec3 { 0.0f, 1.0f, 0.0f }, glm::vec2 { 0.0f, 1.0f } },
		BlockFaceVertex { glm::vec3 { 1.0f, 0.0f, 1.0f }, glm::vec3 { 0.0f, 0.0f, 1.0f }, glm::vec3 { 0.0f, -1.0f, 0.0f }, glm::vec2 { 0.0f, 0.0f } },
		BlockFaceVertex { glm::vec3 { 1.0f, 0.0f, 0.0f }, glm::vec3 { 0.0f, 0.0f, -1.0f }, glm::vec3 { 0.0f, -1.0f, 0.0f }, glm::vec2 { 1.0f, 0.0f } },
		BlockFaceVertex { glm::vec3 { 1.0f, 1.0f, 0.0f }, glm::vec3 { 0.0f, 0.0f, -1.0f }, glm::vec3 { 0.0f, 1.0f, 0.0f }, glm::vec2 { 1.0f, 1.0f } }
	},
	glm::vec3 { 1.0f, 0.0f, 0.0f }
};

static inline constexpr BlockFaceData TOP_BLOCK_FACE_DATA = {
	{
		BlockFaceVertex { glm::vec3 { 1.0f, 1.0f, 0.0f }, glm::vec3 { 0.0f, 0.0f, -1.0f }, glm::vec3 { 1.0f, 0.0f, 0.0f }, glm::vec2 { 0.0f, 0.0f } },
		BlockFaceVertex { glm::vec3 { 0.0f, 1.0f, 0.0f }, glm::vec3 { 0.0f, 0.0f, -1.0f }, glm::vec3 { -1.0f, 0.0f, 0.0f }, glm::vec2 { 1.0f, 0.0f } },
		BlockFaceVertex { glm::vec3 { 0.0f, 1.0f, 1.0f }, glm::vec3 { 0.0f, 0.0f, 1.0f }, glm::vec3 { -1.0f, 0.0f, 0.0f }, glm::vec2 { 1.0f, 1.0f } },
		BlockFaceVertex { glm::vec3 { 1.0f, 1.0f, 1.0f }, glm::vec3 { 0.0f, 0.0f, 1.0f }, glm::vec3 { 1.0f, 0.0f, 0.0f }, glm::vec2 { 0.0f, 1.0f } }
	},
	glm::vec3 { 0.0f, 1.0f, 0.0f }
};

static inline constexpr BlockFaceData BOTTOM_BLOCK_FACE_DATA = {
	{
		BlockFaceVertex { glm::vec3 { 0.0f, 0.0f, 1.0f }, glm::vec3 { 0.0f, 0.0f, 1.0f }, glm::vec3 { -1.0f, 0.0f, 0.0f }, glm::vec2 { 1.0f, 0.0f } },
		BlockFaceVertex { glm::vec3 { 0.0f, 0.0f, 0.0f }, glm::vec3 { 0.0f, 0.0f, -1.0f }, glm::vec3 { -1.0f, 0.0f, 0.0f }, glm::vec2 { 1.0f, 1.0f } },
		BlockFaceVertex { glm::vec3 { 1.0f, 0.0f, 0.0f }, glm::vec3 { 0.0f, 0.0f, -1.0f }, glm::vec3 { 1.0f, 0.0f, 0.0f }, glm::vec2 { 0.0f, 1.0f } },
		BlockFaceVertex { glm::vec3 { 1.0f, 0.0f, 1.0f }, glm::vec3 { 0.0f, 0.0f, 1.0f }, glm::vec3 { 1.0f, 0.0f, 0.0f }, glm::vec2 { 0.0f, 0.0f } }
	},
	glm::vec3 { 0.0f, -1.0f, 0.0f }
};

struct ChunkVertex {
	glm::vec3 pos;
	glm::vec2 texCoord;

	static VkVertexInputBindingDescription GetBindingDescription() {
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(ChunkVertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(ChunkVertex, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(ChunkVertex, texCoord);

		return attributeDescriptions;
	}
};

struct ChunkMeshBuilder {
	unsigned int id;
	std::vector<ChunkVertex> vertices;
	std::vector<uint32_t> indices;
};

enum class ChunkState : uint8_t {
	Ungenerated,
	Generating,
	Generated,
	GeneratingMesh,
	GeneratedMesh,
	Buffering,
	Buffered
};

unsigned int BufferMesh(const std::vector<ChunkVertex>&& vertices, const std::vector<uint32_t>&& indices);
void DeleteMesh(unsigned int id);

class Chunk {
private:
	std::vector<Block> m_Blocks;
	ChunkMeshBuilder m_MeshBuilder;
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

	bool m_Removed = false;

	Chunk();

	void Generate(const siv::PerlinNoise& noise, ChunkLocation location);
	void GenerateMesh(World* world, ChunkLocation location);

	enum class BlockFace : uint8_t {
		Front,
		Back,
		Left,
		Right,
		Top,
		Bottom
	};

	void AddMeshFace(const BlockTypeData& blockTypeData, const BlockFace& face, const WorldPosition& position);

	void BufferMesh();
	void DeleteMesh();

	void SetBlock(const ChunkPosition& position, const Block& block);
	const Block* GetBlock(const ChunkPosition& position) const;

	inline const ChunkState& GetState() const {
		return m_State;
	}

	inline bool IsRemoved() {
		return m_Removed;
	}

	inline void SetRemoved() {
		m_Removed = true;
	}
};
