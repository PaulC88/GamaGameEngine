#pragma once

#include <string>
#include <GL/glew.h>

namespace GamaGameEngine {

	class GLSLProgram {

	public:
		GLSLProgram();
		~GLSLProgram();

		void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

		void linkShaders();

		void addAttribute(const std::string& attributeName);

		GLint getUniformLocation(const std::string& uniformName);

		void use();
		void unuse();

	private:
		GLuint m_programID;
		GLuint m_vertexShaderID;
		GLuint m_fragmentShaderID;
		int m_numAttributes;

		void compileShader(const std::string& filePath, GLuint id);
	};
}