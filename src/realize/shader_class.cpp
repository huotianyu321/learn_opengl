// ����ѵ�һ�µ����ڵ���ɫ����װ�࿽����һ��

#include <HEADER/shader_class.hpp>
#include <HEADER/utils.hpp>
#include <HEADER/set_uniform.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

// ���캯��ʵ��
Shader::Shader(const char* vertexPath, const char* fragmentPath) {

	this->getShaderCodeFromFile(vertexPath, fragmentPath); // ��ȡ��ɫ������
	std::cout << "=============vertexCode: ===============\n" << this->vertexCode << std::endl;
	std::cout << "=============fragmentCode: =============\n" << this->fragmentCode << std::endl;

    const char* vertexCodeStr = this->vertexCode.c_str();
    const char* fragmentCodeStr = this->fragmentCode.c_str();

	unsigned vertexShaderID = createShader("VERTEX", vertexCodeStr);
	unsigned fragmentShaderID = createShader("FRAGMENT", fragmentCodeStr);
    this->ID = createShaderProgram(vertexShaderID, fragmentShaderID);
}

void Shader::getShaderCodeFromFile(const char* vertexPath, const char* fragmentPath)
{
    // 1. ���ļ���ȡ��ɫ������
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // �����ļ�������Щ�����Ҫ�׳��쳣
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        // ���ļ�
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        // ���ļ������������ݶ�������
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // �ر��ļ�
        vShaderFile.close();
        fShaderFile.close();

        // ���ַ���ת��Ϊ�ַ���
        this->vertexCode = vShaderStream.str();
        this->fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e) {
        std::cout << "shader code file read failed: " << e.what() << std::endl;
    }
}

// ʹ����ɫ������
void Shader::use() {
    glUseProgram(this->ID);
}

// ���� uniform ����
void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
	set1float(ID, name, value);
}

void Shader::set4Float(const std::string& name, float v1, float v2, float v3, float v4) const {
	set4float(this->ID, name, v1, v2, v3, v4);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

// ����������Ӵ���
void Shader::checkCompileErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "��ɫ������ʧ��, ���ͣ�" << type << "\n" << infoLog << "\n ------------------------------------" << std::endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "��ɫ����������ʧ��, ���ͣ�" << type << "\n" << infoLog << "\n ------------------------------------" << std::endl;
        }
    }
}