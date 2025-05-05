#ifndef UTILS_H
#define UTILS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

/*
*初始化、设置 OpenGL 版本和核心模式
*创建窗口
*设置当前上下文
*初始化 GLAD
*设置视口
*/
GLFWwindow* initAndCreateWindow(int width, int height, const char* title);

/*
* 在渲染循环开始时调用的函数
* 设置清屏颜色
* 设置要清除的缓冲区
* 处理所有的事件（比如键盘、鼠标、窗口大小变化等）
* 检查ESCAPE键是否按下
*/
void jobAtRenderLoopStart(GLFWwindow* window);

/*
* 在渲染循环结束时调用的函数
* 交换前后缓冲区
*/
void jobBeforRenderLoopEnd(GLFWwindow* window);

/*
* 创建着色器
* 绑定着色器代码
* 编译着色器
* 校验是否成功
*/
unsigned int createShader(const std::string shaderType, const char* shaderCode);

/*
* 创建着色器程序
* 添加着色器
* 链接着色器程序
* 校验
*/
unsigned int createShaderProgram(unsigned int vertexShaderID, unsigned int fragmentShaderID);

/*
* vertices 顶点数据
* verticesSize 顶点数据数组的大小
* indices 顶点索引数据
* indicesSize 顶点索引数据数组的大小
* locationIndex 与顶点着色器中的Location对应
* dimension 顶点数据的维度（有几个分量)
* stride 顶点数据在数组中的跨度
* offset 顶点数据在数组中的起始偏移量
*/
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