// ����һ�����ӣ�����һ����Դ

#include <HEADER/utils.hpp>
#include <HEADER/shader_class.hpp>
#include <HEADER/set_uniform.hpp>
#include <HEADER/process_events.hpp>
#include <HEADER/camera_class.hpp>

#include <iostream>

const int WIDTH = 1800;
const int HEIGHT = 1200;
const char* vertexCodePath = "./src/chapter2_light/1_color/1_code.vs";
const char* boxfragmentCodePath = "./src/chapter2_light/1_color/1_code_box.fs";
const char* lightfragmentCodePath = "./src/chapter2_light/1_color/1_code_light.fs";

Camera camera = Camera(
	glm::vec3(0.0f, 0.0f, 5.0f), 
	glm::vec3(0.0f, 1.0f, 0.0f), 
	-90.0f, 
	0.0f
);

float lastX = 0; 
float lastY = 0;
float deltaTime = 0.0f;
float lastFrameTime = 0.0f;
bool mouseControlActive = false; // ������Ƶ����������ټ���

// ��Դλ��
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

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

	// ������Ķ��㣬 ÿ�������������Σ�ÿ��������3������ 6 * 2 * 3
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};
	VAOData boxVaoData = set_VAO_VBO_EBO(
		vertices, sizeof(vertices), nullptr, 0,
		 0, 3, 3, 0
	);
	VAOData lightVaoData = set_VAO_VBO_EBO(
		vertices, sizeof(vertices), nullptr, 0,
		0, 3, 3, 0
	);

	// ������ɫ��(Ӧ�����������ϣ�
	Shader boxShader(vertexCodePath, boxfragmentCodePath); 
	// ��Դ��ɫ��
	Shader lightShader(vertexCodePath, lightfragmentCodePath);

	jobBeforeEnterRenderLoop(window);
	// ��Ⱦѭ��
	while (!glfwWindowShouldClose(window)) {
		float t = glfwGetTime();
		float currentFrameTime = t;
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		jobAtRenderLoopStart(window);
		processWASD(window, camera, deltaTime);

		// ͸��ͶӰ����
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f); // ͸��ͶӰ

		// �۲����
		glm::mat4 view = camera.GetViewMatrix();

		// box ģ�;���
		glm::mat4 boxModel;

		// ����box  lightColor * objectColor -> ������ɫ
		boxShader.use();
		boxShader.setMat4("projection", projection);
		boxShader.setMat4("view", view);
		boxShader.setMat4("model", boxModel);
		boxShader.set3Float("lightColor", 1.0f, 1.0f, 1.0f);
		boxShader.set3Float("objectColor", 1.0f, 0.5f, 0.31f);

		glBindVertexArray(boxVaoData.VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36); // ����������

		// ��Դ ģ�;���
		glm::mat4 lightModel;
		lightModel = glm::translate(lightModel, lightPos); 
		lightModel = glm::scale(lightModel, glm::vec3(0.2f)); 

		// ���ƹ�Դ������
		lightShader.use();
		lightShader.setMat4("projection", projection);
		lightShader.setMat4("view", view);
		lightShader.setMat4("model", lightModel);
		glBindVertexArray(lightVaoData.VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36); // ���ƹ�Դ

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
	// ��ǰ֡�����λ��
	float xPos = static_cast<float>(xPosIn);
	float yPos = static_cast<float>(yPosIn);
	std::cout << "X: " << xPos << " Y: " << yPos << std::endl;

	// ����������ƶ��������ټ������
	if (
		!mouseControlActive
		&& xPos >= WIDTH / 2 - 10
		&& xPos <= WIDTH / 2 + 10
		&& yPos >= HEIGHT / 2 - 10
		&& yPos <= HEIGHT / 2 + 10
		) {
		mouseControlActive = true; // ����Ƶ���������ʱ����
		lastX = xPos;
		lastY = yPos;
	}

	if (!mouseControlActive) {
		return;
	}

	// ��������ƶ��ľ���
	float xOffset = xPos - lastX;
	// ����y�ᣬ��Ļ����ϵ�������Ͻ�Ϊԭ���. 
	// ����������ƶ�ʱ��y�������󣬵���ʱ������Ӧ�ü�С
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
