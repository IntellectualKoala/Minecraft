#include "pch.h"
#include "Chunk.h"

Chunk::Chunk()
	: m_State(ChunkState::Ungenerated)
{

}

void Chunk::Generate() {
	m_Blocks.resize(Chunk::CHUNK_WIDTH * Chunk::CHUNK_HEIGHT * Chunk::CHUNK_DEPTH);

	for (uint8_t x = 0; x < Chunk::CHUNK_WIDTH; ++x) {
		for (uint8_t z = 0; z < Chunk::CHUNK_DEPTH; ++z) {
			for (uint16_t y = 0; y < Chunk::CHUNK_HEIGHT; ++y) {
				SetBlock({ x, y, z }, { y <= 50 ? BlockType::Grass : BlockType::Air });
			}
		}
	}
}

void Chunk::GenerateMesh(ChunkLocation location) {
	location <<= 4;

	int index = 0;
	uint16_t y = 50;
	for (uint8_t x = 0; x < 16; ++x) {
		for (uint8_t z = 0; z < 16; ++z) {
			m_Mesh.first.emplace_back(ChunkVertex{ { location.x + x, y, location.y + z + 1.0f } });
			m_Mesh.first.emplace_back(ChunkVertex{ { location.x + x, y, location.y + z } });
			m_Mesh.first.emplace_back(ChunkVertex{ { location.x + x + 1.0f, y, location.y + z } });
			m_Mesh.first.emplace_back(ChunkVertex{ { location.x + x + 1.0f, y, location.y + z + 1.0f } });

			m_Mesh.second.emplace_back(index);
			m_Mesh.second.emplace_back(index + 2);
			m_Mesh.second.emplace_back(index + 1);
			m_Mesh.second.emplace_back(index);
			m_Mesh.second.emplace_back(index + 3);
			m_Mesh.second.emplace_back(index + 2);

			index += 4;
		}
	}

	m_State = ChunkState::GeneratedMesh;
}

void Chunk::BufferMesh(const std::function<void(const ChunkMesh&)>& meshBufferFunc) {
	meshBufferFunc(m_Mesh);

	m_State = ChunkState::Buffered;

	m_Mesh.first.clear();
	m_Mesh.second.clear();
}

void Chunk::SetBlock(const ChunkPosition& position, const Block& block) {
	m_Blocks[PositionToIndex(position)] = block;
}

const Block* Chunk::GetBlock(const ChunkPosition& position) const {
	return &m_Blocks[PositionToIndex(position)];
}
