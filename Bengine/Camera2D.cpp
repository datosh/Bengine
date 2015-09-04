#include "Camera2D.h"


namespace Bengine {
	Camera2D::Camera2D() :
		m_screenWidth(500),
		m_screenHeight(500),
		m_needsMatrixUpdate(true),
		m_scale(1),
		m_position(0, 0),
		m_cameraMatrix(1.0f),
		m_orthoMatrix(1.0f)
	{
	}

	Camera2D::~Camera2D()
	{
	}

	void Camera2D::init(int screenWidth, int screenHeight)
	{
		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;
		m_orthoMatrix = glm::ortho(0.0f, static_cast<float>(m_screenWidth), 0.0f, static_cast<float>(m_screenHeight));
	}

	void Camera2D::update()
	{
		if (m_needsMatrixUpdate)
		{
			glm::vec3 translate(
				-m_position.x + m_screenWidth / 2, 
				-m_position.y + m_screenHeight / 2, 
				0.0f);
			m_cameraMatrix = glm::translate(m_orthoMatrix, translate);

			glm::vec3 scale(m_scale, m_scale, 0.0f);
			m_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * m_cameraMatrix;

			m_needsMatrixUpdate = false;
		}
	}
	glm::vec2 Camera2D::convertScreenToWorld(glm::vec2 screenCoords)
	{
		// Invert Y direction
		screenCoords.y = m_screenHeight - screenCoords.y;
		// Make it that 0 is the center
		screenCoords -= glm::vec2(m_screenWidth / 2, m_screenHeight / 2);
		// Scale the coordinates
		screenCoords /= m_scale;
		// Translate with the camera position
		screenCoords += m_position;

		return screenCoords;
	}
}