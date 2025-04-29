#ifndef UTILS_H
#define UTILS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

// 宽度、高度、标题
GLFWwindow* initAndCreateWindow(int width, int height, const char* title);

// 在渲染循环开始时调用的函数
void jobAtRenderLoopStart(GLFWwindow* window);

// 在渲染循环结束前调用
void jobBeforRenderLoopEnd(GLFWwindow* window);

// 创建着色器
unsigned int createShader(const std::string shaderType, const char* shaderCode);

// 创建着色器程序
unsigned int createShaderProgram(unsigned int vertexShaderID, unsigned int fragmentShaderID);

unsigned int set_VAO_VBO_EBO(
	float* vertices,
	unsigned verticesSize,
	unsigned int* indices,
	unsigned int indicesSize,
	unsigned int locationIndex,
	unsigned int dimension,
	unsigned int stride,
	unsigned int offset
);

#endif // UTILS_H