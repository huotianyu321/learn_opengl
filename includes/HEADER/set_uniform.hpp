#ifndef SET_UNIFORM_H
#define SET_UNIFORM_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
//#include <GLFW/glfw3.h>

/*
* 查询uniform变量的位置, 查询不用激活着色器程序
*/
int findUniform(unsigned int shaderProgramID, const std::string& uniformName);


/*
* 设置各种类型uniform变量的值
* 在设置uniform变量之前，必须先使用glUseProgram()函数激活着色器程序
*/
void set3float(
	unsigned int shaderProgramID,
	const std::string& uniformName,
	float v1,
	float v2,
	float v3
);

void set4float(
	unsigned int shaderProgramID,
	const std::string& uniformName,
	float v1,
	float v2,
	float v3,
	float v4
);

void set1float(
	unsigned int shaderProgramID,
	const std::string& uniformName,
	float v1
);

void set1int(
	unsigned int shaderProgramID,
	const std::string& uniformName,
	int i
);

void set1bool(
	unsigned int shaderProgramID,
	const std::string& uniformName,
	bool b
);

void set1mat4(
	unsigned int shaderProgramID,
	const std::string& uniformName,
	const glm::mat4& mat4
);

#endif // !SET_UNIFOTM_H


