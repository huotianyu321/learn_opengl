#include <HEADER/utils.hpp>
#include <HEADER/shader_class.hpp>
#include <HEADER/set_uniform.hpp>
#include <HEADER/process_events.hpp>
#include <HEADER/camera_class.hpp>

#include <iostream>

typedef unsigned int uint;

const int WIDTH = 1800;
const int HEIGHT = 1200;
const char* boxVertexCodePath = "./src/chapter2_light/6_multi_light_caster/1_box_vs.txt";
const char* boxFragmentCodePath = "./src/chapter2_light/6_multi_light_caster/1_box_fs.txt";
const char* lightFragmentCodePath = "./src/chapter2_light/2_basic_lighting/1_light_fs.txt";
const char* lightVertexCodePath = "./src/chapter2_light/2_basic_lighting/1_light_vs.txt";
const char* diffuseTexMapPath = "./resources/container2.png";
const char* specularTexMapPath = "./resources/container2_specular.png";

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

	GLFWwindow* window = initAndCreateWindow(WIDTH, HEIGHT, "opengl");
	if (window == nullptr) {
		glfwTerminate();
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, windowSizeChangeCallback);
	glfwSetScrollCallback(window, mouseScrollCallBack);
	glfwSetCursorPosCallback(window, mouseMoveCallBack);

	// 前3个是顶点位置，中间三个是法向量， 后两个是纹理坐标
	float vertices[] = {
		// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	unsigned int locations[] = { 0, 1, 2 };
	unsigned int dimensions[] = { 3, 3, 2 };
	unsigned int strides[] = { 8, 8, 8 };
	unsigned int offsets[] = { 0, 3, 6 };

	VAOData boxVaoData = set_VAO_VBO_EBO_mutiple(
		vertices, sizeof(vertices), nullptr, 0,
		3, locations, dimensions, strides, offsets
	);
	VAOData lightVaoData = set_VAO_VBO_EBO(
		vertices, sizeof(vertices), nullptr, 0,
		0, 3, 8, 0
	);

	uint diffuseTexMap = createTexture(diffuseTexMapPath, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR, true, false);
	uint specularTexMap = createTexture(specularTexMapPath, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR, true, false);

	// 光照着色器(应用在立方体上）
	Shader boxShader(boxVertexCodePath, boxFragmentCodePath);
	boxShader.use();
	boxShader.setFloat("material.shininess", 32.0f);
	boxShader.setInt("material.diffuse", 0); // 将漫反射贴图纹理采样器绑定到纹理单元0
	boxShader.setInt("material.specular", 1); // 将镜面反射贴图纹理采样器绑定到纹理单元1

	// 光源着色器
	Shader lightShader(lightVertexCodePath, lightFragmentCodePath);
	lightShader.use();
	lightShader.set3Float("lightColor", 1.0f, 1.0f, 1.0f);

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

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};
	glm::vec3 pointLightColors[] = {
		glm::vec3(1.0f, 0.6f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0, 0.0),
		glm::vec3(0.2f, 0.2f, 1.0f)
	};

	jobBeforeEnterRenderLoop(window);
	// 渲染循环
	while (!glfwWindowShouldClose(window)) {
		float t = glfwGetTime();
		float currentFrameTime = t;
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		jobAtRenderLoopStart(window); // 把清屏颜色换成glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		processWASD(window, camera, deltaTime);

		// 透视投影矩阵
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f); // 透视投影

		// 观察矩阵
		glm::mat4 view = camera.GetViewMatrix();

		// 绘制box
		boxShader.use();
		boxShader.setMat4("projection", projection);
		boxShader.setMat4("view", view);
		boxShader.set1Vec3("viewPos", camera.Position);

		// dirLight
		boxShader.set3Float("dirLight.direction", -0.2f, -1.0f, -0.3f);
		boxShader.set3Float("dirLight.ambient", 0.3f, 0.24f, 0.14f);
		boxShader.set3Float("dirLight.diffuse", 0.7f, 0.42f, 0.26f);
		boxShader.set3Float("dirLight.specular", 0.5f, 0.5f, 0.5f);
		// spotLight
		boxShader.set1Vec3("spotLight.position", camera.Position);
		boxShader.set1Vec3("spotLight.direction", camera.Front);
		boxShader.setFloat("spotLight.constant", 1.0f);
		boxShader.setFloat("spotLight.linear", 0.09f);
		boxShader.setFloat("spotLight.quadratic", 0.032f);
		boxShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f))); // 内切光角
		boxShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(13.0f))); // 外切光角 
		boxShader.set3Float("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		boxShader.set3Float("spotLight.diffuse", 0.8f, 0.8f, 0.0f);
		boxShader.set3Float("spotLight.specular", 0.8f, 0.8f, 0.0f);
		// point light
		for (int i = 0; i < 4; i++) {
			const std::string prefix = "pointLights[" + std::to_string(i) + "]";
			boxShader.set1Vec3((prefix + ".position").c_str(), pointLightPositions[i]);
			boxShader.set3Float((prefix + ".ambient").c_str(), pointLightColors[i].x * 0.1, pointLightColors[i].y * 0.1, pointLightColors[i].z * 0.1);
			boxShader.set1Vec3((prefix + ".diffuse").c_str(), pointLightColors[i]);
			boxShader.set1Vec3((prefix + ".specular").c_str(), pointLightColors[i]);
			boxShader.setFloat((prefix + ".constant").c_str(), 1.0f);
			boxShader.setFloat((prefix + ".linear").c_str(), 0.09f);
			boxShader.setFloat((prefix + ".quadratic").c_str(), 0.032f);
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseTexMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularTexMap);
		glBindVertexArray(boxVaoData.VAO);

		for (uint i = 0; i < 10; ++i) {
			glm::mat4 boxModel;
			boxModel = glm::translate(boxModel, cubePositions[i]);
			float angle = 20.0f * i;
			boxModel = glm::rotate(boxModel, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			boxShader.setMat4("model", boxModel);

			glDrawArrays(GL_TRIANGLES, 0, 36); // 绘制立方体
		}

		// 绘制光源
		lightShader.use();
		lightShader.setMat4("projection", projection);
		lightShader.setMat4("view", view);
		glBindVertexArray(lightVaoData.VAO);

		for (uint i = 0; i < 4; ++i) {
			glm::mat4 lightModel;
			lightModel = glm::translate(lightModel, pointLightPositions[i]);
			lightModel = glm::scale(lightModel, glm::vec3(0.2f));
			lightShader.setMat4("model", lightModel);

			glDrawArrays(GL_TRIANGLES, 0, 36); // 绘制光源
		}


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
