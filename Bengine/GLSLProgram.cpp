#include "GLSLProgram.h"

namespace Bengine {
	GLSLProgram::GLSLProgram() : m_numAttributes(0), m_programID(0), m_vertShaderID(0), m_fragShaderID(0)
	{
	}

	GLSLProgram::~GLSLProgram()
	{
	}

	void GLSLProgram::compileShaders(const std::string & vertShaderFilePath, const std::string & fragShaderFilePath)
	{
		m_programID = glCreateProgram();

		m_vertShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (!m_vertShaderID)
		{
			fatalError("Vertex shader failed to be created!");
		}

		m_fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (!m_fragShaderID)
		{
			fatalError("Fragment shader failed to be created!");
		}

		compileShader(vertShaderFilePath, m_vertShaderID);
		compileShader(fragShaderFilePath, m_fragShaderID);
	}

	void GLSLProgram::linkShaders()
	{
		glAttachShader(m_programID, m_vertShaderID);
		glAttachShader(m_programID, m_fragShaderID);

		glLinkProgram(m_programID);

		GLint success = 0;
		glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
		if (success == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> errorLog(maxLength);
			glGetProgramInfoLog(m_programID, maxLength, &maxLength, &errorLog[0]);

			glDeleteProgram(m_programID);
			glDeleteShader(m_vertShaderID);
			glDeleteShader(m_fragShaderID);

			std::printf("%s \n", &(errorLog[0]));
			fatalError("Shaders failed to link!");
		}

		glDetachShader(m_programID, m_vertShaderID);
		glDetachShader(m_programID, m_fragShaderID);
		glDeleteShader(m_vertShaderID);
		glDeleteShader(m_fragShaderID);
	}

	void GLSLProgram::addAttribute(const std::string & attributeName)
	{
		glBindAttribLocation(m_programID, m_numAttributes++, attributeName.c_str());
	}

	void GLSLProgram::use()
	{
		glUseProgram(m_programID);

		for (auto i = 0; i < m_numAttributes; ++i)
		{
			glEnableVertexAttribArray(i);
		}
	}

	void GLSLProgram::unuse()
	{
		glUseProgram(0);

		for (auto i = 0; i < m_numAttributes; ++i)
		{
			glDisableVertexAttribArray(i);
		}
	}

	GLint GLSLProgram::getUniformLocation(const std::string & uniformName)
	{
		GLint location = glGetUniformLocation(m_programID, uniformName.c_str());
		if (location == GL_INVALID_INDEX)
		{
			fatalError("Uniform " + uniformName + " not found in shader!");
		}
		return location;
	}

	void GLSLProgram::compileShader(const std::string& filePath, GLuint id)
	{
		std::ifstream vertexFile(filePath);
		if (vertexFile.fail())
		{
			perror(filePath.c_str());
			fatalError("Failed to open" + filePath + "!");
		}

		std::string fileContents = "";
		std::string line;
		while (std::getline(vertexFile, line))
		{
			fileContents += line + "\n";
		}
		vertexFile.close();
		const char* p_fileConents = fileContents.c_str();
		glShaderSource(id, 1, &p_fileConents, nullptr);
		glCompileShader(id);

		GLint success = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<char> errorLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);
			glDeleteShader(id);
			std::printf("%s\n", &(errorLog[0]));
			fatalError("Shader " + filePath + " failed to compile");
		}
	}
}