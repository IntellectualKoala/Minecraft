#pragma once

#include <glm/glm.hpp>

typedef glm::vec3 WorldPosition;
typedef glm::vec2 WorldPosition2D;
typedef glm::uvec3 ChunkPosition;
typedef glm::uvec2 ChunkPosition2D;
typedef glm::ivec2 ChunkLocation;

namespace PosUtils {
	static constexpr ChunkLocation ConvertWorldPosToChunkLoc(const WorldPosition& position) {
		return {
			(static_cast<int>(position.x) >> 4),
			(static_cast<int>(position.z) >> 4)
		};
	}

	static constexpr ChunkLocation ConvertWorldPosToChunkLoc(const WorldPosition2D& position) {
		return {
			(static_cast<int>(position.x) >> 4),
			(static_cast<int>(position.y) >> 4)
		};
	}

	static constexpr ChunkPosition ConvertWorldPosToChunkPos(const WorldPosition& position) {
		return {
			(static_cast<int>(position.x) & 15),
			position.y,
			(static_cast<int>(position.z) & 15)
		};
	}

	static constexpr ChunkPosition2D ConvertWorldPosToChunkPos(const WorldPosition2D& position) {
		return {
			(static_cast<int>(position.x) & 15),
			(static_cast<int>(position.y) & 15)
		};
	}

	static constexpr ChunkLocation ConvertWorldPosToChunkLoc(uint16_t x, uint16_t z) {
		return {
			(static_cast<int>(x) >> 4),
			(static_cast<int>(z) >> 4)
		};
	}
}
