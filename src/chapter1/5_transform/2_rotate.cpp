// 使用旋转矩阵和缩放矩阵

#include <HEADER/utils.hpp>
#include <HEADER/call_backs.hpp>
#include <HEADER/shader_class.hpp>
#include <HEADER/set_uniform.hpp>
#include <HEADER/process_events.hpp>

#include <iostream>

int main() {
	const char* vertexCodePath = "./src/chapter1/5_transform/2_code.vs";
	const char* fragmentCodePath = "./src/chapter1/5_transform/2_code.fs";
	const char* imagePath1 = "./resources/container.jpg";
	const char* imagePath2 = "./resources/awesomeface.png";

	GLFWwindow* window = initAndCreateWindow(1800, 1200, "5.2");
	if (window == nullptr) {
		glfwTerminate();
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, framebufferSizeChangeCallback);

	Shader myShader(vertexCodePath, fragmentCodePath);

	float vertices[] = {
		// 顶点坐标          // 颜色           // 纹理坐标
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	// 顶点索引
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	unsigned int location[] = { 0, 1, 2 };
	unsigned int dimension[] = { 3, 3, 2 };
	unsigned int stride[] = { 8, 8, 8 };
	unsigned int offset[] = { 0, 3, 6 };
	unsigned int VAO = set_VAO_VBO_EBO_mutiple(
		vertices, sizeof(vertices), indices, sizeof(indices),
		3, location, dimension, stride, offset
	);

	// 第一个纹理
	unsigned int texture1 = createTexture(
		imagePath1,
		GL_REPEAT, GL_REPEAT,
		GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR,
		false,
		false
	);
	// 第二个纹理
	unsigned int texture2 = createTexture(
		imagePath2,
		GL_MIRRORED_REPEAT, GL_REPEAT,
		GL_LINEAR, GL_LINEAR,
		true,
		true
	);

	glm::mat4 transform; // 此时是一个4x4的单位矩阵
	printMat4(transform);
	// 生成绕z轴旋转90°的变换矩阵
	transform = glm::rotate(
		transform,
		glm::radians(90.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);
	printMat4(transform);
	// 在前一步的基础上, 生成使xyz各缩放0.5的变换矩阵
	transform = glm::scale(transform, glm::vec3(0.5, 0.5, 0.5));
	printMat4(transform);

	myShader.use();
	myShader.setInt("texture1", 0); // 注意这里是着色器代码中的sampler2D
	set1int(myShader.ID, "texture2", 1);
	set1mat4(myShader.ID, "transform", transform);

	while (!glfwWindowShouldClose(window))
	{

		jobAtRenderLoopStart(window);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		myShader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		jobBeforRenderLoopEnd(window);
	}


	doClearJob(window, &VAO, nullptr, nullptr);

	glfwTerminate();
	return 0;
}



