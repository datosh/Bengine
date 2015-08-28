#pragma once

#include "stdafx.h"

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

namespace Bengine {
	class Camera2D
	{
	public:
		Camera2D();
		~Camera2D();

		void init(int screenWidth, int screenHeight);

		void update();

		void setPositon(const glm::vec2& position) { m_position = position; m_needsMatrixUpdate = true; }
		void setScale(float scale) { m_scale = scale; m_needsMatrixUpdate = true; }

		glm::vec2 getPosition() { return m_position; }
		float getScale() { return m_scale; }
		glm::mat4 getCameraMatrix() { return m_cameraMatrix; }

	private:
		int m_screenWidth, m_screenHeight;
		bool m_needsMatrixUpdate;
		float m_scale;
		glm::vec2 m_position;
		glm::mat4 m_cameraMatrix;
		glm::mat4 m_orthoMatrix;
	};
}