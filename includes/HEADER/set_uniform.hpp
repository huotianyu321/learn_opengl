#ifndef SET_UNIFORM_H
#define SET_UNIFORM_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
//#include <GLFW/glfw3.h>

/*
* ��ѯuniform������λ��, ��ѯ���ü�����ɫ������
*/
int findUniform(unsigned int shaderProgramID, const std::string& uniformName);


/*
* ���ø�������uniform������ֵ
* ������uniform����֮ǰ��������ʹ��glUseProgram()����������ɫ������
*/

/*
* ����vec3���ͣ���������������
*/
void set3float(
	unsigned int shaderProgramID,
	const std::string& uniformName,
	float v1,
	float v2,
	float v3
);

/*
* ����vec3���ͣ�����һ��vec3����
*/
void set1vec3(
	unsigned int shaderProgramID,
	const std::string& uniformName,
	const glm::vec3& vec3
);

/*
* ����vec4���ͣ������ĸ�������
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

void set1bool(
	unsigned int shaderProgramID,
	const std::string& uniformName,
	bool b
);

/*
* ����mat4���ͣ�����һ��mat4����
*/
void set1mat4(
	unsigned int shaderProgramID,
	const std::string& uniformName,
	const glm::mat4& mat4
);

#endif // !SET_UNIFOTM_H


