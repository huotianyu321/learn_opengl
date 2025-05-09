#ifndef SET_UNIFORM_H
#define SET_UNIFORM_H

#include <glad/glad.h>
#include <string>
//#include <GLFW/glfw3.h>

/*
* ��ѯuniform������λ��
* ������ɫ������
* ����uniform������ֵ
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


