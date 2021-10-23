#include "pch.h"
#include "Chunk.h"

#include "World.h"

Chunk::Chunk()
	: m_State(ChunkState::Ungenerated)
{
}

void Chunk::Generate(const siv::PerlinNoise& noise, ChunkLocation location) {
	m_State = ChunkState::Generating;

	m_Blocks.resize(Chunk::CHUNK_WIDTH * Chunk::CHUNK_HEIGHT * Chunk::CHUNK_DEPTH);

	location <<= 4;

	for (uint8_t x = 0; x < Chunk::CHUNK_WIDTH; ++x) {
		for (uint8_t z = 0; z < Chunk::CHUNK_DEPTH; ++z) {
			const double random = noise.noise2D_0_1((location.x + x) * 0.025, (location.y + z) * 0.025) * 50;
			const int grassHeight = 25 + random;
			const int dirtHeight = (grassHeight - 3);

			for (uint16_t y = 0; y < Chunk::CHUNK_HEIGHT; ++y) {
				if (y > grassHeight)
					SetBlock({ x, y, z }, { BlockType::Air });
				else if (y == grassHeight)
					SetBlock({ x, y, z }, { BlockType::Grass });
				else if (y >= dirtHeight)
					SetBlock({ x, y, z }, { BlockType::Dirt });
				else if (y > 0)
					SetBlock({ x, y, z }, { BlockType::Stone });
				else
					SetBlock({ x, y, z }, { BlockType::Bedrock });
			}
		}
	}

	m_State = ChunkState::Generated;
}

void Chunk::GenerateMesh(World* world, ChunkLocation location) {
	assert(m_State == ChunkState::Generated);
	m_State = ChunkState::GeneratingMesh;

	// A precondition for Chunk::GenerateMesh is that each
	// chunk bordering the current one is already generated.
	Chunk& frontChunk = world->GetChunk(ChunkLocation{ location.x, location.y + 1 });
	Chunk& rightChunk = world->GetChunk(ChunkLocation{ location.x + 1, location.y });

	location <<= 4;

	for (uint8_t x = 0; x < Chunk::CHUNK_WIDTH; ++x) {
		bool xNotFinished = x != (Chunk::CHUNK_WIDTH - 1);

		for (uint8_t z = 0; bool zNotFinished = (z < Chunk::CHUNK_DEPTH); ++z) {
			for (uint16_t y = 0; y < Chunk::CHUNK_HEIGHT; ++y) {
				auto block = GetBlock({ x, y, z });
				auto& blockData = block->GetBlockTypeData();

				const iWorldPosition frontBlockPosition = { x, y, z + 1 };
				const iWorldPosition rightBlockPosition = { x + 1, y, z };
				const iWorldPosition topBlockPosition = { x, y + 1, z };

				auto* frontBlock = z != (Chunk::CHUNK_DEPTH - 1)
					? GetBlock(frontBlockPosition)
					: frontChunk.GetBlock({ frontBlockPosition.x, frontBlockPosition.y, 0 });
				auto* rightBlock = xNotFinished
					? GetBlock(rightBlockPosition)
					: rightChunk.GetBlock({ 0, rightBlockPosition.y, rightBlockPosition.z });

				auto& frontBlockData = frontBlock->GetBlockTypeData();
				auto& rightBlockData = rightBlock->GetBlockTypeData();

				if ((blockData.data & BlockTypeDataBits::BLOCK_DATA_SOLID_BIT) != 0) {
					const iWorldPosition realPosition = {
						x + location.x,
						y,
						z + location.y
					};

					if ((frontBlockData.data & BlockTypeDataBits::BLOCK_DATA_TRANSPERENT_BIT) != 0)
						AddMeshFace(blockData, BlockFace::Front, realPosition);

					if ((rightBlockData.data & BlockTypeDataBits::BLOCK_DATA_TRANSPERENT_BIT) != 0)
						AddMeshFace(blockData, BlockFace::Right, realPosition);

					if (topBlockPosition.y < Chunk::CHUNK_HEIGHT
						&& (GetBlock(topBlockPosition)->GetBlockTypeData().data & BlockTypeDataBits::BLOCK_DATA_TRANSPERENT_BIT) != 0)
						AddMeshFace(blockData, BlockFace::Top, realPosition);
				}

				if ((blockData.data & BlockTypeDataBits::BLOCK_DATA_TRANSPERENT_BIT) != 0) {
					if ((frontBlockData.data & BlockTypeDataBits::BLOCK_DATA_SOLID_BIT) != 0) {
						AddMeshFace(frontBlockData, BlockFace::Back, {
							frontBlockPosition.x + location.x,
							frontBlockPosition.y,
							frontBlockPosition.z + location.y
						});
					}

					if ((rightBlockData.data & BlockTypeDataBits::BLOCK_DATA_SOLID_BIT) != 0) {
						AddMeshFace(rightBlockData, BlockFace::Left, {
							rightBlockPosition.x + location.x,
							rightBlockPosition.y,
							rightBlockPosition.z + location.y
						});
					}

					if (topBlockPosition.y < Chunk::CHUNK_HEIGHT) {
						auto& topBlockData = GetBlock(topBlockPosition)->GetBlockTypeData();

						if ((topBlockData.data & BlockTypeDataBits::BLOCK_DATA_SOLID_BIT) != 0) {
							AddMeshFace(topBlockData, BlockFace::Bottom, {
								topBlockPosition.x + location.x,
								topBlockPosition.y,
								topBlockPosition.z + location.y
							});
						}
					}
				}
			}
		}
	}

	m_State = ChunkState::GeneratedMesh;
}

void Chunk::AddMeshFace(const BlockTypeData& blockTypeData, const BlockFace& face, const WorldPosition& position) {
	BlockFaceData blockFaceData;

	switch (face) {
	case BlockFace::Front:
		blockFaceData = FRONT_BLOCK_FACE_DATA;
		break;
	case BlockFace::Back:
		blockFaceData = BACK_BLOCK_FACE_DATA;
		break;
	case BlockFace::Left:
		blockFaceData = LEFT_BLOCK_FACE_DATA;
		break;
	case BlockFace::Right:
		blockFaceData = RIGHT_BLOCK_FACE_DATA;
		break;
	case BlockFace::Top:
		blockFaceData = TOP_BLOCK_FACE_DATA;
		break;
	case BlockFace::Bottom:
		blockFaceData = BOTTOM_BLOCK_FACE_DATA;
		break;
	}

	auto sizeOfVertices = m_MeshBuilder.vertices.size();

	for (uint8_t i = 0; i < 4; ++i)
	{
		auto& vertex = blockFaceData.vertices[i];

		m_MeshBuilder.vertices.emplace_back(ChunkVertex
		{
			vertex.position + position,
			vertex.texcoords
		});
	}

	std::vector<uint32_t> indices{
		0, 1, 2, 2, 3, 0
	};

	for (size_t i = 0; i < indices.size(); ++i)
		m_MeshBuilder.indices.emplace_back(indices[i] + sizeOfVertices);
}

void Chunk::BufferMesh() {
	assert(m_State == ChunkState::GeneratedMesh);
	m_State = ChunkState::Buffering;

	m_MeshBuilder.id = ::BufferMesh(std::move(m_MeshBuilder.vertices), std::move(m_MeshBuilder.indices));

	m_MeshBuilder.vertices.clear();
	m_MeshBuilder.indices.clear();

	m_State = ChunkState::Buffered;
}

void Chunk::DeleteMesh() {
	assert(m_State == ChunkState::Buffered);

	::DeleteMesh(m_MeshBuilder.id);
}

void Chunk::SetBlock(const ChunkPosition& position, const Block& block) {
	m_Blocks[PositionToIndex(position)] = block;
}

const Block* Chunk::GetBlock(const ChunkPosition& position) const {
	return &m_Blocks[PositionToIndex(position)];
}
