#pragma once

#include "pch.h"

#include "Chunk.h"

struct Camera
{
	glm::vec3 position = glm::vec3((Chunk::CHUNK_WIDTH / 2) + 0.5f, 52.5f, (Chunk::CHUNK_DEPTH / 2) + 0.5f),
		front = glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)),
		up = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));

	float fov = 70.0f;

	inline void SetPitchAndYaw(float pitch, float yaw) {
		yaw = fmodf(yaw, 360);
		pitch = glm::clamp(pitch, -89.9f, 89.9f);

		front = glm::normalize(glm::vec3{
			cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
			sin(glm::radians(pitch)),
			sin(glm::radians(yaw)) * cos(glm::radians(pitch))
		});
	}

	inline float GetPitch() const {
		return glm::degrees(glm::asin(front.y));
	}

	inline float GetYaw() const {
		return glm::degrees(glm::atan(front.z, front.x));
	}
};
