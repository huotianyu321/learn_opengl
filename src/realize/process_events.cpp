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

void processWASD(
	GLFWwindow* window,
	float deltaTime,
	glm::vec3& cameraPos,
	const glm::vec3& cameraFront,
	const glm::vec3& worldUp
) {
	// ���ﲻ��д�ɳ���, ÿ��֮֡���ʱ���ǹ̶���
	// ʱ���Խ��, �ƶ�Ӧ��Խ��
	float cameraDisplacement = 10.0f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += cameraDisplacement * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos -= cameraDisplacement * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
		cameraPos -= cameraRight * cameraDisplacement;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
		cameraPos += cameraRight * cameraDisplacement;
	}
}


void processWASD(
	GLFWwindow* window,
	Camera& camera,
	float deltaTime
) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		camera.ProcessKeyboard(UP, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		camera.ProcessKeyboard(DOWN, deltaTime);
	}
}
