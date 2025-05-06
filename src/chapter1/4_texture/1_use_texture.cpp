// 采样： 使用纹理坐标为片段获取纹理颜色
// 纹理环绕方式  
// 纹理过滤方式 （纹理像素与纹理坐标，OpenGL使用纹理坐标去纹理图像上查找像素，进行采样提取纹理像素的颜色）
// 多级渐远纹理： 在远处小物体上使用高分辨率纹理的解决方案

// 注意纹理图像坐标系和NDC坐标系的差异
// 纹理坐标在x和y轴上，范围为0到1之间

// 创建纹理对象 - 绑定某种纹理 - 配置参数 - 加载图片,生成纹理图像,释放图片
// 片段着色器 sampler2D 类型 纹理采样器, 采样函数texture(textureSampler, textureCoord)

#include <HEADER/utils.hpp>
#include <HEADER/call_backs.hpp>
#include <HEADER/shader_class.hpp>

#include <iostream>

int main() {
	const char* vertexCodePath = "./src/chapter1/4_texture/1_code.vs";
	const char* fragmentCodePath = "./src/chapter1/4_texture/1_code.fs";
	const char* imagePath = "./resources/container.jpg";

	GLFWwindow* window = initAndCreateWindow(1800, 1200, "4.1");

	// 构建着色器对象,编译着色器;构建着色器程序,链接着色器对象
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



