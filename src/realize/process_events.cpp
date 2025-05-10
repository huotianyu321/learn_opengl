#include <HEADER/process_events.hpp>

void processEscapeKey(GLFWwindow* window) {
	// 检查窗口中某个键的按下状态，如果按下了返回GLFW_PRESS, 否则返回GLFW_RELEASE
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void processUpDownKey(GLFWwindow* window, float* number) {
    const float STEP = 0.1f;
    const float MIN_VALUE = 0.0f;
    const float MAX_VALUE = 100.0f;

    if (number == nullptr) {
        return; // 防止空指针崩溃
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        *number += STEP;
        if (*number > MAX_VALUE) {
            *number = MAX_VALUE;
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        *number -= STEP;
        if (*number < MIN_VALUE) {
            *number = MIN_VALUE;
        }
    }
}