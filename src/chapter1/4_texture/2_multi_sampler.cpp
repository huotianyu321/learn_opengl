// 多个纹理采样器
// opengl一般有0 - 15个纹理单元 GL_TEXTURE0 - GL_TEXTURE15
// 通过纹理单元这个中间件 关联片段着色器的sampler2D 和代码中的纹理对象
// 通过glUiform1i为sampler2D设置一个值, 关联uniform变量和纹理单元
// 通过在BindTexture前ActiveTexture(GL_TEXTURE0), 关联纹理单元和纹理对象
// 纹理单元存在的目的是为了在一个片段着色器中可以使用多个纹理
// GL_TEXTUER0是默认激活的, 只有一个纹理时不用做这些操作


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
		// 顶点坐标          // 颜色           // 纹理坐标
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	// 顶点索引
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

	// 第一个纹理
	unsigned int texture1 = createTexture(
		imagePath1,
		GL_REPEAT, GL_REPEAT,
		GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR,
		false,
		false
	);
	// 第二个纹理
	unsigned int texture2 = createTexture(
		imagePath2,
		GL_MIRRORED_REPEAT, GL_REPEAT,
		GL_LINEAR, GL_LINEAR,
		true,
		true
	);

	// 通过uniform设置片段着色器中的两个sampler2D, 使其分别从纹理单元0和纹理单元1中采样
	// opengl一般有0 - 15个纹理单元
	// 稍后在渲染循环中bind纹理对象前分别激活纹理单元,使得各纹理单元与纹理对象
	myShader.use(); 
	myShader.setInt("texture1", 0); // 注意这里是着色器代码中的sampler2D
	set1int(myShader.ID, "texture2", 1);

	while (!glfwWindowShouldClose(window))
	{
		jobAtRenderLoopStart(window);

		// 将纹理对象1绑定到纹理单元0上
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1); 
		// 将纹理对象2绑定到纹理单元1上
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



