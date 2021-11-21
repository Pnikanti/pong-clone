#include <sstream>
#include <fstream>
#include <glew/glew.h>
#include "log.h"
#include "shader.h"

ShaderHandler::ShaderProgramSource ShaderHandler::Parse(const std::string& vertexFile, const std::string& fragmentFile)
{
	std::ifstream vertexStream(vertexFile);
	std::ifstream fragmentStream(fragmentFile);

	std::string line;
	std::stringstream stringStream[2];

	while (getline(vertexStream, line))
	{
		stringStream[(int)ShaderType::VERTEX] << line << '\n';
	}
	while (getline(fragmentStream, line))
	{
		stringStream[(int)ShaderType::FRAGMENT] << line << '\n';
	}

	return { stringStream[0].str(), stringStream[1].str() };
}

unsigned int ShaderHandler::Compile(unsigned int type, const std::string& source)
{
	unsigned int shader = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	int result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(shader, length, &length, message);
		LOGGER_WARN("Failed to compile {0} shader: {1}", type == GL_VERTEX_SHADER ? "vertex" : "fragment", message);
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}

unsigned int ShaderHandler::Create(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = Compile(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = Compile(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	int result;

	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetProgramInfoLog(program, length, &length, message);
		LOGGER_WARN("Failed to link shader program: {0}", message);
	}

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}