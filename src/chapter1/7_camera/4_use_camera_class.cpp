// 使用相机类

#include <HEADER/utils.hpp>
#include <HEADER/shader_class.hpp>
#include <HEADER/set_uniform.hpp>
#include <HEADER/process_events.hpp>
#include <HEADER/camera_class.hpp>

#include <iostream>

const int WIDTH = 1800;
const int HEIGHT = 1200;
const char* texture1_path = "./resources/container.jpg";
const char* texture2_path = "./resources/awesomeface.png";
const char* vertexCodePath = "./src/chapter1/6_coordinate_system/1_code.vs";
const char* fragmentCodePath = "./src/chapter1/6_coordinate_system/1_code.fs";

Camera camera = Camera(
	glm::vec3(0.0f, 0.0f, 3.0f), 
	glm::vec3(0.0f, 1.0f, 0.0f),
	-90.0f, 
	0.0f
);

float lastX = WIDTH / 2; // 上一帧鼠标位置
float lastY = HEIGHT / 2; // 当前帧与上一帧的时间差
float deltaTime = 0.0f; // 上一帧的时间
float lastFrameTime = 0.0f;
float isFirstTimeMouseMove = true;

void windowSizeChangeCallback(GLFWwindow* window, int newWidth, int newHeight);
void mouseMoveCallBack(GLFWwindow* window, double xposIn, double yposIn);
void mouseScrollCallBack(GLFWwindow* window, double xoffset, double yoffset);

int main() {

	GLFWwindow* window = initAndCreateWindow(WIDTH, HEIGHT, "6.2");
	if (window == nullptr) {
		glfwTerminate();
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, windowSizeChangeCallback);
	glfwSetScrollCallback(window, mouseScrollCallBack);
	glfwSetCursorPosCallback(window, mouseMoveCallBack);

	// 立方体的顶点， 每个面两个三角形，每个三角形3个顶点 6 * 2 * 3
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
	VAOData vaoData = set_VAO_VBO_EBO_mutiple(
		vertices, sizeof(vertices), nullptr, 0,
		3, location, dimension, stride, offset
	);

	unsigned int VAO = vaoData.VAO;
	unsigned int VBO = vaoData.VBO;
	unsigned int EBO = vaoData.EBO;

	// 第一个纹理
	unsigned int texture1 = createTexture(
		texture1_path,
		GL_REPEAT, GL_REPEAT,
		GL_LINEAR, GL_LINEAR,
		false,
		false
	);
	// 第二个纹理
	unsigned int texture2 = createTexture(
		texture2_path,
		GL_MIRRORED_REPEAT, GL_REPEAT,
		GL_LINEAR, GL_LINEAR,
		true,
		true
	);

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

	// 开启深度测试， 如果不开启，可以看到立方体在旋转过程中穿模
	jobBeforeEnterRenderLoop(window);
	// 渲染循环
	while (!glfwWindowShouldClose(window)) {
		float t = glfwGetTime();
		float currentFrameTime = t;
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		jobAtRenderLoopStart(window);
		processWASD(window, camera, deltaTime);

		// 绑定纹理
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		// 激活着色器
		ourShader.use();

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
			// 3的倍数的立方体随时间旋转
			if (i % 3 == 0) {
				angle = t * 25.0f;
			}

			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ourShader.setMat4("model", model);

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36); // 绘制立方体
		}

		jobBeforRenderLoopEnd(window);
	}

	glDisable(GL_DEPTH_TEST); 
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteTextures(1, &texture1);
	glDeleteTextures(1, &texture2);
	glfwTerminate();
	return 0;
}


void mouseMoveCallBack(GLFWwindow* window, double xPosIn, double yPosIn) {
	// 当前帧的鼠标位置
	float xPos = static_cast<float>(xPosIn);
	float yPos = static_cast<float>(yPosIn);
	// std::cout << "X: " << xPos << " Y: " << yPos << std::endl;

	if (isFirstTimeMouseMove) {
		lastX = xPos;
		lastY = yPos;
		isFirstTimeMouseMove = false;
	}

	// 计算鼠标移动的距离
	float xOffset = xPos - lastX;
	// 反向y轴. 屏幕坐标系是以左上角为原点的, 当鼠标向下移动时，y坐标增大，但此时俯仰角应该减小
	float yOffset = lastY - yPos;

	// 更新上一帧鼠标位置
	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset); // 更新相机的朝向
}


void mouseScrollCallBack(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void windowSizeChangeCallback(GLFWwindow* window, int newWidth, int newHeight) {
	glViewport(0, 0, newWidth, newHeight);
}