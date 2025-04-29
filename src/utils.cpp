#include <HEADER/utils.hpp>
#include <HEADER/call_backs.hpp>
#include <iostream>

/*
*glfwInit() 是 GLFW 的初始化函数，用于加载库并设置内部状态。
*
*设置 OpenGL 版本和核心模式
*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
*	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
*	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
*
*创建窗口
*	glfwCreateWindow(width, height, title, nullptr, nullptr);
*
*设置当前上下文
*	glfwMakeContextCurrent(window);  这行代码将当前窗口的 OpenGL 上下文绑定到当前线程，以便后续的 OpenGL 调用可以作用于该上下文。
*
*初始化 GLAD
*	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
*	GLAD 是一个 OpenGL 函数加载器，用于动态加载 OpenGL 函数指针。如果初始化失败，代码会输出错误信息并返回 nullptr。
*
*设置视口
*	glViewport(0, 0, width, height);
*/
GLFWwindow* initAndCreateWindow(int width, int height, const char* title) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 	// 使用核心模式（只是用OpenGL功能的子集，不需要向后兼容的特性）
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);	// 需要向前兼容模式

	GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "窗口创建失败" << std::endl;
		return nullptr;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "初始化GLAD失败" << std::endl;
		return nullptr;
	}

	glViewport(0, 0, width, height);
	return window;
}

/*
* 先调用glfwPollEvents()，处理所有的事件（比如键盘、鼠标、窗口大小变化等）
* 然后调用processEscapeKey()函数检查ESCAPE键是否按下。
*/
void jobAtRenderLoopStart(GLFWwindow* window) {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwPollEvents();
	processEscapeKey(window);
}

void jobBeforRenderLoopEnd(GLFWwindow* window) {
	glfwSwapBuffers(window);
}

/*
* 创建着色器
* 绑定着色器代码
* 编译着色器
* 校验是否成功
*/
unsigned int createShader(const std::string shaderType, const char* shaderCode) {
	unsigned int shaderID;

	if (shaderType == "VERTEX") {
		shaderID = glCreateShader(GL_VERTEX_SHADER);
	}
	else if (shaderType == "FRAGMENT") {
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

/*
* 创建着色器程序
* 添加着色器
* 链接着色器程序
* 校验
*/
unsigned int createShaderProgram(unsigned int vertexShaderID, unsigned int fragmentShaderID) {
	unsigned int shaderProgramID = glCreateProgram();

	glAttachShader(shaderProgramID, vertexShaderID);
	glAttachShader(shaderProgramID, fragmentShaderID); // 前一个着色器的输出,必须与下一个着色器的输入相匹配
	glLinkProgram(shaderProgramID); // 链接着色器

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

	glDeleteShader(vertexShaderID); 	// 链接完成后就可以删除着色器了
	glDeleteShader(fragmentShaderID);

	return shaderProgramID;
}


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
) {
	unsigned int VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO); // 顶点数组对象VAO
	glGenBuffers(1, &VBO); // 顶点缓冲对象
	glGenBuffers(1, &EBO); // 元素缓冲对象

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	std::cout << "indcies size" << sizeof(indices) << std::endl;
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW); 	// 复制顶点数据

	glVertexAttribPointer(locationIndex, dimension, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
	glEnableVertexAttribArray(locationIndex);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

	// 解绑
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return VAO;
}