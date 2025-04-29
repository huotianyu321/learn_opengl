// 非常重要的一节
// VAO VBO EBO 绑定关系 状态记录
// 着色器代码 着色器对象 着色器程序对象
// 执行一次渲染各个对象的绑定与激活顺序

#include <HEADER/utils.hpp>
#include <HEADER/call_backs.hpp>
#include <iostream>

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";


int main() {
    const int SCR_WIDTH = 2000;
    const int SCR_HEIGHT = 1500;

    GLFWwindow* window = initAndCreateWindow(SCR_HEIGHT, SCR_HEIGHT, "HELLO TRIANGLE");
	if (window == nullptr) {
		std::cout << "初始化创建失败" << std::endl;
		return -1;
	}

    glfwSetFramebufferSizeCallback(window, framebufferSizeChangeCallback);

	unsigned int vertexShaderID = createShader("VERTEX", vertexShaderSource);
    unsigned int fragmentShaderID = createShader("FRAGMENT", fragmentShaderSource);
	unsigned int shaderProgramID = createShaderProgram(vertexShaderID, fragmentShaderID);

    // 顶点数据
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // 右上
         0.5f, -0.5f, 0.0f,  // 右下
        -0.5f, -0.5f, 0.0f,  // 左下
        -0.5f,  0.5f, 0.0f   // 左上 
    };
    // 顶点索引
    unsigned int indices[] = {
        0, 1, 3, // 右上方三角形(顺时针顶点)
        1, 2, 3 // 左下方
    };

	unsigned int VAO = set_VAO_VBO_EBO(
        vertices, sizeof vertices, indices, sizeof indices, 0, 3, 3, 0);
    
    while (!glfwWindowShouldClose(window)) {
		jobAtRenderLoopStart(window);

        // 因为目前只有一个着色器程序一个VAO, 其实在循环外做一次绑定也可以
        glUseProgram(shaderProgramID);

        glBindVertexArray(VAO); 

        // 使用EBO绘制顶点，图元类型, 顶点个数, 顶点索引的数据类型, 顶点索引的偏移量
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // PS: 如果这样写就是只使用VBO来渲染, 与EBO的配置无关
        // 这里只绘出一个三角形,如果想要绘出矩形,就要在vertices数组中写6个点
        // 这样的话会有两个点重复, 无法复用顶点, 这也是EBO存在的意义
        // 图元类型, 顶点数据起始索引, 顶点个数
        glDrawArrays(GL_TRIANGLES, 0, 3);

		jobBeforRenderLoopEnd(window);
    }

    // 结束释放资源
    glfwTerminate();
    return 0;
}
