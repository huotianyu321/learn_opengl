// ʹ����ת��������ž���

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
		// ��������          // ��ɫ           // ��������
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	// ��������
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

	// ��һ������
	unsigned int texture1 = createTexture(
		imagePath1,
		GL_REPEAT, GL_REPEAT,
		GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR,
		false,
		false
	);
	// �ڶ�������
	unsigned int texture2 = createTexture(
		imagePath2,
		GL_MIRRORED_REPEAT, GL_REPEAT,
		GL_LINEAR, GL_LINEAR,
		true,
		true
	);

	glm::mat4 transform; // ��ʱ��һ��4x4�ĵ�λ����
	printMat4(transform);
	// ������z����ת90��ı任����
	transform = glm::rotate(
		transform,
		glm::radians(90.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);
	printMat4(transform);
	// ��ǰһ���Ļ�����, ����ʹxyz������0.5�ı任����
	transform = glm::scale(transform, glm::vec3(0.5, 0.5, 0.5));
	printMat4(transform);

	myShader.use();
	myShader.setInt("texture1", 0); // ע����������ɫ�������е�sampler2D
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



