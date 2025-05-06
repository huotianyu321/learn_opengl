// 使用out关键字把顶点位置输出到片段着色器，并将片段的颜色设置为与顶点位置相等（来看看连顶点位置值都在三角形中被插值的结果）
// 为什么左半边是黑的： 因为坐下角坐标是-0.5，-0.5，0； 当负数被设置为颜色时会被处理为0， 在-0.5 - 0区间，片段插值的结果都是0，所以左下方是黑色的

#include <HEADER/utils.hpp>
#include <HEADER/call_backs.hpp>
#include <HEADER/shader_class.hpp>
#include <iostream>

const char* vertexCodePath = "./src/chapter1/3_shader/exercise3_code.vs";
const char* fragmentCodePath = "./src/chapter1/3_shader/exercise3_code.fs";

int main()
{
	GLFWwindow* window = initAndCreateWindow(1800, 1200, "3-exercise3");
	if (NULL == window) {
		glfwTerminate();
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, framebufferSizeChangeCallback);

	Shader* myShader = new Shader(vertexCodePath, fragmentCodePath);

	float vertices[] = {
		0.5f,  0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
	   -0.5f, -0.5f, 0.0f,
	};
	unsigned int VAO = set_VAO_VBO_EBO(
		vertices,
		sizeof vertices,
		nullptr,
		0,
		0,
		3,
		3,
		0
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


