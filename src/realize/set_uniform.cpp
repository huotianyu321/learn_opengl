#include <HEADER/set_uniform.hpp>
#include <iostream>

void set4float(
	unsigned int shaderProgramID,
	const std::string& uniformName,
	float v1,
	float v2,
	float v3,
	float v4
) {
	int location = glGetUniformLocation(shaderProgramID, uniformName.c_str());
	if (location == -1) {
		std::cout << "uniform: " << uniformName << " not found" << std::endl;
		return;
	}
	// ������uniform����֮ǰ��������ʹ��glUseProgram()����������ɫ������
	glUseProgram(shaderProgramID);
	glUniform4f(location, v1, v2, v3, v4);
}

void set1float(unsigned int shaderProgramID, const std::string& uniformName, float v1)
{
	int location = glGetUniformLocation(shaderProgramID, uniformName.c_str());
	if (location == -1) {
		std::cout << "uniform: " << uniformName << " not found" << std::endl;
		return;
	}

	glUseProgram(shaderProgramID);
	glUniform1f(location, v1);
}

void set1int(unsigned int shaderProgramID, const std::string& uniformName, int i) {
	int location = glGetUniformLocation(shaderProgramID, uniformName.c_str());
	if (location == -1) {
		std::cout << "uniform: " << uniformName << " not found" << std::endl;
		return;
	}
	glUseProgram(shaderProgramID);
	glUniform1i(location, i);
}
