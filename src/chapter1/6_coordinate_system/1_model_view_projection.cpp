// �ֲ��ռ� -> ģ�;��� -> ����ռ� -> �۲���� -> �۲�ռ� -> ͶӰ���� -> �ü��ռ�

// opengl ��������ϵ

// ʹ��model������ת����
// ʹ��view�����ƶ����ӣ�ģ������ƶ���
// ʹ��projection����Ӫ��͸��Ч��

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

	// ��������
	// NDC����ϵ��ΧΪ[-1, 1]
	// �������귶ΧΪ[0, 1]
	float vertices[] = {
		// λ��            // ��������
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

	// ��һ������
	unsigned int texture1 = createTexture(
		texture1_path,
		GL_REPEAT, GL_REPEAT,
		GL_LINEAR, GL_LINEAR,
		false,
		false
	);
	// �ڶ�������
	unsigned int texture2 = createTexture(
		texture2_path,
		GL_MIRRORED_REPEAT, GL_REPEAT,
		GL_LINEAR, GL_LINEAR,
		true,
		true
	);

	// ��ɫ��
	Shader shader(vertexCodePath, fragmentCodePath);
	// ������������󶨵�����Ԫ
	shader.use();
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);

	// ��Ⱦѭ��
	while (!glfwWindowShouldClose(window)) {
		jobAtRenderLoopStart(window);

		// ������
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		// ������ɫ��
		shader.use();

		// �����任����
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;

		double t = glfwGetTime();
		float angle = (sin(t) + 1) * 360;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f)); // ��x����ת
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); // ��z�Ḻƽ��(ģ�������z��������ƽ��)
		projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f); // ͸��ͶӰ

		// ���ݱ任������ɫ��
		shader.setMat4("model", model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection); // ͸��ͶӰ����

		// ���ƾ���
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


