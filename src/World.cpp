#include "pch.h"
#include "World.h"

World::World(uint32_t seed, unsigned int radius)
	: m_Radius(radius), m_GenState(WorldGenerationState::Idle),
	m_Noise(siv::PerlinNoise(seed))
{
}

World::World(unsigned int radius)
	: m_Radius(radius), m_GenState(WorldGenerationState::Idle),
	m_Noise(siv::PerlinNoise(std::mt19937()))
{
}

void World::Refresh(const ChunkLocation& origin) {
	m_ThreadPool.Reset();
	Prune(origin);
	Generate(origin);
}

void World::Generate(const ChunkLocation& origin) {
	auto loop = [&](unsigned int radius, auto code) {
		const int xMax = origin.x + radius;
		const int zMax = origin.y + radius;
		const int xMin = origin.x - radius;
		const int zMin = origin.y - radius;

		std::vector<std::future<void>> futures;

		for (int x = xMin; x <= xMax; ++x) {
			for (int z = zMin; z <= zMax; ++z) {
				if (sqrt(pow(origin.x - x, 2) + pow(origin.y - z, 2)) <= radius) {
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

	loop(m_Radius + 1, [&](const ChunkLocation& location) {
		{
			std::shared_lock<std::shared_mutex> lock(m_Mutex);
			if (m_Chunks.find(location) != m_Chunks.end())
				return;
		}

		Chunk chunk;
		chunk.Generate(m_Noise, location);
		SetChunk(location, std::move(chunk));
	});

	m_GenState = WorldGenerationState::GeneratingMeshes;

	loop(m_Radius, [&](const ChunkLocation& location) {
		Chunk& chunk = GetChunk(location);

		if (chunk.GetState() == ChunkState::Generated)
			chunk.GenerateMesh(this, location);
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
		if (it->second.IsRemoved()) {
			std::lock_guard<std::shared_mutex> lock(m_Mutex);

			if (it->second.GetState() == ChunkState::Buffered)
				it->second.DeleteMesh();

			it = m_Chunks.erase(it);
			continue;
		}

		if (it->second.GetState() == ChunkState::GeneratedMesh)
			it->second.BufferMesh();

		++it;
	}
}

void World::SetChunk(const ChunkLocation& location, Chunk&& chunk) {
	std::lock_guard<std::shared_mutex> lock(m_Mutex);
	m_Chunks.emplace(location, std::move(chunk));
}

const Chunk* World::SafeGetChunk(const ChunkLocation& location) const {
	const auto& found = m_Chunks.find(location);
	return (found == m_Chunks.end()) ? nullptr : &found->second;
}

Chunk& World::GetChunk(const ChunkLocation& location) {
	std::shared_lock<std::shared_mutex> lock(m_Mutex);
	return m_Chunks.at(location);
}
