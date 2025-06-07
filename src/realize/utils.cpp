#include <HEADER/utils.hpp>
#include <HEADER/process_events.hpp>
#include <iostream>
#include <stb_image/stb_image.h>

GLFWwindow* initAndCreateWindow(int width, int height, const char* title) {
	// 初始化函数，用于加载库并设置内部状态。
	glfwInit();
	// 设置 OpenGL 版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// 使用核心模式（只是用OpenGL功能的子集，不需要向后兼容的特性）
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);	// 需要向前兼容模式

	// 创建窗口
	GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "窗口创建失败" << std::endl;
		return nullptr;
	}

	// 设置当前上下文
	glfwMakeContextCurrent(window);
	// 隐藏鼠标并且将其捕获在窗口中心
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// 初始化 GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "初始化GLAD失败" << std::endl;
		return nullptr;
	}

	// glad初始化成功后，OpenGL函数指针会被加载到当前上下文中，这样就可以使用OpenGL函数了。
	// 设置视口
	glViewport(0, 0, width, height);
	return window;
}

void jobBeforeEnterRenderLoop(GLFWwindow* window) {
	glEnable(GL_DEPTH_TEST);
}

void jobAtRenderLoopStart(GLFWwindow* window) {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 先调用glfwPollEvents()，处理所有的事件（比如键盘、鼠标、窗口大小变化等）
	glfwPollEvents();
	// 然后调用processEscapeKey()函数检查ESCAPE键是否按下。
	processEscapeKey(window);
}

void jobBeforRenderLoopEnd(GLFWwindow* window) {
	glfwSwapBuffers(window);
}

unsigned int createShader(const std::string shaderType, const char* shaderCode) {
	unsigned int shaderID;

	if (shaderType == "VERTEX" || shaderType == "vertex") {
		shaderID = glCreateShader(GL_VERTEX_SHADER);
	}
	else if (shaderType == "FRAGMENT" || shaderType == "fragment") {
		shaderID = glCreateShader(GL_FRAGMENT_SHADER);
	}

	glShaderSource(shaderID, 1, &shaderCode, NULL);

	glCompileShader(shaderID);

	int isSuccess;
	char infoLog[512];
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isSuccess);

	if (!isSuccess) {
		glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
		std::cout << shaderType << "  type shader compile failed, log is:\n" << infoLog << std::endl;
	}
	else {
		std::cout << shaderType << " type shader compile success, shaderID is: " << shaderID << std::endl;
	}

	return shaderID;
}

unsigned int createShaderProgram(unsigned int vertexShaderID, unsigned int fragmentShaderID) {
	unsigned int shaderProgramID = glCreateProgram();

	glAttachShader(shaderProgramID, vertexShaderID);
	// 前一个着色器的输出,必须与下一个着色器的输入相匹配
	glAttachShader(shaderProgramID, fragmentShaderID);
	// 链接着色器
	glLinkProgram(shaderProgramID);

	int isSuccess;
	char infoLog[512];
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &isSuccess);

	if (!isSuccess) {
		glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
		std::cout << "shader program create failed, info is:\n" << infoLog << std::endl;
	}
	else {
		std::cout << "shader program create sucess, programID is: " << shaderProgramID << std::endl;
	}

	// 链接完成后就可以删除着色器了
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return shaderProgramID;
}

VAOData set_VAO_VBO_EBO(
	float* vertices,
	unsigned verticesSize,
	unsigned int* indices,
	unsigned int indicesSize,
	unsigned int locationIndex,
	unsigned int dimension,
	unsigned int stride,
	unsigned int offset
) {
	unsigned int VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO); // 顶点数组对象VAO
	glGenBuffers(1, &VBO); // 顶点缓冲对象
	glGenBuffers(1, &EBO); // 元素缓冲对象

	glBindVertexArray(VAO);

	if (vertices != nullptr && verticesSize > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW); 	// 复制顶点数据

		glVertexAttribPointer(locationIndex, dimension, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
		glEnableVertexAttribArray(locationIndex);
	}

	if (indices != nullptr && indicesSize > 0) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);
	}

	// 解绑
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return { VAO, VBO, EBO };
}

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
) {
	unsigned int VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO); // 顶点数组对象VAO
	glGenBuffers(1, &VBO); // 顶点缓冲对象
	glGenBuffers(1, &EBO); // 元素缓冲对象

	glBindVertexArray(VAO);

	if (vertices != nullptr && verticesSize > 0) {

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW); 	// 复制顶点数据

		for (unsigned int i = 0; i < count; ++i) {

			glVertexAttribPointer(locationIndex[i], dimension[i], GL_FLOAT, GL_FALSE, stride[i] * sizeof(float), (void*)(offset[i] * sizeof(float)));
			glEnableVertexAttribArray(locationIndex[i]);

		}
	}

	if (indices != nullptr && indicesSize > 0) {

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

	}

	// 解绑
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return { VAO, VBO, EBO };
}

unsigned int createTexture(
	const char* imagePath,
	unsigned int wrapS,
	unsigned int wrapT,
	unsigned int magFilter,
	unsigned int minFilter,
	bool alpha,
	bool flip
) {
	unsigned int texture;
	// 创建纹理对象
	glGenTextures(1, &texture);
	// 绑定到2d纹理, 之后对2D纹理所作的操作都会影响这个对象
	glBindTexture(GL_TEXTURE_2D, texture);

	// 设置纹理的重复方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);

	// 设置过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);

	// 加载图片, 创建纹理图像  生成多级渐远纹理
	int width, height, nrChannels;
	// 设置翻转图片
	stbi_set_flip_vertically_on_load(flip);
	unsigned char* imageData = stbi_load(imagePath, &width, &height, &nrChannels, 0);
	if (imageData) {
		// 根据图片生成纹理图像
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, imageData);
		// 生成多级渐远纹理
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "image load failed " << std::endl;
	}
	// 释放资源
	stbi_image_free(imageData);
	return texture;
}

unsigned int createTexture(const char* imagePath, unsigned int wrapS, unsigned int wrapT, unsigned int magFilter, unsigned int minFilter, bool flip)
{
	unsigned int texture;
	// 创建纹理对象
	glGenTextures(1, &texture);
	// 绑定到2d纹理, 之后对2D纹理所作的操作都会影响这个对象
	glBindTexture(GL_TEXTURE_2D, texture);

	// 设置纹理的重复方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);

	// 设置过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);

	// 加载图片, 创建纹理图像  生成多级渐远纹理
	int width, height, nrChannels;
	// 设置翻转图片
	stbi_set_flip_vertically_on_load(flip);
	unsigned char* imageData = stbi_load(imagePath, &width, &height, &nrChannels, 0);
	if (imageData) {
		// 根据图片生成纹理图像
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
		// 生成多级渐远纹理
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "image load failed " << std::endl;
	}
	// 释放资源
	stbi_image_free(imageData);
	return texture;
}

void printMat4(const glm::mat4& mat) {
	std::cout << "=================================================\n";
	std::cout << mat[0][0] << " " << mat[0][1] << " " << mat[0][2] << " " << mat[0][3] << std::endl;
	std::cout << mat[1][0] << " " << mat[1][1] << " " << mat[1][2] << " " << mat[1][3] << std::endl;
	std::cout << mat[2][0] << " " << mat[2][1] << " " << mat[2][2] << " " << mat[2][3] << std::endl;
	std::cout << mat[3][0] << " " << mat[3][1] << " " << mat[3][2] << " " << mat[3][3] << std::endl;
	std::cout << "=================================================\n";
}
