// 在顶点着色器中定义风氏光照模型 - 此时叫做Gouraud着色，而不是Phong着色
// 异常效果：
/*
* 那么我们看到什么？
你可以看到（亲自或在提供的图像中）立方体前面两个三角形的清晰区分。
这个‘带’之所以可见是因为片段插值。
从示例图像中，我们可以看到立方体前面上右角的顶点被镜面高光照亮。
由于底右三角形的上右角顶点被照亮，而三角形的其他两个顶点没有被照亮，因此明亮的值会插值到其他两个顶点。
左上三角形也发生同样的情况。由于中间片段的颜色并不是直接来自光源，而是插值的结果，
因此在中间片段的 lighting 是不正确的，上左和下右三角形的亮度相撞，导致两个三角形之间出现明显的带。当使用更复杂的形状时，这种效果会变得更加明显。
*/

#include <HEADER/utils.hpp>
#include <HEADER/shader_class.hpp>
#include <HEADER/set_uniform.hpp>
#include <HEADER/process_events.hpp>
#include <HEADER/camera_class.hpp>

#include <iostream>

const int WIDTH = 1800;
const int HEIGHT = 1200;
const char* boxVertexCodePath = "./src/chapter2_light/2_basic_lighting/exercise4_box_vs.txt";
const char* boxFragmentCodePath = "./src/chapter2_light/2_basic_lighting/exercise4_box_fs.txt";
const char* lightFragmentCodePath = "./src/chapter2_light/2_basic_lighting/1_light_fs.txt";
const char* lightVertexCodePath = "./src/chapter2_light/2_basic_lighting/1_light_vs.txt";

Camera camera = Camera(
	glm::vec3(0.0f, 0.0f, 3.0f), // 初始位置
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
	//boxShader.setFloat("specPower", 32.0f);
	lightShader.use();
	lightShader.set3Float("lightColor", 1.0f, 1.0f, 1.0f);

	// 光源位置
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

	jobBeforeEnterRenderLoop(window);
	// 渲染循环
	while (!glfwWindowShouldClose(window)) {
		float t = glfwGetTime();
		float currentFrameTime = t;
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		jobAtRenderLoopStart(window);
		processWASD(window, camera, deltaTime);

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
		boxShader.set1Vec3("viewPos", camera.Position);

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
