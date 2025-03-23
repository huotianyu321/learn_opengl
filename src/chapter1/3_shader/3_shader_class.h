#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#endif

class Shader {
public:
	unsigned int ID;

	// ��������ɫ�������ļ�·��
	Shader(const char* vertexPath, const char* fragmentPath);

	void use(); 
	// һЩ���ߺ�����������uniform
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void set4Float(const std::string& name, float v1, float v2, float v3, float v4) const;


private:
	// ��װ���
	void checkCompileErrors(unsigned int shader, std::string type);
};