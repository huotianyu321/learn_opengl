// 练习3: 尝试在矩形上只显示纹理图像的中间一部分，修改纹理坐标，达到能看见单个的像素的效果。
// 尝试使用GL_NEAREST的纹理过滤方式让像素显示得更清晰
// ---- 把四个顶点的纹理坐标设置的尽可能接近

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
		cout << "创建窗口失败" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// === 加载所有OpenGL函数指针
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "初始化GLAD失败" << endl;
		return -1;
	}

	// 构建着色器对象,编译着色器;构建着色器程序,链接着色器对象
	Shader myShader(vertexCodePath, fragmentCodePath);

	float vertices[] = {
		// 顶点坐标          // 颜色           // 纹理坐标
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.55f, 0.55f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.55f, 0.45f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.45f, 0.45f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.45f, 0.55f  // top left 
	};
	// 顶点索引
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

	// ==== 第一个纹理
	glGenTextures(1, &texture); // 创建纹理对象
	glBindTexture(GL_TEXTURE_2D, texture); // 绑定到2d纹理, 之后对2D纹理所作的操作都会影响这个对象
	// 设置纹理的重复方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 设置过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// 加载图片, 创建纹理图像  生成多级渐远纹理
	int width, height, nrChannels;
	unsigned char* imageData = stbi_load(imagePath, &width, &height, &nrChannels, 0);
	if (imageData) {
		// 根据图片生成纹理图像
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
		// 生成多级渐远纹理
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "图片1加载失败 " << endl;
	}
	stbi_image_free(imageData);

	// 第二个纹理
	glGenTextures(1, &texture2); // 创建纹理对象
	glBindTexture(GL_TEXTURE_2D, texture2); // 绑定到2d纹理, 之后对2D纹理所作的操作都会影响这个对象
	// 设置纹理的重复方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 设置过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// 加载图片, 创建纹理图像  生成多级渐远纹理
	int width2, height2, nrChannels2;
	stbi_set_flip_vertically_on_load(true); // 加载图片时y轴颠倒
	unsigned char* imageData2 = stbi_load(imagePath2, &width2, &height2, &nrChannels2, 0);
	if (imageData2) {
		// 根据图片生成纹理图像, 第二个图片是png格式, 需要指定透明通道
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData2);
		// 生成多级渐远纹理
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "图片2加载失败 " << endl;
	}
	stbi_image_free(imageData2);

	// 通过uniform设置片段着色器中的两个sampler2D, 使其对应纹理单元0 和纹理单元1
	// opengl一般有0 - 15个纹理单元
	// 稍后在渲染循环中bind纹理对象前分别激活纹理单元,使得各纹理单元知道纹理做了哪些配置
	myShader.use(); // 别忘了先激活
	myShader.setInt("texture1", 0); // 注意这里是着色器代码中的sampler2D
	glUniform1i(glGetUniformLocation(myShader.ID, "texture2"), 1);

	while (!glfwWindowShouldClose(window))
	{
		processInupt(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture); // 注意这里是c++代码中的纹理对象

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



