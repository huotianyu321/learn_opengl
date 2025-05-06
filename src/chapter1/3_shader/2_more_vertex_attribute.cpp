// 顶点数据包含更多的顶点属性 不只是位置信息
// 重点概念：片段插值

#include <HEADER/utils.hpp>
#include <HEADER/call_backs.hpp>
#include <iostream>

const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n" // 从顶点数据接收，之后传递给片段着色器
    "out vec4 ourColor;\n" 
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   ourColor = vec4(aColor, 1.0);\n"
    "}\n";
const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec4 ourColor;\n" // 类型和名称必须与传递的变量相同
    "void main()\n"
    "{\n"
    "   FragColor = ourColor;\n"
    "}\n";


int main()
{
	GLFWwindow* window = initAndCreateWindow(1800, 1200, "OpenGL 3.3"); // 初始化窗口
    if (window == nullptr) {
		std::cout << "窗口创建失败" << std::endl;
		return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebufferSizeChangeCallback); // 注册窗口变化回调

	unsigned int vertexShaderID = createShader("VERTEX", vertexShaderSource);
	unsigned int fragmentShaderID = createShader("FRAGMENT", fragmentShaderSource);
	unsigned int shaderProgramID = createShaderProgram(vertexShaderID, fragmentShaderID);

    //===
    float vertices[] = {
        // 三角形, 每行前三个为顶点位置，后三个为顶点颜色
        0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
       -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

	unsigned int location[] = { 0, 1 };
    unsigned int dimension[] = { 3, 3 };
    unsigned int stride[] = { 6, 6 };
	unsigned int offset[] = { 0, 3 };

    unsigned int VAO = set_VAO_VBO_EBO_mutiple(
        vertices,
        sizeof vertices,
        nullptr,
        0,
        3,
        location,
        dimension,
        stride,
        offset
    );

    // ===
    while (!glfwWindowShouldClose(window)) {
        jobAtRenderLoopStart(window);

        glUseProgram(shaderProgramID); // 这个着色器程序中有location=0和location=1的变量
        glBindVertexArray(VAO); // 这个配置中记录了这两个变量的值应该如何获取
        glDrawArrays(GL_TRIANGLES, 0, 3); // 绘制时按照前两行的指示获取相应的值进行绘制

        jobBeforRenderLoopEnd(window);
    }

    //glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);
    //glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}


