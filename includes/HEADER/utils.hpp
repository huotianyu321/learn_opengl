#ifndef UTILS_H
#define UTILS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

struct VAOData {
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
};

/*
* 该文件提供与opengl相关的一些内容的封装函数
*/

/*
* 初始化、设置 OpenGL 版本和核心模式
* 创建窗口, 设置当前上下文, 初始化 GLAD, 设置视口
* @param width 窗口宽度
* @param height 窗口高度
* @param title 窗口标题
* @return 窗口指针
*/
GLFWwindow* initAndCreateWindow(int width, int height, const char* title);

/*
* 在进入渲染循环前做些什么
*/
void jobBeforeEnterRenderLoop(GLFWwindow* window);

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
* 创建着色器, 绑定着色器代码
* 编译着色器, 校验是否成功
* @param shaderType 着色器类型(vertex或fragment)
* @param shaderCode 着色器代码
* @return 着色器ID
*/
unsigned int createShader(const std::string shaderType, const char* shaderCode);

/*
* 创建着色器程序, 添加着色器
* 链接着色器程序, 校验
* @param vertexShaderID 顶点着色器ID
* @param fragmentShaderID 片段着色器ID
* @return 着色器程序ID
*/
unsigned int createShaderProgram(unsigned int vertexShaderID, unsigned int fragmentShaderID);

/*
* 创建VAO, VBO, EBO
* 绑定顶点数据，设置一个顶点属性
* 绑定索引数据
* 解绑VAO, VBO, EBO
* @param vertices 顶点数据
* @param verticesSize 顶点数据数组的大小
* @param indices 顶点索引数据
* @param indicesSize 顶点索引数据数组的大小
* @param locationIndex 与顶点着色器中的Location对应
* @param dimension 顶点数据的维度（有几个分量)
* @param stride 顶点数据在数组中的跨度
* @param offset 顶点数据在数组中的起始偏移量
* @return VAO id
*/

VAOData set_VAO_VBO_EBO(
	float* vertices,
	unsigned verticesSize,
	unsigned int* indices,
	unsigned int indicesSize,
	unsigned int locationIndex,
	unsigned int dimension,
	unsigned int stride,
	unsigned int offset
);

/*
* 与上一个方法的区别：设置一组顶点属性
* @param vertices 顶点数据
* @param verticesSize 顶点数据数组的大小
* @param indices 顶点索引数据
* @param indicesSize 顶点索引数据数组的大小
* @param count 顶点属性的数量
* @param locationIndex 顶点着色器中的Location数组
* @param dimension 维度数组，每个元素表示顶点属性的维度
* @param stride 跨度数组，每个元素表示顶点属性在数组中的跨度
* @param offset 偏移量数组，每个元素表示顶点属性在数组中的起始偏移量
* @return VAO id
*/
VAOData set_VAO_VBO_EBO_mutiple(
	float* vertices,
	unsigned verticesSize,
	unsigned int* indices,
	unsigned int indicesSize,
	unsigned int count,
	unsigned int locationIndex[],
	unsigned int dimension[],
	unsigned int stride[],
	unsigned int offset[]
);

/*
* 1.加载图片 2.创建纹理
* @param imagePath图片文件路径
* @param wrapS 纹理水平环绕方式
* @param wrapT 纹理垂直环绕方式
* @param magFilter 纹理放大过滤方式
* @param minFilter 纹理缩小过滤方式
* @param alpha 是否有透明通道
* @param flip 是否翻转图片
*/
unsigned int createTexture(
	const char* imagePath, 
	unsigned int wrapS, 
	unsigned int wrapT, 
	unsigned int magFilter, 
	unsigned int minFilter,
	bool alpha,
	bool flip
);

/*
* 1.加载图片 2.创建纹理 - v2版本
* @param imagePath图片文件路径
* @param wrapS 纹理水平环绕方式
* @param wrapT 纹理垂直环绕方式
* @param magFilter 纹理放大过滤方式
* @param minFilter 纹理缩小过滤方式
* @param flip 是否翻转图片
*/
unsigned int createTexture(
	const char* imagePath,
	unsigned int wrapS,
	unsigned int wrapT,
	unsigned int magFilter,
	unsigned int minFilter,
	bool flip
);

/*
* 打印矩阵
*/
void printMat4(const glm::mat4& mat);

#endif // UTILS_H