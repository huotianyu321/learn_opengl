// 在观察空间中实现Phong光照模型
// 主程序没什么差别，主要在光照着色器的中

#include <HEADER/utils.hpp>
#include <HEADER/shader_class.hpp>
#include <HEADER/set_uniform.hpp>
#include <HEADER/process_events.hpp>
#include <HEADER/camera_class.hpp>

#include <iostream>
#include <cmath>

const int WIDTH = 1800;
const int HEIGHT = 1200;
const char* boxVertexCodePath = "./src/chapter2_light/2_basic_lighting/exercise3_box_vs.txt";
const char* boxFragmentCodePath = "./src/chapter2_light/2_basic_lighting/exercise3_box_fs.txt";
const char* lightFragmentCodePath = "./src/chapter2_light/2_basic_lighting/1_light_fs.txt";
const char* lightVertexCodePath = "./src/chapter2_light/2_basic_lighting/1_light_vs.txt";

Camera camera = Camera(
	glm::vec3(0.0f, 0.0f, 4.0f), // 初始位置
	glm::vec3(0.0f, 1.0f, 0.0f), // worldUp
	-90.0f, // 初始Yaw 偏航角
	0.0f // 初始Pitch 俯仰角
);

float lastX = WIDTH / 2; 
float lastY = HEIGHT / 2;
float deltaTime = 0.0f;
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

	// 前3个是顶点位置，后三个是法向量
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	unsigned int locations[] = { 0, 1 };
	unsigned int dimensions[] = { 3, 3 };
	unsigned int strides[] = { 6 , 6 };
	unsigned int offsets[] = { 0, 3 };

	VAOData boxVaoData = set_VAO_VBO_EBO_mutiple(
		vertices, sizeof(vertices), nullptr, 0,
		2, locations, dimensions, strides, offsets
	);
	VAOData lightVaoData = set_VAO_VBO_EBO(
		vertices, sizeof(vertices), nullptr, 0,
		0, 3, 6, 0
	);


	// 光照着色器(应用在立方体上）
	Shader boxShader(boxVertexCodePath, boxFragmentCodePath);
	// 光源着色器
	Shader lightShader(lightVertexCodePath, lightFragmentCodePath);

	boxShader.use();
	boxShader.set3Float("lightColor", 1.0f, 1.0f, 1.0f);
	boxShader.set1Vec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
	// 尝试改变高光反光度
	// 高光度越大，反射能力越强，散射越少，反射光的光斑越集中
	boxShader.setFloat("specPower", 128.0f);
	lightShader.use();
	lightShader.set3Float("lightColor", 1.0f, 1.0f, 1.0f);

	jobBeforeEnterRenderLoop(window);
	// 渲染循环
	while (!glfwWindowShouldClose(window)) {
		float t = glfwGetTime();
		float currentFrameTime = t;
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		jobAtRenderLoopStart(window);
		processWASD(window, camera, deltaTime);

		// 光源位置
		glm::vec3 lightPos(2.0f * sin(t), 0.0f, 2.0f * cos(t));

		// 透视投影矩阵
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f); // 透视投影

		// 观察矩阵
		glm::mat4 view = camera.GetViewMatrix();

		// box 模型矩阵
		glm::mat4 boxModel;

		// 绘制box
		boxShader.use();
		boxShader.setMat4("projection", projection);
		boxShader.setMat4("view", view);
		boxShader.setMat4("model", boxModel);
		boxShader.set3Float("lightPos", lightPos.x, lightPos.y, lightPos.z);

		glBindVertexArray(boxVaoData.VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36); // 绘制立方体

		// 光源 模型矩阵
		glm::mat4 lightModel;
		lightModel = glm::translate(lightModel, lightPos);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f));

		// 绘制光源立方体
		lightShader.use();
		lightShader.setMat4("projection", projection);
		lightShader.setMat4("view", view);
		lightShader.setMat4("model", lightModel);
		glBindVertexArray(lightVaoData.VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36); // 绘制光源

		jobBeforRenderLoopEnd(window);
	}

	glDisable(GL_DEPTH_TEST);
	glDeleteBuffers(1, &boxVaoData.VBO);
	glDeleteBuffers(1, &boxVaoData.EBO);
	glDeleteVertexArrays(1, &boxVaoData.VAO);
	glDeleteBuffers(1, &lightVaoData.VBO);
	glDeleteBuffers(1, &lightVaoData.EBO);
	glDeleteBuffers(1, &boxVaoData.VBO);
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
