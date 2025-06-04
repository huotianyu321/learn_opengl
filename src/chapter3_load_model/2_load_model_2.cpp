// ʹ��assimp�����һ��ģ�ͣ������һ������Ч��
#include <stb_image/stb_image.h>

#include <HEADER/utils.hpp>
#include <HEADER/shader_class.hpp>
#include <HEADER/set_uniform.hpp>
#include <HEADER/process_events.hpp>
#include <HEADER/camera_class.hpp>
#include <HEADER/model_class.hpp>

#include <iostream>

typedef unsigned int uint;

const int WIDTH = 1800;
const int HEIGHT = 1200;
const char* modelPath = "./resources/backpack/backpack.obj";
const char* vertexShaderPath = "./src/chapter3_load_model/2_vs.txt";
const char* fragmentShaderPath = "./src/chapter3_load_model/2_fs.txt";

Camera camera = Camera(
	glm::vec3(0.0f, 0.0f, 3.0f), // ��ʼλ��
	glm::vec3(0.0f, 1.0f, 0.0f), // worldUp
	-90.0f, // ��ʼYaw ƫ����
	0.0f // ��ʼPitch ������
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

	stbi_set_flip_vertically_on_load(true);

	Shader shader(vertexShaderPath, fragmentShaderPath);
	Model model(modelPath);

	shader.use();
	shader.setFloat("material.shininess", 32.0f);
	shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
	shader.setFloat("spotLight.constant", 1.0f);
	shader.setFloat("spotLight.linear", 0.014f);
	shader.setFloat("spotLight.quadratic", 0.0007f);
	shader.set3Float("spotLight.ambient", 0.2f, 0.2f, 0.2f);
	shader.set3Float("spotLight.diffuse", 0.5f, 0.5f, 0.5f);
	shader.set3Float("spotLight.spcular", 1.0f, 1.0f, 1.0f);

	jobBeforeEnterRenderLoop(window);
	// ��Ⱦѭ��
	while (!glfwWindowShouldClose(window)) {
		float t = glfwGetTime();
		float currentFrameTime = t;
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		jobAtRenderLoopStart(window); // ��������ɫ����glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		processWASD(window, camera, deltaTime);

		// ͸��ͶӰ����
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f); // ͸��ͶӰ
		// �۲����
		glm::mat4 view = camera.GetViewMatrix();
		// ģ�;���
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		shader.use();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		shader.setMat4("model", modelMatrix);
		shader.set1Vec3("viewPos", camera.Position);
		shader.set1Vec3("spotLight.position", camera.Position);
		shader.set1Vec3("spotLight.direction", camera.Front);

		model.Draw(shader);

		jobBeforRenderLoopEnd(window);
	}

	glfwTerminate();
	return 0;
}


void mouseMoveCallBack(GLFWwindow* window, double xPosIn, double yPosIn) {
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

	camera.ProcessMouseMovement(xOffset, yOffset); // ��������ĳ���
}


void mouseScrollCallBack(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void windowSizeChangeCallback(GLFWwindow* window, int newWidth, int newHeight) {
	glViewport(0, 0, newWidth, newHeight);
}
