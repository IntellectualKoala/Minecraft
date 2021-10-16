#include "pch.h"
#include "World.h"

World::World(unsigned int radius)
	: m_Radius(0), m_NumChunks(0), m_GenState(WorldGenerationState::Idle)
{
	UpdateRadius(radius);
}

void World::Refresh(const ChunkLocation& origin) {
	m_ThreadPool.Reset();
	Prune(origin);
	Generate(origin);
}

void World::Generate(const ChunkLocation& origin) {
	auto loop = [&](auto code) {
		const int xMax = origin.x + m_Radius;
		const int zMax = origin.y + m_Radius;
		const int xMin = origin.x - m_Radius;
		const int zMin = origin.y - m_Radius;

		std::vector<std::future<void>> futures;

		for (int x = xMin; x <= xMax; ++x) {
			for (int z = zMin; z <= zMax; ++z) {
				if (sqrt(pow(origin.x - x, 2) + pow(origin.y - z, 2)) <= m_Radius) {
					futures.emplace_back(m_ThreadPool.Enqueue([code, x, z]
					{
						code({ x, z });
					}));
				}
			}
		}

		for (auto& future : futures)
			future.wait();
	};

	m_GenState = WorldGenerationState::GeneratingBlocks;

	loop([&](const ChunkLocation& location) {
		if (m_Chunks.find(location) == m_Chunks.end()) {
			Chunk chunk;
			chunk.Generate(/* location */);
			SetChunk(location, std::move(chunk));
		}
	});

	m_GenState = WorldGenerationState::GeneratingMeshes;

	loop([&](const ChunkLocation& location) {
		GetChunk(location).GenerateMesh(location);
	});

	m_GenState = WorldGenerationState::Idle;
}

void World::Prune(const ChunkLocation& origin) {
	for (auto it = m_Chunks.begin(); it != m_Chunks.end(); ++it)
		if (sqrt(pow(origin.x - it->first.x, 2) + pow(origin.y - it->first.y, 2)) > m_Radius)
			it->second.SetRemoved();
}

void World::Update() {
	auto it = m_Chunks.begin();
	while (it != m_Chunks.end())
	{
		switch (it->second.GetState())
		{
		case ChunkState::Removed:
			{
				std::lock_guard<std::mutex> lock(m_Mutex);
				it->second.DeleteMesh();
				it = m_Chunks.erase(it);
			}
			continue;
		case ChunkState::GeneratedMesh:
			it->second.BufferMesh();
		default:
			break;
		}

		++it;
	}
}

void World::SetChunk(const ChunkLocation& location, Chunk&& chunk) {
	std::lock_guard<std::mutex> lock(m_Mutex);
	m_Chunks.emplace(location, std::move(chunk));
}

Chunk& World::GetChunk(const ChunkLocation& location) {
	std::lock_guard<std::mutex> lock(m_Mutex);
	return m_Chunks.at(location);
}
