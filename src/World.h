#pragma once

#include "Positions.h"
#include "Chunk.h"
#include "ThreadPool.h"

enum class WorldGenerationState : uint8_t {
	Idle,
	GeneratingBlocks,
	GeneratingMeshes
};

class World {
private:
	std::unordered_map<ChunkLocation, Chunk> m_Chunks;
	unsigned int m_Radius;
	unsigned int m_NumChunks;

	WorldGenerationState m_GenState;
	ThreadPool m_ThreadPool;
	std::mutex m_Mutex;

public:
	World(unsigned int radius);

	void Refresh(const ChunkLocation& origin);
	void Generate(const ChunkLocation& origin);
	void Prune(const ChunkLocation& origin);

	void Update(const std::function<void(const ChunkMesh&)>& meshBufferFunc);

	void SetChunk(const ChunkLocation& location, Chunk&& chunk);
	Chunk& GetChunk(const ChunkLocation& location);

	inline void UpdateRadius(unsigned int radius) {
		m_NumChunks = M_PI * m_Radius * m_Radius;
		m_Radius = radius;
	}

	inline const WorldGenerationState& GetGenState() const {
		return m_GenState;
	}
};
