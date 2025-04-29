// GLAD和GLFW
// 一些初始化工作
// 回调绑定
// 处理输入
// 双缓冲
// NDC坐标

#include <HEADER/utils.hpp>
#include <HEADER/call_backs.hpp>
#include <iostream>

const unsigned int SCREEN_WIDTH = 2000;
const unsigned int SCREEN_HEIGHT = 1500;

int main() {
	// 初始化GLFW和GLAD， 创建窗口，设置viewport
	GLFWwindow* window = initAndCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "learn_opengl");
	if (window == NULL) {
		glfwTerminate();
		return -1;
	}

	// 注册窗口大小的回调函数
	glfwSetFramebufferSizeCallback(window, framebufferSizeChangeCallback);

	while (!glfwWindowShouldClose(window)) {
		jobAtRenderLoopStart(window);

		// do something....
		std::cout << "rendering...\n";

		jobBeforRenderLoopEnd(window);
	}

	// 结束释放资源
	glfwTerminate();
	return 0;
}