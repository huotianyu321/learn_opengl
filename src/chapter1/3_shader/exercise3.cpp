// ʹ��out�ؼ��ְѶ���λ�������Ƭ����ɫ��������Ƭ�ε���ɫ����Ϊ�붥��λ����ȣ�������������λ��ֵ�����������б���ֵ�Ľ����
// Ϊʲô�����Ǻڵģ� ��Ϊ���½�������-0.5��-0.5��0�� ������������Ϊ��ɫʱ�ᱻ����Ϊ0�� ��-0.5 - 0���䣬Ƭ�β�ֵ�Ľ������0���������·��Ǻ�ɫ��

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


