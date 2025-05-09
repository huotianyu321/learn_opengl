#ifndef SET_UNIFORM_H
#define SET_UNIFORM_H

#include <glad/glad.h>
#include <string>
//#include <GLFW/glfw3.h>

/*
* 查询uniform变量的位置
* 激活着色器程序
* 设置uniform变量的值
*/
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

#endif // !SET_UNIFOTM_H


