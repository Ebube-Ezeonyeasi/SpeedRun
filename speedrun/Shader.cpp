#include "includes.h"
#include "Shader.h"


Shader::Shader(const std::string& filePath)
	:m_filePath(filePath), m_ShaderId(0)
{
	std::pair<std::string, std::string> pair = ParseShader(m_filePath);

	std::string vertexShader = pair.first;
	std::string fragmentShader = pair.second;

	std::cout << vertexShader << " \n\n\n\n\n" << fragmentShader;
	
	m_ShaderId = CreateShaderProgram(vertexShader, fragmentShader);

}



std::pair <std::string, std::string> Shader::ParseShader(const std::string& filepath) 
{

	std::ifstream shaderFile(filepath.c_str());
	std::string line;
	std::stringstream shaderText[2];

	enum class ShaderType {
		 NONE = -1, VERTEX = 0, FRAGMENT = 1
};

	ShaderType type = ShaderType::NONE;

	while (std::getline(shaderFile, line)) {
  
		
		if (line.find("#shader vertex") != std::string::npos)
		{
			type = ShaderType::VERTEX;
		}
		else if (line.find("#shader fragment") != std::string::npos)
		{
			type = ShaderType::FRAGMENT;
		}
		else
		{
			shaderText[(int)type] << line << "\n";
		}
		

	}

	std::pair < std::string, std::string> returnVal = std::make_pair(shaderText[0].str(), shaderText[1].str());

	return returnVal;
}

unsigned int Shader::CompileShader(const unsigned int& type, const std::string& shader)
{

	unsigned int id = glCreateShader(type);
	const char* sourceCode = shader.c_str();

	glShaderSource(id, 1, &sourceCode, nullptr);
	glCompileShader(id);

	int compileStatus;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus);

	if (compileStatus == GL_FALSE) {

		int infoLength;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLength);
		char* info = (char*)alloca(infoLength * sizeof(char));
		glGetShaderInfoLog(id, infoLength, &infoLength, info);

		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader...sos\n";

		std::cout << info << "\n";

		glDeleteShader(id);

	}
	return id;

}

unsigned int Shader::CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader) 
{

	unsigned int program = glCreateProgram();

	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);


	report(glAttachShader(program, vs));
	report(glAttachShader(program, fs));

	report(glLinkProgram(program));

	int linkageStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkageStatus);

	if (linkageStatus == GL_FALSE) {
			
		int infoLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLength);
		char* info = (char*)alloca(infoLength * sizeof(char));
		glGetProgramInfoLog(program, infoLength, &infoLength, info);

		std::cout << "Failed to link shaders...sos \n" << info << " \n";

	}



	report(glValidateProgram(program));

	report(glDeleteShader(vs));
	report(glDeleteShader(fs));

	return program;
}

void Shader::Bind() 
{


	report(glUseProgram(m_ShaderId));

}

void Shader::UnBind() 
{


	report(glUseProgram(0));

}

Shader::~Shader() 
{


	report(glDeleteProgram(m_ShaderId));

}

int Shader::GetUniformLocation(std::string name) 
{

	if (m_uniformLocations.find(name) != m_uniformLocations.end())
		return m_uniformLocations[name];
	
	int location = glGetUniformLocation(m_ShaderId, name.c_str());
	m_uniformLocations[name] = location;

	return location;

}

void Shader::SetTexture(const std::string& name, const int& value) 
{
	
	report(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetMatrix4(const std::string& name, int no_of_matricies, unsigned int transpose, const float* matrixData) 
{

	report(glUniformMatrix4fv(GetUniformLocation(name), no_of_matricies, transpose, matrixData));

}

void Shader::SetMatrix3(const std::string& name, int no_of_matricies, unsigned int transpose, const float* matrixData)
{

	report(glUniformMatrix3fv(GetUniformLocation(name), no_of_matricies, transpose, matrixData));

}

void Shader::SetVec3p(const std::string& name, int number, const float* value)
{
	report(glUniform3fv(GetUniformLocation(name), number, value));
}

void Shader::SetVec3f(const std::string& name, float one, float two, float three)
{
	report(glUniform3f(GetUniformLocation(name), one, two, three));
}

void Shader::SetFloat(const std::string& name, float value)
{
	report(glUniform1f(GetUniformLocation(name), value));
}