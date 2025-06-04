// 使用相机类

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

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); // 初始相机位置
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // 初始相机朝向
glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
float yaw = -90.0f; // 偏航角, 一开始是看向-z轴的
float pitch = 0.0f; // 俯仰角, 一开始是水平的
Camera camera = Camera(cameraPos, worldUp, yaw, pitch);

float lastX = WIDTH / 2; // 记录上一帧鼠标位置. 初始设置为窗口中心，因为一开始相机的俯仰角和偏航角为0
float lastY = HEIGHT / 2;
//bool isMouseFirstIn = true; // 特殊处理鼠标第一次进入窗口
bool mouseControlActive = false; // 当鼠标移到窗口中心再激活
float sensitivity = 0.05f; // 鼠标灵敏度

float fov = 45.0f; // 视野角度

float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrameTime = 0.0f; // 上一帧的时间

// 处理wasd键盘输入
void processWASD(GLFWwindow* window);
void mouse_move_cb(GLFWwindow* window, double xposIn, double yposIn);
void mouse_scroll_cb(GLFWwindow* window, double xoffset, double yoffset);

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 设置全屏窗口
	/*GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Title", monitor, NULL);*/

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "COORDINATE SYSTEM", NULL, NULL);
	if (window == NULL) {
		std::cout << "创建窗口失败" << std::endl;
		glfwTerminate();
		return -1;
	}
	// 设置窗口的上下文
	glfwMakeContextCurrent(window);
	// 绑定窗口大小改变的回调函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// 绑定鼠标移动回调
	glfwSetCursorPosCallback(window, mouse_move_cb);
	// 将鼠标限制在窗口内，并隐藏鼠标光标 // 有bug
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// 绑定鼠标滚轮回调
	glfwSetScrollCallback(window, mouse_scroll_cb);

	// 初始化GLAD 
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "加载GLAD失败" << std::endl;
		return -1;
	}
	glViewport(0, 0, WIDTH, HEIGHT); // 设置视口大小

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
	// 第一个纹理
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// 设置纹理环绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 设置纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 加载纹理图片,生成纹理并创建多级渐远纹理
	int width, height, nrChannels;
	//stbi_set_flip_vertically_on_load(true); // 翻转y轴
	unsigned char* data = stbi_load(texture1_path, &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "纹理加载失败" << std::endl;
	}
	stbi_image_free(data);

	// 第二个纹理
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// 设置纹理环绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 设置纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 加载纹理图片, 生成纹理并创建多级渐远纹理
	unsigned char* data2 = stbi_load(texture2_path, &width, &height, &nrChannels, 0);
	if (data2) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "纹理加载失败" << std::endl;
	}
	stbi_image_free(data2);

	// 着色器
	Shader ourShader(vertexCodePath, fragmentCodePath);
	// 将纹理采样器绑定到纹理单元
	ourShader.use();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);

	// 绘制十个立方体,通过model矩阵实现不同位置的立方体
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

	// 开启深度测试
	glEnable(GL_DEPTH_TEST);
	// 渲染循环
	while (!glfwWindowShouldClose(window)) {
		processInupt(window);
		processWASD(window); // 处理WASD键盘输入, 改变全局变量cameraPos

		float currentFrameTime = (float)glfwGetTime();
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		// 清除颜色缓冲区
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// =====清除深度缓冲区====
		glClear(GL_DEPTH_BUFFER_BIT);

		// 绑定纹理
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		// 激活着色器
		ourShader.use();

		// 鼠标滚轮控制了fov，每帧重新计算
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f); // 透视投影
		ourShader.setMat4("projection", projection); // 透视投影矩阵

		// 创建观察矩阵
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);

		// 通过模型矩阵在循环中改变立方体的位置(使用相同的顶点数据)
		for (unsigned int i = 0; i < 10; ++i) {
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]); // 平移

			float angle = 20.0f * i; // 旋转角度	
			angle = glfwGetTime() * 25.0f;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

			// 传递模型矩阵到着色器
			//unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
			//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			ourShader.setMat4("model", model);

			// 绘制矩形
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36); // 绘制立方体
		}

		// 交换缓冲区, 处理事件
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDisable(GL_DEPTH_TEST); // 关闭深度测试

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
	// 当前帧的鼠标位置
	float xPos = static_cast<float>(xPosIn);
	float yPos = static_cast<float>(yPosIn);
	std::cout << "X: " << xPos << " Y: " << yPos << std::endl;

	// 让鼠标从外边移动到中心再激活控制
	if (
		!mouseControlActive
		&& xPos >= WIDTH / 2 - 50
		&& xPos <= WIDTH / 2 + 50
		&& yPos >= HEIGHT / 2 - 50
		&& yPos <= HEIGHT / 2 + 50
		) {
		mouseControlActive = true; // 鼠标移到窗口中心时激活
		lastX = xPos;
		lastY = yPos;
	}

	if (!mouseControlActive) {
		return;
	}


	//if (isMouseFirstIn) {
	//	lastX = xPos; // 记录鼠标第一次进入窗口时的位置
	//	lastY = yPos;
	//	isMouseFirstIn = false; // 只处理一次
	//}

	// 计算鼠标移动的距离
	float xOffset = xPos - lastX;
	// 反向y轴，屏幕坐标系是以左上角为原点的. 
	// 当鼠标向下移动时，y坐标增大，但此时俯仰角应该减小
	float yOffset = lastY - yPos;

	// 更新上一帧鼠标位置
	lastX = xPos;
	lastY = yPos;

	yaw += xOffset * sensitivity; // 更新偏航角
	pitch += yOffset * sensitivity; // 更新俯仰角

	camera.ProcessMouseMovement(xOffset, yOffset); // 更新相机的朝向
}


void mouse_scroll_cb(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}
