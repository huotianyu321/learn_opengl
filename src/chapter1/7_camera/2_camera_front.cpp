// wasd控制, keybord事件
// 体会一下定义cameraFront这个向量的便利
// cameraFront 与 “相机坐标系的z轴在世界坐标系中的向量表示” 相反向
// cameraFront“以相机的世界位置为原点”指向相机看向的“目标点在世界坐标系的位置”
// 通过相机的世界坐标cameraPos和cameraFront很容易计算出目标点的世界坐标(cameraPos+cameraFront)
// 其次，通过cameraFront和worldUp可以计算出cameraRight
// 这个练习中cameraFront始终是(0, 0, -1)，相机始终看向-z
// 下个练习中，当相机的欧拉角变化时，可以通过欧拉角计算出cameraFront

// 注意worldUp是相机所在世界的上向量，不是相机坐标系的y轴正方向（当俯仰角变化时，相机坐标系的y轴正方向与worldUp不同）

#include <HEADER/utils.hpp>
#include <HEADER/call_backs.hpp>
#include <HEADER/shader_class.hpp>
#include <HEADER/set_uniform.hpp>
#include <HEADER/process_events.hpp>

#include <iostream>

const char* texture1_path = "./resources/container.jpg";
const char* texture2_path = "./resources/awesomeface.png";
const char* vertexCodePath = "./src/chapter1/6_coordinate_system/1_code.vs";
const char* fragmentCodePath = "./src/chapter1/6_coordinate_system/1_code.fs";

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrameTime = 0.0f; // 上一帧的时间

int main() {
	const int WIDTH = 1800;
	const int HEIGHT = 1200;

	GLFWwindow* window = initAndCreateWindow(WIDTH, HEIGHT, "6.2");
	if (window == nullptr) {
		glfwTerminate();
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, framebufferSizeChangeCallback);

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
	unsigned int VAO = set_VAO_VBO_EBO_mutiple(
		vertices, sizeof(vertices), nullptr, 0,
		3, location, dimension, stride, offset
	);

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

	// 投影矩阵其实很少发生变化, 一般在窗口大小改变时才会改变, 可以不在每帧都传递
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f); // 透视投影
	ourShader.setMat4("projection", projection); // 透视投影矩阵

	// 开启深度测试， 如果不开启，可以看到立方体在旋转过程中穿模
	jobBeforeEnterRenderLoop(window);
	// 渲染循环
	while (!glfwWindowShouldClose(window)) {
		float t = glfwGetTime();
		float currentFrameTime = t;
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		jobAtRenderLoopStart(window);
		processWASD(window, deltaTime, cameraPos, cameraFront, worldUp);

		// 绑定纹理
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		// 激活着色器
		ourShader.use();

		// 创建观察矩阵
		glm::mat4 view;
		view = glm::lookAt(
			cameraPos, // eye
			cameraPos + cameraFront, // center
			//glm::vec3(cameraFront.x * 0.1, cameraFront.y * 0.1, cameraFront.z * 0.1), // 测试用
			worldUp // worldup
		);
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

			// 绘制矩形
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36); // 绘制立方体
		}

		jobBeforRenderLoopEnd(window);
	}

	// 执行清理工作
	return 0;
}

