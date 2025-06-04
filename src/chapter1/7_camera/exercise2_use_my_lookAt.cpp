// ʹ�������

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <headers/shader_class.h>
#include <headers/call_backs.h>

#include "exercise2_my_lookAt.h"

const unsigned int WIDTH = 1200;
const unsigned int HEIGHT = 900;

const char* texture1_path = "./resources/container.jpg";
const char* texture2_path = "./resources/awesomeface.png";
const char* vertexCodePath = "./src/chapter1/6_coordinate_system/1_shader.vertex";
const char* fragmentCodePath = "./src/chapter1/6_coordinate_system/1_shader.fragment";

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); // ��ʼ���λ��
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // ��ʼ�������
glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
float yaw = -90.0f; // ƫ����, һ��ʼ�ǿ���-z���
float pitch = 0.0f; // ������, һ��ʼ��ˮƽ��
Camera camera = Camera(cameraPos, worldUp, yaw, pitch);

float lastX = WIDTH / 2; // ��¼��һ֡���λ��. ��ʼ����Ϊ�������ģ���Ϊһ��ʼ����ĸ����Ǻ�ƫ����Ϊ0
float lastY = HEIGHT / 2;
//bool isMouseFirstIn = true; // ���⴦������һ�ν��봰��
bool mouseControlActive = false; // ������Ƶ����������ټ���
float sensitivity = 0.05f; // ���������

float fov = 45.0f; // ��Ұ�Ƕ�

float deltaTime = 0.0f; // ��ǰ֡����һ֡��ʱ���
float lastFrameTime = 0.0f; // ��һ֡��ʱ��

// ����wasd��������
void processWASD(GLFWwindow* window);
void mouse_move_cb(GLFWwindow* window, double xposIn, double yposIn);
void mouse_scroll_cb(GLFWwindow* window, double xoffset, double yoffset);

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// ����ȫ������
	/*GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Title", monitor, NULL);*/

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "COORDINATE SYSTEM", NULL, NULL);
	if (window == NULL) {
		std::cout << "��������ʧ��" << std::endl;
		glfwTerminate();
		return -1;
	}
	// ���ô��ڵ�������
	glfwMakeContextCurrent(window);
	// �󶨴��ڴ�С�ı�Ļص�����
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// ������ƶ��ص�
	glfwSetCursorPosCallback(window, mouse_move_cb);
	// ����������ڴ����ڣ������������ // ��bug
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// �������ֻص�
	glfwSetScrollCallback(window, mouse_scroll_cb);

	// ��ʼ��GLAD 
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "����GLADʧ��" << std::endl;
		return -1;
	}
	glViewport(0, 0, WIDTH, HEIGHT); // �����ӿڴ�С

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

	// VAO, VBO
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture
	unsigned int texture1, texture2;
	// ��һ������
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// ���������Ʒ�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// ����������˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// ��������ͼƬ,�������������༶��Զ����
	int width, height, nrChannels;
	//stbi_set_flip_vertically_on_load(true); // ��תy��
	unsigned char* data = stbi_load(texture1_path, &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "�������ʧ��" << std::endl;
	}
	stbi_image_free(data);

	// �ڶ�������
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// ���������Ʒ�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// ����������˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// ��������ͼƬ, �������������༶��Զ����
	unsigned char* data2 = stbi_load(texture2_path, &width, &height, &nrChannels, 0);
	if (data2) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "�������ʧ��" << std::endl;
	}
	stbi_image_free(data2);

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

	// ������Ȳ���
	glEnable(GL_DEPTH_TEST);
	// ��Ⱦѭ��
	while (!glfwWindowShouldClose(window)) {
		processInupt(window);
		processWASD(window); // ����WASD��������, �ı�ȫ�ֱ���cameraPos

		float currentFrameTime = (float)glfwGetTime();
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		// �����ɫ������
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// =====�����Ȼ�����====
		glClear(GL_DEPTH_BUFFER_BIT);

		// ������
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		// ������ɫ��
		ourShader.use();

		// �����ֿ�����fov��ÿ֡���¼���
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f); // ͸��ͶӰ
		ourShader.setMat4("projection", projection); // ͸��ͶӰ����

		// �����۲����
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);

		// ͨ��ģ�;�����ѭ���иı��������λ��(ʹ����ͬ�Ķ�������)
		for (unsigned int i = 0; i < 10; ++i) {
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]); // ƽ��

			float angle = 20.0f * i; // ��ת�Ƕ�	
			angle = glfwGetTime() * 25.0f;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

			// ����ģ�;�����ɫ��
			//unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
			//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			ourShader.setMat4("model", model);

			// ���ƾ���
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36); // ����������
		}

		// ����������, �����¼�
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDisable(GL_DEPTH_TEST); // �ر���Ȳ���

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	glDeleteTextures(1, &texture1);
	glDeleteTextures(1, &texture2);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


void processWASD(GLFWwindow* window) {
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
}

void mouse_move_cb(GLFWwindow* window, double xPosIn, double yPosIn) {
	// ��ǰ֡�����λ��
	float xPos = static_cast<float>(xPosIn);
	float yPos = static_cast<float>(yPosIn);
	std::cout << "X: " << xPos << " Y: " << yPos << std::endl;

	// ����������ƶ��������ټ������
	if (
		!mouseControlActive
		&& xPos >= WIDTH / 2 - 50
		&& xPos <= WIDTH / 2 + 50
		&& yPos >= HEIGHT / 2 - 50
		&& yPos <= HEIGHT / 2 + 50
		) {
		mouseControlActive = true; // ����Ƶ���������ʱ����
		lastX = xPos;
		lastY = yPos;
	}

	if (!mouseControlActive) {
		return;
	}


	//if (isMouseFirstIn) {
	//	lastX = xPos; // ��¼����һ�ν��봰��ʱ��λ��
	//	lastY = yPos;
	//	isMouseFirstIn = false; // ֻ����һ��
	//}

	// ��������ƶ��ľ���
	float xOffset = xPos - lastX;
	// ����y�ᣬ��Ļ����ϵ�������Ͻ�Ϊԭ���. 
	// ����������ƶ�ʱ��y�������󣬵���ʱ������Ӧ�ü�С
	float yOffset = lastY - yPos;

	// ������һ֡���λ��
	lastX = xPos;
	lastY = yPos;

	yaw += xOffset * sensitivity; // ����ƫ����
	pitch += yOffset * sensitivity; // ���¸�����

	camera.ProcessMouseMovement(xOffset, yOffset); // ��������ĳ���
}


void mouse_scroll_cb(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}
