#pragma once

#include "stdafx.h"

#include "Errors.h"

#include <glew.h>

namespace Bengine {
	class GLSLProgram
	{
	public:
		GLSLProgram();
		~GLSLProgram();

		void compileShaders(const std::string& vertShaderFilePath, const std::string& fragShaderFilePath);
		void linkShaders();
		void addAttribute(const std::string& attributeName);
		void use();
		void unuse();

		GLint getUniformLocation(const std::string& uniformName);

	private:
		void compileShader(const std::string& filePath, GLuint id);

		int m_numAttributes;
		GLuint m_programID;
		GLuint m_vertShaderID;
		GLuint m_fragShaderID;
	};
}

