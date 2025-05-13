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
    unsigned int ID; // ��ɫ������� ID

    // ���캯�����Ӷ�����ɫ����Ƭ����ɫ���ļ�·��������ɫ������
    Shader(const char* vertexPath, const char* fragmentPath);

    // ʹ����ɫ������
    void use();

    // ���� uniform ������ֵ
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void set3Float(const std::string& name, float v1, float v2, float v3) const;
    void set1Vec3(const std::string& name, const glm::vec3& vec) const;
    void set4Float(const std::string& name, float v1, float v2, float v3, float v4) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;

private:
	std::string vertexCode = ""; // ������ɫ������
	std::string fragmentCode = ""; // Ƭ����ɫ������

	void getShaderCodeFromFile(const char* vertexPath, const char* fragmentPath);

    // ����������Ӵ���
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif // SHADER_CLASS_H