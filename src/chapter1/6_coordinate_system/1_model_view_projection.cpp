// 局部空间 -> 模型矩阵 -> 世界空间 -> 观察矩阵 -> 观察空间 -> 投影矩阵 -> 裁剪空间

// opengl 右手坐标系

// 使用model矩阵旋转箱子
// 使用view矩阵移动箱子（模拟相机移动）
// 使用projection矩阵营造透视效果

#define STB_IMAGE_IMPLEMENTATION

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
		std::cout << "创建窗口失败" << std::endl;
		glfwTerminate();
		return -1;
	}
	// 设置窗口的上下文
	glfwMakeContextCurrent(window);
	// 绑定窗口大小改变的回调函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// 初始化GLAD 
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "加载GLAD失败" << std::endl;
		return -1;
	}

	// 顶点数据
	// NDC坐标系范围为[-1, 1]
	// 纹理坐标范围为[0, 1]
	float vertices[] = {
		// 位置            // 纹理坐标
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f,  -0.5f, 0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	// VAO, VBO, EBO
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 设置顶点属性
	// 第一个参数是顶点属性的索引(location)
	// 第二个参数是每个顶点属性的大小
	// 第三个参数是数据类型
	// 第四个参数是是否归一化
	// 第五个参数是步长
	// 第六个参数是偏移量
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture
	unsigned int texture1, texture2;
	// 第一个纹理
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// 设置纹理环绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 设置纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 加载纹理图片,生成纹理并创建多级渐远纹理
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // 翻转y轴
	unsigned char* data = stbi_load(texture1_path, &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "纹理加载失败" << std::endl;
	}
	stbi_image_free(data);

	// 第二个纹理
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// 设置纹理环绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 设置纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 加载纹理图片, 生成纹理并创建多级渐远纹理
	unsigned char* data2 = stbi_load(texture2_path, &width, &height, &nrChannels, 0);
	if (data2) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "纹理加载失败" << std::endl;
	}
	stbi_image_free(data2);

	// 着色器
	Shader ourShader(vertexCodePath, fragmentCodePath);
	// 将纹理采样器绑定到纹理单元
	ourShader.use();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);

	// 渲染循环
	while (!glfwWindowShouldClose(window)) {
		processInupt(window);

		// 清除颜色缓冲区
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// 绑定纹理
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		// 激活着色器
		ourShader.use();

		// 创建变换矩阵
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;

		model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // 绕x轴旋转
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); // 沿z轴负平移(模拟相机沿z轴正方向平移)
		projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f); // 透视投影

		// 传递变换矩阵到着色器
		unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
		unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
		// 投影矩阵其实很少发生变化, 一般在窗口大小改变时才会改变, 可以不在每帧都传递
		ourShader.setMat4("projection", projection); // 透视投影矩阵

		// 绘制矩形
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// 交换缓冲区, 处理事件
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);

	glDeleteTextures(1, &texture1);
	glDeleteTextures(1, &texture2);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


