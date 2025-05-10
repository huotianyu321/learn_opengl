#include <HEADER/process_events.hpp>

void processEscapeKey(GLFWwindow* window) {
	// ��鴰����ĳ�����İ���״̬����������˷���GLFW_PRESS, ���򷵻�GLFW_RELEASE
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void processUpDownKey(GLFWwindow* window, float* number) {
    const float STEP = 0.1f;
    const float MIN_VALUE = 0.0f;
    const float MAX_VALUE = 100.0f;

    if (number == nullptr) {
        return; // ��ֹ��ָ�����
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