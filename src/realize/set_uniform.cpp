#include <HEADER/set_uniform.hpp>
#include <iostream>

int findUniform(unsigned int shaderProgramID, const std::string& uniformName) {
	return glGetUniformLocation(shaderProgramID, uniformName.c_str());
}

void set3float(
	unsigned int shaderProgramID,
	const std::string& uniformName,
	float v1,
	float v2,
	float v3
) {
	int location = findUniform(shaderProgramID, uniformName);
	if (location == -1) {
		//std::cout << "uniform: " << uniformName << " not found" << std::endl;
		return;
	}

	glUseProgram(shaderProgramID);
	glUniform3f(location, v1, v2, v3);
}

void set1vec3(unsigned int shaderProgramID, const std::string& uniformName, const glm::vec3& vec3) {
	int location = findUniform(shaderProgramID, uniformName);
	if (location == -1) {
		//std::cout << "uniform: " << uniformName << " not found" << std::endl;
		return;
	}

	glUseProgram(shaderProgramID);
	glUniform3fv(location, 1, glm::value_ptr(vec3));
}


void set4float(
	unsigned int shaderProgramID,
	const std::string& uniformName,
	float v1,
	float v2,
	float v3,
	float v4
) {
	int location = findUniform(shaderProgramID, uniformName);
	if (location == -1) {
		//std::cout << "uniform: " << uniformName << " not found" << std::endl;
		return;
	}

	glUseProgram(shaderProgramID);
	glUniform4f(location, v1, v2, v3, v4);
}

void set1float(unsigned int shaderProgramID, const std::string& uniformName, float v1)
{
	int location = findUniform(shaderProgramID, uniformName);
	if (location == -1) {
		//std::cout << "uniform: " << uniformName << " not found" << std::endl;
		return;
	}

	glUseProgram(shaderProgramID);
	glUniform1f(location, v1);
}

void set1int(unsigned int shaderProgramID, const std::string& uniformName, int i) {
	int location = findUniform(shaderProgramID, uniformName);
	if (location == -1) {
		//std::cout << "uniform: " << uniformName << " not found" << std::endl;
		return;
	}

	glUseProgram(shaderProgramID);
	glUniform1i(location, i);
}

void set1bool(unsigned int shaderProgramID, const std::string& uniformName, bool value) {
	set1int(shaderProgramID, uniformName, value ? 1 : 0);
}


void set1mat4(unsigned int shaderProgramID, const std::string& uniformName, const glm::mat4& mat4)
{
	int location = findUniform(shaderProgramID, uniformName);
	if (location == -1) {
		//std::cout << "uniform: " << uniformName << " not found" << std::endl;
		return;
	}

	glUseProgram(shaderProgramID);
	// 数量1, 不转置, 转换格式
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat4));
}