// ��ϰ3: �����ھ�����ֻ��ʾ����ͼ����м�һ���֣��޸��������꣬�ﵽ�ܿ������������ص�Ч����
// ����ʹ��GL_NEAREST��������˷�ʽ��������ʾ�ø�����
// ---- ���ĸ�����������������õľ����ܽӽ�

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>

#include <learn_opengl/shader_class.h>
#include <learn_opengl/call_backs.h>

#include <iostream>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

const char* vertexCodePath = "./src/chapter1/4_texture/1_shader_code.vertex";
const char* fragmentCodePath = "./src/chapter1/4_texture/2_shader_code.fragment";
const char* imagePath = "./resources/container.jpg";
const char* imagePath2 = "./resources/awesomeface.png";

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
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.55f, 0.55f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.55f, 0.45f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.45f, 0.45f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.45f, 0.55f  // top left 
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

	unsigned int texture, texture2;

	// ==== ��һ������
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
		cout << "ͼƬ1����ʧ�� " << endl;
	}
	stbi_image_free(imageData);

	// �ڶ�������
	glGenTextures(1, &texture2); // �����������
	glBindTexture(GL_TEXTURE_2D, texture2); // �󶨵�2d����, ֮���2D���������Ĳ�������Ӱ���������
	// ����������ظ���ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// ���ù��˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// ����ͼƬ, ��������ͼ��  ���ɶ༶��Զ����
	int width2, height2, nrChannels2;
	stbi_set_flip_vertically_on_load(true); // ����ͼƬʱy��ߵ�
	unsigned char* imageData2 = stbi_load(imagePath2, &width2, &height2, &nrChannels2, 0);
	if (imageData2) {
		// ����ͼƬ��������ͼ��, �ڶ���ͼƬ��png��ʽ, ��Ҫָ��͸��ͨ��
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData2);
		// ���ɶ༶��Զ����
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "ͼƬ2����ʧ�� " << endl;
	}
	stbi_image_free(imageData2);

	// ͨ��uniform����Ƭ����ɫ���е�����sampler2D, ʹ���Ӧ����Ԫ0 ������Ԫ1
	// openglһ����0 - 15������Ԫ
	// �Ժ�����Ⱦѭ����bind�������ǰ�ֱ𼤻�����Ԫ,ʹ�ø�����Ԫ֪������������Щ����
	myShader.use(); // �������ȼ���
	myShader.setInt("texture1", 0); // ע����������ɫ�������е�sampler2D
	glUniform1i(glGetUniformLocation(myShader.ID, "texture2"), 1);

	while (!glfwWindowShouldClose(window))
	{
		processInupt(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture); // ע��������c++�����е��������

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

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



