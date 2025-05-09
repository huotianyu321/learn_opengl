// ������������
// openglһ����0 - 15������Ԫ GL_TEXTURE0 - GL_TEXTURE15
// ͨ������Ԫ����м�� ����Ƭ����ɫ����sampler2D �ʹ����е��������
// ͨ��glUiform1iΪsampler2D����һ��ֵ, ����uniform����������Ԫ
// ͨ����BindTextureǰActiveTexture(GL_TEXTURE0), ��������Ԫ���������
// ����Ԫ���ڵ�Ŀ����Ϊ����һ��Ƭ����ɫ���п���ʹ�ö������
// GL_TEXTUER0��Ĭ�ϼ����, ֻ��һ������ʱ��������Щ����


#include <HEADER/utils.hpp>
#include <HEADER/call_backs.hpp>
#include <HEADER/shader_class.hpp>
#include <HEADER/set_uniform.hpp>

#include <iostream>

using namespace std;
int main() {

	const char* vertexCodePath = "./src/chapter1/4_texture/1_code.vs";
	const char* fragmentCodePath = "./src/chapter1/4_texture/2_code.fs";
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

	// ͨ��uniform����Ƭ����ɫ���е�����sampler2D, ʹ��ֱ������Ԫ0������Ԫ1�в���
	// openglһ����0 - 15������Ԫ
	// �Ժ�����Ⱦѭ����bind�������ǰ�ֱ𼤻�����Ԫ,ʹ�ø�����Ԫ���������
	myShader.use(); 
	myShader.setInt("texture1", 0); // ע����������ɫ�������е�sampler2D
	set1int(myShader.ID, "texture2", 1);

	while (!glfwWindowShouldClose(window))
	{
		jobAtRenderLoopStart(window);

		// ���������1�󶨵�����Ԫ0��
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1); 
		// ���������2�󶨵�����Ԫ1��
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		myShader.use(); 
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		jobBeforRenderLoopEnd(window);
	}

	doClearJob(&VAO, nullptr, nullptr);

	glfwTerminate();
	return 0;
}



