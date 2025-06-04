// ͨ��ŷ��������������ĳ���cameraFront����
/*
* glm::vec3 front; // �����������
* front.y = sin(glm::radians(pitch));
* front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
* front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
* cameraFront = glm::normalize(front); // ��һ��
*/
// �������ֻ��Ƹ����Ǻ�ƫ���ǣ����漰��ת��

// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);���������bug

#include <HEADER/utils.hpp>
#include <HEADER/call_backs.hpp>
#include <HEADER/shader_class.hpp>
#include <HEADER/set_uniform.hpp>
#include <HEADER/process_events.hpp>

#include <iostream>

const int WIDTH = 1800;
const int HEIGHT = 1200;
const char* texture1_path = "./resources/container.jpg";
const char* texture2_path = "./resources/awesomeface.png";
const char* vertexCodePath = "./src/chapter1/6_coordinate_system/1_code.vs";
const char* fragmentCodePath = "./src/chapter1/6_coordinate_system/1_code.fs";

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

float yaw = -90.0f; // ƫ����, һ��ʼ�ǿ���-z���
float pitch = 0.0f; // ������, һ��ʼ��ˮƽ��
float fov = 45.0f; // ��Ұ�Ƕ�

float lastX = WIDTH / 2; // ��¼��һ֡���λ��. ��ʼ����Ϊ�������ģ���Ϊһ��ʼ����ĸ����Ǻ�ƫ����Ϊ0
float lastY = HEIGHT / 2; 
float isFirstTimeMouseMove = true;
float sensitivity = 0.05f; // ���������

float deltaTime = 0.0f; // ��ǰ֡����һ֡��ʱ���
float lastFrameTime = 0.0f; // ��һ֡��ʱ��

void mouse_move_cb(GLFWwindow* window, double xposIn, double yposIn);
void mouse_scroll_cb(GLFWwindow* window, double xoffset, double yoffset);

int main() {
	GLFWwindow* window = initAndCreateWindow(WIDTH, HEIGHT, "6.2");
	if (window == nullptr) {
		glfwTerminate();
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, framebufferSizeChangeCallback);
	glfwSetScrollCallback(window, mouse_scroll_cb);
	glfwSetCursorPosCallback(window, mouse_move_cb);

	// ������Ķ��㣬 ÿ�������������Σ�ÿ��������3������ 6 * 2 * 3
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	unsigned int location[] = { 0, 1 };
	unsigned int dimension[] = { 3, 2 };
	unsigned int stride[] = { 5, 5 };
	unsigned int offset[] = { 0, 3 };
	unsigned int VAO = set_VAO_VBO_EBO_mutiple(
		vertices, sizeof(vertices), nullptr, 0,
		3, location, dimension, stride, offset
	);

	// ��һ������
	unsigned int texture1 = createTexture(
		texture1_path,
		GL_REPEAT, GL_REPEAT,
		GL_LINEAR, GL_LINEAR,
		false,
		false
	);
	// �ڶ�������
	unsigned int texture2 = createTexture(
		texture2_path,
		GL_MIRRORED_REPEAT, GL_REPEAT,
		GL_LINEAR, GL_LINEAR,
		true,
		true
	);

	// ��ɫ��
	Shader ourShader(vertexCodePath, fragmentCodePath);
	// ������������󶨵�����Ԫ
	ourShader.use();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);

	// ����ʮ��������,ͨ��model����ʵ�ֲ�ͬλ�õ�������
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// ������Ȳ��ԣ� ��������������Կ�������������ת�����д�ģ
	jobBeforeEnterRenderLoop(window);
	// ��Ⱦѭ��
	while (!glfwWindowShouldClose(window)) {
		float t = glfwGetTime();
		float currentFrameTime = t;
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		jobAtRenderLoopStart(window);
		processWASD(window, deltaTime, cameraPos, cameraFront, worldUp);

		// ������
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		// ������ɫ��
		ourShader.use();

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(fov), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f); // ͸��ͶӰ
		ourShader.setMat4("projection", projection); // ͸��ͶӰ����

		// �����۲����
		glm::mat4 view;
		view = glm::lookAt(
			cameraPos, // eye
			cameraPos + cameraFront, // center
			worldUp // wroldup
		);
		ourShader.setMat4("view", view);

		// ͨ��ģ�;�����ѭ���иı��������λ��(ʹ����ͬ�Ķ�������)
		for (unsigned int i = 0; i < 10; ++i) {
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]); // ƽ��

			float angle = 20.0f * i; // ��ת�Ƕ�
			// 3�ı�������������ʱ����ת
			if (i % 3 == 0) {
				angle = t * 25.0f;
			}

			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ourShader.setMat4("model", model);

			// ���ƾ���
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36); // ����������
		}

		jobBeforRenderLoopEnd(window);
	}

	// ������....
	return 0;
}


void mouse_move_cb(GLFWwindow* window, double xPosIn, double yPosIn) {
	// ��ǰ֡�����λ��
	float xPos = static_cast<float>(xPosIn);
	float yPos = static_cast<float>(yPosIn);
	// std::cout << "X: " << xPos << " Y: " << yPos << std::endl;

	if (isFirstTimeMouseMove) {
		lastX = xPos;
		lastY = yPos;
		isFirstTimeMouseMove = false;
	}

	// ��������ƶ��ľ���
	float xOffset = xPos - lastX;
	// ����y��. ��Ļ����ϵ�������Ͻ�Ϊԭ���, ����������ƶ�ʱ��y�������󣬵���ʱ������Ӧ�ü�С
	float yOffset = lastY - yPos;

	// ������һ֡���λ��
	lastX = xPos;
	lastY = yPos;

	yaw += xOffset * sensitivity; // ����ƫ����
	pitch += yOffset * sensitivity; // ���¸�����

	// ���Ƹ��������²�����90��
	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	glm::vec3 front; // �����������
	front.y = sin(glm::radians(pitch)); 
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch)); 
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch)); 
	cameraFront = glm::normalize(front); // ��һ��
}


void mouse_scroll_cb(GLFWwindow* window, double xoffset, double yoffset) {
	float xOffset = static_cast<float>(xoffset);
	float yOffset = static_cast<float>(yoffset);
	std::cout << "X: " << xOffset << " Y: " << yOffset << std::endl;
	fov += yOffset;
	if (fov < 1.0f) {
		fov = 1.0f;
	}
	if (fov > 45.0f) {
		fov = 45.0f;
	}
}
