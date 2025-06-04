// ʹ��ģ�;���ֻ����3������������ת���Լ���1�����ӣ�������ʣ�µ����ӱ��־�ֹ

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learn_opengl/shader_class.h>
#include <learn_opengl/call_backs.h>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

const char* texture1_path = "./resources/container.jpg";
const char* texture2_path = "./resources/awesomeface.png";
const char* vertexCodePath = "./src/chapter1/6_coordinate_system/1_shader.vertex";
const char* fragmentCodePath = "./src/chapter1/6_coordinate_system/1_shader.fragment";

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "COORDINATE SYSTEM", NULL, NULL);
	if (window == NULL) {
		std::cout << "��������ʧ��" << std::endl;
		glfwTerminate();
		return -1;
	}
	// ���ô��ڵ�������
	glfwMakeContextCurrent(window);
	// �󶨴��ڴ�С�ı�Ļص�����
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// ��ʼ��GLAD 
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "����GLADʧ��" << std::endl;
		return -1;
	}

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	// VAO, VBO
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture
	unsigned int texture1, texture2;
	// ��һ������
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// ���������Ʒ�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// ����������˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// ��������ͼƬ,�������������༶��Զ����
	int width, height, nrChannels;
	//stbi_set_flip_vertically_on_load(true); // ��תy��
	unsigned char* data = stbi_load(texture1_path, &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "�������ʧ��" << std::endl;
	}
	stbi_image_free(data);

	// �ڶ�������
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// ���������Ʒ�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// ����������˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// ��������ͼƬ, �������������༶��Զ����
	unsigned char* data2 = stbi_load(texture2_path, &width, &height, &nrChannels, 0);
	if (data2) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "�������ʧ��" << std::endl;
	}
	stbi_image_free(data2);

	// ��ɫ��
	Shader ourShader(vertexCodePath, fragmentCodePath);
	// ������������󶨵�����Ԫ
	ourShader.use();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);

	// ����ʮ��������,ͨ��model����ʵ�ֲ�ͬλ�õ�������
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// ������Ȳ���
	glEnable(GL_DEPTH_TEST);
	// ��Ⱦѭ��
	while (!glfwWindowShouldClose(window)) {
		processInupt(window);

		// �����ɫ������
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// =====�����Ȼ�����====
		glClear(GL_DEPTH_BUFFER_BIT);

		// ������
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		// ������ɫ��
		ourShader.use();

		// �����任����
		glm::mat4 view;
		glm::mat4 projection;
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); // ��z�Ḻƽ��(ģ�������z��������ƽ��)
		projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f); // ͸��ͶӰ
		unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
		// ͶӰ������ʵ���ٷ����仯, һ���ڴ��ڴ�С�ı�ʱ�Ż�ı�, ���Բ���ÿ֡������
		ourShader.setMat4("projection", projection); // ͸��ͶӰ����

		// ͨ��ģ�;�����ѭ���иı��������λ��(ʹ����ͬ�Ķ�������)
		for (unsigned int i = 0; i < 10; ++i) {
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]); // ƽ��

			float angle = 20.0f * i; // ��ת�Ƕ�

			if (i % 3 == 0) {
				angle = glfwGetTime() * 25.0f;
			}

			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));


			// ���ݱ任������ɫ��
			unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			// ���ƾ���
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36); // ����������
		}

		// ����������, �����¼�
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDisable(GL_DEPTH_TEST); // �ر���Ȳ���

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	glDeleteTextures(1, &texture1);
	glDeleteTextures(1, &texture2);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


