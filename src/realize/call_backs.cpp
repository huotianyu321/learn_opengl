#include <HEADER/call_backs.hpp>
#include <iostream>

void framebufferSizeChangeCallback(GLFWwindow* window, int newWidth, int newHeight) {
	glViewport(0, 0, newWidth, newHeight);
}
