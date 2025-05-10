//使用一个uniform变量作为mix函数的第三个参数来改变两个纹理可见度，使用上和下键来改变箱子或笑脸的可见度

#include <HEADER/utils.hpp>
#include <HEADER/call_backs.hpp>
#include <HEADER/shader_class.hpp>
#include <HEADER/set_uniform.hpp>
#include <HEADER/process_events.hpp>

#include <iostream>

using namespace std;
int main() {

	const char* vertexCodePath = "./src/chapter1/4_texture/1_code.vs";
	const char* fragmentCodePath = "./src/chapter1/4_texture/exercise_4_code.fs";
	const char* imagePath1 = "./resources/container.jpg";
	const char* imagePath2 = "./resources/awesomeface.png";

	GLFWwindow* window = initAndCreateWindow(1800, 1200, "4.2");
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

	// 通过uniform设置片段着色器中的两个sampler2D, 使其分别从纹理单元0和纹理单元1中采样
	// opengl一般有0 - 15个纹理单元
	// 稍后在渲染循环中bind纹理对象前分别激活纹理单元,使得各纹理单元与纹理对象
	myShader.use();
	myShader.setInt("texture1", 0); // 注意这里是着色器代码中的sampler2D
	set1int(myShader.ID, "texture2", 1);

	float number = 50.0f; // 混合比例
	while (!glfwWindowShouldClose(window))
	{
		jobAtRenderLoopStart(window);

		processUpDownKey(window, &number);

		// 将纹理对象1绑定到纹理单元0上
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		// 将纹理对象2绑定到纹理单元1上
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		myShader.use();
		set1float(myShader.ID, "percent", number);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		jobBeforRenderLoopEnd(window);
	}

	doClearJob(&VAO, nullptr, nullptr);

	glfwTerminate();
	return 0;
}



