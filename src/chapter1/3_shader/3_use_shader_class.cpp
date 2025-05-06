// 将shader封装到一个类中

#include <HEADER/utils.hpp>
#include <HEADER/call_backs.hpp>
#include <HEADER/shader_class.hpp>
#include <iostream>

const char* vertexCodePath = "./src/chapter1/3_shader/3_code.vs";
const char* fragmentCodePath = "./src/chapter1/3_shader/3_code.fs";

int main()
{
	GLFWwindow* window = initAndCreateWindow(1800, 1200, "3.3 shader class");
	if (NULL == window) {
		glfwTerminate();
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, framebufferSizeChangeCallback); // 注册窗口变化回调

	Shader* myShader = new Shader(vertexCodePath, fragmentCodePath);

	float vertices[] = {
		// 三角形, 每行前三个为顶点位置，后三个为顶点颜色
		0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
	   -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	unsigned int location[] = { 0 ,1 };
	unsigned int dimension[] = { 3, 3 };
	unsigned int stride[] = { 6, 6 };
	unsigned int offset[] = { 0, 3 };

	unsigned int VAO = set_VAO_VBO_EBO_mutiple(vertices,
		sizeof vertices,
		nullptr,
		0,
		2,
		location,
		dimension,
		stride,
		offset
	);


	// ===
	while (!glfwWindowShouldClose(window)) {
		jobAtRenderLoopStart(window);

		myShader->use();
		glBindVertexArray(VAO); 
		glDrawArrays(GL_TRIANGLES, 0, 3); 

		jobBeforRenderLoopEnd(window);
	}

	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;

}


