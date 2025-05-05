#include <HEADER/call_backs.hpp>

void framebufferSizeChangeCallback(GLFWwindow* window, int newWidth, int newHeight) {
	glViewport(0, 0, newWidth, newHeight);
}

void processEscapeKey(GLFWwindow* window) {
	// 检查窗口中某个键的按下状态，如果按下了返回GLFW_PRESS, 否则返回GLFW_RELEASE
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}