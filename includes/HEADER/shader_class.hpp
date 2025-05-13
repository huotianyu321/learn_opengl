#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

class Shader {
public:
    unsigned int ID; // 着色器程序的 ID

    // 构造函数：从顶点着色器和片段着色器文件路径创建着色器程序
    Shader(const char* vertexPath, const char* fragmentPath);

    // 使用着色器程序
    void use();

    // 设置 uniform 变量的值
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void set3Float(const std::string& name, float v1, float v2, float v3) const;
    void set1Vec3(const std::string& name, const glm::vec3& vec) const;
    void set4Float(const std::string& name, float v1, float v2, float v3, float v4) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;

private:
	std::string vertexCode = ""; // 顶点着色器代码
	std::string fragmentCode = ""; // 片段着色器代码

	void getShaderCodeFromFile(const char* vertexPath, const char* fragmentPath);

    // 检查编译或链接错误
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif // SHADER_CLASS_H