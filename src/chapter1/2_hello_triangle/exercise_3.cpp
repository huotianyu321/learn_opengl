/*
* 课后练习3 使用两个着色器绘制不同颜色的三角形
*/
#include <HEADER/utils.hpp>
#include <HEADER/call_backs.hpp>

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n";
const char* fragmentShaderSource1 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";
const char* fragmentShaderSource2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"}\n";

int isSuccess;
char infoLog[512];

int main()
{
    GLFWwindow* window = initAndCreateWindow(2000, 1500, "1-e2");

    unsigned int vertexShaderID, fragmentShaderID;
    vertexShaderID = createShader("VERTEX", vertexShaderSource);
    fragmentShaderID = createShader("FRAGMENT", fragmentShaderSource1);
    unsigned int shaderProgramID1 = createShaderProgram(vertexShaderID, fragmentShaderID);

    vertexShaderID = createShader("VERTEX", vertexShaderSource);
    fragmentShaderID = createShader("FRAGMENT", fragmentShaderSource2);
    unsigned int shaderProgramID2 = createShaderProgram(vertexShaderID, fragmentShaderID);

    //===
    float vertices[] = {
        // 三角形1
        0.5f,  0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
       -0.5f, -0.5f, 0.0f,

    };

    float vertices2[] = {
        // 三角形2
         0.6f,  0.25f, 0.0f,
        0.75f, 0.0f, 0.0f,
        0.6f, -0.25f, 0.0f,
    };
    // ===
    unsigned int VAO1 = set_VAO_VBO_EBO(
		vertices, sizeof(vertices), nullptr, 0, 0, 3, 3, 0
	);

	unsigned int VAO2 = set_VAO_VBO_EBO(
		vertices2, sizeof(vertices2), nullptr, 0, 0, 3, 3, 0
	);

    // ===
    while (!glfwWindowShouldClose(window)) {
		jobAtRenderLoopStart(window);

        glUseProgram(shaderProgramID1);
        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgramID2);
        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        jobBeforRenderLoopEnd(window);
    }

    glfwTerminate();
    return 0;
}

