// 聚光灯 - 实现一个第一人称视角的聚光灯（手电筒）- 添加外圆锥实现平滑柔滑边缘的效果
// 光强计算公式 I = (theta - gamma) / (fi - gamma) , 其中theta是片段至光源位置方向向量和光源方向的反方向方向向量的夹角余弦值， fi是内切光角余弦值， gama是外切光角余弦值
// 当theta <= gamma, 说明片段在外切光角以外，取0
// 当gamma < theta < fi, 在内外切光角中间，光强为 0 - 1
// 当theta >= fi, 在内切光角以内，取1

#include <HEADER/utils.hpp>
#include <HEADER/shader_class.hpp>
#include <HEADER/set_uniform.hpp>
#include <HEADER/process_events.hpp>
#include <HEADER/camera_class.hpp>

#include <iostream>

typedef unsigned int uint;

const int WIDTH = 1800;
const int HEIGHT = 1200;
const char* boxVertexCodePath = "./src/chapter2_light/5_light_caster/1_box_vs.txt";
const char* boxFragmentCodePath = "./src/chapter2_light/5_light_caster/4_box_fs.txt";
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

	// 光源位置
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

	uint diffuseTexMap = createTexture(diffuseTexMapPath, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR, true, false);
	uint specularTexMap = createTexture(specularTexMapPath, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR, true, false);

	// 光照着色器(应用在立方体上）
	Shader boxShader(boxVertexCodePath, boxFragmentCodePath);
	boxShader.use();
	// 这些没有改变过，就放在loop外边了
	boxShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f))); // 内切光角
	boxShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f))); // 外切光角 
	boxShader.set3Float("light.ambient", 0.2f, 0.2f, 0.2f);
	boxShader.set3Float("light.diffuse", 0.5f, 0.5f, 0.5f);
	boxShader.set3Float("light.specular", 1.0f, 1.0f, 1.0f);

	boxShader.setFloat("material.shininess", 32.0f);
	boxShader.setInt("matierial.diffuse", 0); // 将漫反射贴图纹理采样器绑定到纹理单元0
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

		// 绘制box
		boxShader.use();
		boxShader.setMat4("projection", projection);
		boxShader.setMat4("view", view);
		boxShader.set1Vec3("viewPos", camera.Position);
		boxShader.set1Vec3("light.position", camera.Position);
		boxShader.set1Vec3("light.direction", camera.Front);
		boxShader.setFloat("light.constant", 1.0f);
		boxShader.setFloat("light.linear", 0.014f);
		boxShader.setFloat("light.quadratic", 0.0007f);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseTexMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularTexMap);
		glBindVertexArray(boxVaoData.VAO);

		for (uint i = 0; i < 10; ++i) {
			// box 模型矩阵
			glm::mat4 boxModel;
			boxModel = glm::translate(boxModel, cubePositions[i]);
			float angle = 20.0f * i;
			boxModel = glm::rotate(boxModel, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			boxShader.setMat4("model", boxModel);

			glDrawArrays(GL_TRIANGLES, 0, 36); // 绘制立方体
		}

		// 光源 模型矩阵
		glm::mat4 lightModel;
		lightModel = glm::translate(lightModel, lightPos);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f));

		// 绘制光源立方体
		//lightShader.use();
		//lightShader.setMat4("projection", projection);
		//lightShader.setMat4("view", view);
		//lightShader.setMat4("model", lightModel);
		//glBindVertexArray(lightVaoData.VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36); // 绘制光源

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
