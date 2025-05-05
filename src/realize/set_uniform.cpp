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