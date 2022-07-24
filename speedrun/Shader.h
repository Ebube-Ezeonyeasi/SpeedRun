#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>


class Shader
{
private:
	std::string m_filePath;
	unsigned int m_ShaderId;
	std::unordered_map<std::string, int> m_uniformLocations;

public:
	Shader(const std::string& filePath);
	std::pair <std::string, std::string> ParseShader(const std::string& filepath);
	unsigned int CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CompileShader(const unsigned int& type, const std::string& shader);

	void SetTexture(const std::string& name, const int& value);
	void SetMatrix4(const std::string& name, int no_of_matricies, unsigned int transpose, const float* matrixData);
	void SetMatrix3(const std::string& name, int no_of_matricies, unsigned int transpose, const float* matrixData);
	void SetVec3p(const std::string& name, int number, const float* value);
	void SetVec3f(const std::string& name, float one, float two, float three);
	void SetFloat(const std::string& name, float value);
	int GetUniformLocation(std::string name);

	void Bind();
	void UnBind();

	~Shader();
};

