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
	mutable std::shared_mutex m_Mutex;

	siv::PerlinNoise m_Noise;

public:
	World(uint32_t seed, unsigned int radius);
	World(unsigned int radius);

	void Refresh(const ChunkLocation& origin);
	void Generate(const ChunkLocation& origin);
	void Prune(const ChunkLocation& origin);

	void Update();

	void SetChunk(const ChunkLocation& location, Chunk&& chunk);
	const Chunk* SafeGetChunk(const ChunkLocation& location) const;
	Chunk& GetChunk(const ChunkLocation& location);

	inline void UpdateRadius(const ChunkLocation& origin, unsigned int radius) {
		m_Radius = radius;
		Refresh(origin);
	}

	inline const WorldGenerationState& GetGenState() const {
		return m_GenState;
	}
};
