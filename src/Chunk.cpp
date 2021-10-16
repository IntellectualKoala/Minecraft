#include "pch.h"
#include "Chunk.h"

Chunk::Chunk()
	: m_State(ChunkState::Ungenerated)
{
}

void Chunk::Generate() {
	m_State = ChunkState::Ungenerated;

	m_Blocks.resize(Chunk::CHUNK_WIDTH * Chunk::CHUNK_HEIGHT * Chunk::CHUNK_DEPTH);

	for (uint8_t x = 0; x < Chunk::CHUNK_WIDTH; ++x) {
		for (uint8_t z = 0; z < Chunk::CHUNK_DEPTH; ++z) {
			for (uint16_t y = 0; y < Chunk::CHUNK_HEIGHT; ++y) {
				SetBlock({ x, y, z }, { y <= 50 ? BlockType::Grass : BlockType::Air });
			}
		}
	}

	m_State = ChunkState::Generated;
}

void Chunk::GenerateMesh(ChunkLocation location) {
	if (m_State != ChunkState::Generated)
		return;

	location <<= 4;

	int index = 0;
	uint16_t y = 50;
	for (uint8_t x = 0; x < 16; ++x) {
		for (uint8_t z = 0; z < 16; ++z) {
			// This is temporary
			m_Mesh.vertices.emplace_back(ChunkVertex{ { location.x + x, y, location.y + z + 1.0f } });
			m_Mesh.vertices.emplace_back(ChunkVertex{ { location.x + x, y, location.y + z } });
			m_Mesh.vertices.emplace_back(ChunkVertex{ { location.x + x + 1.0f, y, location.y + z } });
			m_Mesh.vertices.emplace_back(ChunkVertex{ { location.x + x + 1.0f, y, location.y + z + 1.0f } });

			m_Mesh.indices.emplace_back(index);
			m_Mesh.indices.emplace_back(index + 2);
			m_Mesh.indices.emplace_back(index + 1);
			m_Mesh.indices.emplace_back(index);
			m_Mesh.indices.emplace_back(index + 3);
			m_Mesh.indices.emplace_back(index + 2);

			index += 4;
		}
	}

	m_State = ChunkState::GeneratedMesh;
}

void Chunk::BufferMesh() {
	m_Mesh.id = ::BufferMesh(m_Mesh.vertices, m_Mesh.indices);

	m_State = ChunkState::Buffered;

	m_Mesh.vertices.clear();
	m_Mesh.indices.clear();
}

void Chunk::DeleteMesh() {
	::DeleteMesh(m_Mesh.id);
}

void Chunk::SetBlock(const ChunkPosition& position, const Block& block) {
	m_Blocks[PositionToIndex(position)] = block;
}

const Block* Chunk::GetBlock(const ChunkPosition& position) const {
	return &m_Blocks[PositionToIndex(position)];
}
