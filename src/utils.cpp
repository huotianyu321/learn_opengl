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

