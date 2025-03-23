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

	// 参数是着色器代码文件路径
	Shader(const char* vertexPath, const char* fragmentPath);

	void use(); 
	// 一些工具函数用于设置uniform
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void set4Float(const std::string& name, float v1, float v2, float v3, float v4) const;


private:
	// 封装检查
	void checkCompileErrors(unsigned int shader, std::string type);
};