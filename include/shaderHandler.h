#pragma once

#include <string>

namespace ShaderHandler {
	enum class ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	struct ShaderProgramSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};

	ShaderProgramSource Parse(const std::string& vertexFile, const std::string& fragmentFile);
	unsigned int Compile(unsigned int type, const std::string& source);
	unsigned int Create(const std::string& vertexShader, const std::string& fragmentShader);
}