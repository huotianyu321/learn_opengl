// ������ ʹ����������ΪƬ�λ�ȡ������ɫ
// �����Ʒ�ʽ  
// ������˷�ʽ �������������������꣬OpenGLʹ����������ȥ����ͼ���ϲ������أ����в�����ȡ�������ص���ɫ��
// �༶��Զ���� ��Զ��С������ʹ�ø߷ֱ�������Ľ������

// ע������ͼ������ϵ��NDC����ϵ�Ĳ���
// ����������x��y���ϣ���ΧΪ0��1֮��

// ����������� - ��ĳ������ - ���ò��� - ����ͼƬ,��������ͼ��,�ͷ�ͼƬ
// Ƭ����ɫ�� sampler2D ���� ���������, ��������texture(textureSampler, textureCoord)

#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>

#include <learn_opengl/shader_class.h>
#include <learn_opengl/call_backs.h>

#include <iostream>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

const char* vertexCodePath = "./src/chapter1/4_texture/1_shader_code.vertex";
const char* fragmentCodePath = "./src/chapter1/4_texture/1_shader_code.fragment";
const char* imagePath = "./resources/container.jpg";

using namespace std;
int main() {
	//
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "learn_texture", NULL, NULL);
	if (window == NULL) {
		cout << "��������ʧ��" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// === ��������OpenGL����ָ��
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "��ʼ��GLADʧ��" << endl;
		return -1;
	}

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

	// ====
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// ===�����ص� �������
	unsigned int texture;
	glGenTextures(1, &texture); // �����������
	glBindTexture(GL_TEXTURE_2D, texture); // �󶨵�2d����, ֮���2D���������Ĳ�������Ӱ���������
	// ����������ظ���ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// ���ù��˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// ����ͼƬ, ��������ͼ��  ���ɶ༶��Զ����
	int width, height, nrChannels;
	unsigned char* imageData = stbi_load(imagePath, &width, &height, &nrChannels, 0);
	if (imageData) {
		// ����ͼƬ��������ͼ��
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
		// ���ɶ༶��Զ����
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "ͼƬ����ʧ�� " << endl;
	}
	stbi_image_free(imageData);

	while (!glfwWindowShouldClose(window))
	{
		processInupt(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, texture);

		myShader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}



