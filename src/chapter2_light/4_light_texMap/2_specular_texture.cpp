// Ƭ����ɫ�� ��������ͼ
// ʹ����������ͼ������ʽṹ���е���������ɫ�ͻ�������ɫ��ʹ��ͬƬ���и���ʵ����ɫ
// ����һ�������У�ֻʹ����������ͼ�ᷢ�� �м��ľͷҲ�������Եľ���߹⣨��ʱmaterial��specularд����1.0 1.0 1.0��
// �ټ�һ�ž������ͼ��ʹ��ͬƬ���и���ʵ�ľ���߹�

#include <HEADER/utils.hpp>
#include <HEADER/shader_class.hpp>
#include <HEADER/set_uniform.hpp>
#include <HEADER/process_events.hpp>
#include <HEADER/camera_class.hpp>

#include <iostream>

typedef unsigned int uint;

const int WIDTH = 1800;
const int HEIGHT = 1200;
const char* boxVertexCodePath = "./src/chapter2_light/4_light_texMap/1_box_vs.txt";
const char* boxFragmentCodePath = "./src/chapter2_light/4_light_texMap/2_box_fs.txt";
const char* lightFragmentCodePath = "./src/chapter2_light/2_basic_lighting/1_light_fs.txt";
const char* lightVertexCodePath = "./src/chapter2_light/2_basic_lighting/1_light_vs.txt";
const char* diffuseTexMapPath = "./resources/container2.png";
const char* specularTexMapPath = "./resources/container2_specular.png";

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

	// ǰ3���Ƕ���λ�ã��м������Ƿ������� ����������������
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

	// ��Դλ��
	glm::vec3 lightPos(0.0f, 0.2f, 2.0f);

	uint diffuseTexMap = createTexture(diffuseTexMapPath, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR, true, false);
	uint specularTexMap = createTexture(specularTexMapPath, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR, true, false);

	// ������ɫ��(Ӧ�����������ϣ�
	Shader boxShader(boxVertexCodePath, boxFragmentCodePath);
	boxShader.use();
	// ��Щû�иı�����ͷ���loop�����
	boxShader.set3Float("light.position", lightPos.x, lightPos.y, lightPos.z);
	boxShader.set3Float("light.ambient", 0.2f, 0.2f, 0.2f);
	boxShader.set3Float("light.diffuse", 0.5f, 0.5f, 0.5f);
	boxShader.set3Float("light.specular", 1.0f, 1.0f, 1.0f);
	boxShader.setFloat("material.shininess", 32.0f);
	boxShader.setInt("matierial.diffuse", 0); // ����������ͼ����������󶨵�����Ԫ0
	boxShader.setInt("material.specular", 1); // �����淴����ͼ����������󶨵�����Ԫ1

	// ��Դ��ɫ��
	Shader lightShader(lightVertexCodePath, lightFragmentCodePath);
	lightShader.use();
	lightShader.set3Float("lightColor", 1.0f, 1.0f, 1.0f);

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

		// ����box
		boxShader.use();
		boxShader.setMat4("projection", projection);
		boxShader.setMat4("view", view);
		boxShader.setMat4("model", boxModel);
		boxShader.set1Vec3("viewPos", camera.Position);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseTexMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularTexMap);
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
