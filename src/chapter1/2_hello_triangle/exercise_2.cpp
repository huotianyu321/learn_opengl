/*
* 课后练习2 使用不同的VAO和VBO绘制两个三角形
*/
#include <HEADER/utils.hpp>
#include <HEADER/call_backs.hpp>

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

int main()
{
    GLFWwindow* window = initAndCreateWindow(2000, 1500, "1-e2");

	unsigned int vertexShaderID = createShader("VERTEX", vertexShaderSource);
	unsigned int fragmentShaderID = createShader("FRAGMENT", fragmentShaderSource);
	unsigned int shaderProgramID = createShaderProgram(vertexShaderID, fragmentShaderID);

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

        glUseProgram(shaderProgramID);

        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        jobBeforRenderLoopEnd(window);
    }

    glfwTerminate();
    return 0;
}

