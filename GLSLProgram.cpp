#include "GLSLProgram.h"
#include "LogAndDebug.h"

#include <fstream>
#include <vector>

namespace GamaGameEngine {

	GLSLProgram::GLSLProgram() : m_programID(0), m_vertexShaderID(0), m_fragmentShaderID(0), m_numAttributes(0)	{
		//Empty
	}

	GLSLProgram::~GLSLProgram()	{
		//Empty
	}

	//Compiles shader files for GPU 
	void GLSLProgram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {

		m_programID = glCreateProgram();

		m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (m_vertexShaderID == 0) 
			fatalError("Vertex shader failed to be created!");

		m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (m_fragmentShaderID == 0)
			fatalError("Fragment shader failed to be created!");

		compileShader(vertexShaderFilePath, m_vertexShaderID);
		compileShader(fragmentShaderFilePath, m_fragmentShaderID);
	}

	//Link shader files to program
	void GLSLProgram::linkShaders() {

		glAttachShader(m_programID, m_vertexShaderID);
		glAttachShader(m_programID, m_fragmentShaderID);

		glLinkProgram(m_programID);

		GLint isLinked = 0;
		glGetProgramiv(m_programID, GL_LINK_STATUS, (int *)&isLinked);
	
		if (isLinked == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> errorLog(maxLength);
			glGetProgramInfoLog(m_programID, maxLength, &maxLength, &errorLog[0]);

			glDeleteProgram(m_programID);
			glDeleteShader(m_vertexShaderID);
			glDeleteShader(m_fragmentShaderID);

			fatalError("Failure linking Shader files to program");
		}

		glDetachShader(m_programID, m_vertexShaderID);
		glDetachShader(m_programID, m_fragmentShaderID);
		glDeleteShader(m_vertexShaderID);
		glDeleteShader(m_fragmentShaderID);
	}

	//Add an attibute to shader
	void GLSLProgram::addAttribute(const std::string& attributeName) {
		glBindAttribLocation(m_programID, m_numAttributes++, attributeName.c_str());
	}

	//Return Shader uniform location
	GLint GLSLProgram::getUniformLocation(const std::string& uniformName) {
		GLint location = glGetUniformLocation(m_programID, uniformName.c_str());
		
		if (location == GL_INVALID_INDEX) 
			fatalError("Uniform " + uniformName + " not found in shader!");
		
		return location;
	}

	//Use the shader
	void GLSLProgram::use() {
		glUseProgram(m_programID);
		for (int i = 0; i < m_numAttributes; i++) {
			glEnableVertexAttribArray(i);
		}
	}

	//Disable the shader
	void GLSLProgram::unuse() {
		glUseProgram(0);
	
		for (int i = 0; i < m_numAttributes; i++) 
			glDisableVertexAttribArray(i);
		
	}

	//Compiles a single shader file
	void GLSLProgram::compileShader(const std::string& filePath, GLuint id) {

		std::ifstream shaderFile(filePath);

		if (shaderFile.fail()) {
			fatalError("Failed to open " + filePath);
		}

		std::string fileContents = "";
		std::string line;

		while (std::getline(shaderFile, line)) 
			fileContents += line + "\n";

		shaderFile.close();

		const char* contentsPtr = fileContents.c_str();

		glShaderSource(id, 1, &contentsPtr, nullptr);
		glCompileShader(id);

		GLint success = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> errorLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

			glDeleteShader(id);

			fatalError("Shader failed to compile" + filePath);
		}
	}
}