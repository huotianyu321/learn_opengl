// 局部空间 -> 模型矩阵 -> 世界空间 -> 观察矩阵 -> 观察空间 -> 投影矩阵 -> 裁剪空间

// opengl 右手坐标系

// 使用model矩阵旋转箱子
// 使用view矩阵移动箱子（模拟相机移动）
// 使用projection矩阵营造透视效果

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

int main() {

	GLFWwindow* window = initAndCreateWindow(1800, 1200, "6.1");
	if (window == nullptr) {
		glfwTerminate();
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, framebufferSizeChangeCallback);

	// 顶点数据
	// NDC坐标系范围为[-1, 1]
	// 纹理坐标范围为[0, 1]
	float vertices[] = {
		// 位置            // 纹理坐标
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f,  -0.5f, 0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	unsigned int location[] = { 0, 1 };
	unsigned int dimension[] = { 3, 2 };
	unsigned int stride[] = { 5, 5 };
	unsigned int offset[] = { 0, 3 };
	unsigned int VAO = set_VAO_VBO_EBO_mutiple(
		vertices, sizeof(vertices), indices, sizeof(indices),
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
	Shader shader(vertexCodePath, fragmentCodePath);
	// 将纹理采样器绑定到纹理单元
	shader.use();
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);

	// 渲染循环
	while (!glfwWindowShouldClose(window)) {
		jobAtRenderLoopStart(window);

		// 绑定纹理
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		// 激活着色器
		shader.use();

		// 创建变换矩阵
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;

		double t = glfwGetTime();
		float angle = (sin(t) + 1) * 360;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f)); // 绕x轴旋转
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); // 沿z轴负平移(模拟相机沿z轴正方向平移)
		projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f); // 透视投影

		// 传递变换矩阵到着色器
		shader.setMat4("model", model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection); // 透视投影矩阵

		// 绘制矩形
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		jobBeforRenderLoopEnd(window);
	}

	doClearJob(window, &VAO, nullptr, nullptr);
	glDeleteTextures(1, &texture1);
	glDeleteTextures(1, &texture2);

	glfwTerminate();
	return 0;
}


