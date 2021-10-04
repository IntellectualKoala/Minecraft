#include "pch.h"
#include "World.h"

World::World(unsigned int radius)
	: m_Radius(0), m_NumChunks(0), m_GenState(WorldGenerationState::Idle)
{
	UpdateRadius(radius);
}

void World::Refresh(const ChunkLocation& origin) {
	Prune(origin);
	m_ThreadPool.Reset();
	Generate(origin);
}

void World::Generate(const ChunkLocation& origin) {
	auto loop = [&](auto code) {
		// This is temporary
		static const int worldWidth = 10;
		static const int worldHeight = 10;

		std::vector<std::future<void>> futures;

		for (int x = 0; x < worldWidth; ++x) {
			for (int z = 0; z < worldHeight; ++z) {
				futures.emplace_back(m_ThreadPool.Enqueue([&, x, z]
				{
					code({ x, z });
				}));
			}
		}

		for (auto& future : futures)
			future.wait();
	};

	m_GenState = WorldGenerationState::GeneratingBlocks;

	loop([&](const ChunkLocation& location) {
		SetChunk(location, std::move(Chunk()));
		GetChunk(location).Generate(/* location */);
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

void World::Update(const std::function<void(const ChunkMesh&)>& meshBufferFunc) {
	auto it = m_Chunks.begin();
	while (it != m_Chunks.end())
	{
		switch (it->second.GetState())
		{
		case ChunkState::Removed:
		{
			std::lock_guard lock(m_Mutex);
			m_Chunks.erase(it);
		}
		continue;
		case ChunkState::GeneratedMesh:
			it->second.BufferMesh(meshBufferFunc);
		}

		++it;
	}
}

void World::SetChunk(const ChunkLocation& location, Chunk&& chunk) {
	std::lock_guard lock(m_Mutex);
	m_Chunks.emplace(location, std::move(chunk));
}

Chunk& World::GetChunk(const ChunkLocation& location) {
	std::lock_guard lock(m_Mutex);
	return m_Chunks.at(location);
}
