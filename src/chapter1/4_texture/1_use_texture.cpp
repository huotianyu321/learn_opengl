// ������ ʹ����������ΪƬ�λ�ȡ������ɫ
// �����Ʒ�ʽ  
// ������˷�ʽ �������������������꣬OpenGLʹ����������ȥ����ͼ���ϲ������أ����в�����ȡ�������ص���ɫ��
// �༶��Զ���� ��Զ��С������ʹ�ø߷ֱ�������Ľ������

// ע������ͼ������ϵ��NDC����ϵ�Ĳ���
// ����������x��y���ϣ���ΧΪ0��1֮��

// ����������� - ��ĳ������ - ���ò��� - ����ͼƬ,��������ͼ��,�ͷ�ͼƬ
// Ƭ����ɫ�� sampler2D ���� ���������, ��������texture(textureSampler, textureCoord)

#include <HEADER/utils.hpp>
#include <HEADER/call_backs.hpp>
#include <HEADER/shader_class.hpp>

#include <iostream>

int main() {
	const char* vertexCodePath = "./src/chapter1/4_texture/1_code.vs";
	const char* fragmentCodePath = "./src/chapter1/4_texture/1_code.fs";
	const char* imagePath = "./resources/container.jpg";

	GLFWwindow* window = initAndCreateWindow(1800, 1200, "4.1");

	// ������ɫ������,������ɫ��;������ɫ������,������ɫ������
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

	unsigned int texture = createTexture(
		imagePath,
		GL_REPEAT, GL_REPEAT,
		GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR
	);

	while (!glfwWindowShouldClose(window))
	{
		jobAtRenderLoopStart(window);

		glBindTexture(GL_TEXTURE_2D, texture);
		myShader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		jobBeforRenderLoopEnd(window);
	}

	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}



