// �������ݰ�������Ķ������� ��ֻ��λ����Ϣ
// �ص���Ƭ�β�ֵ

#include <HEADER/utils.hpp>
#include <HEADER/call_backs.hpp>
#include <iostream>

const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n" // �Ӷ������ݽ��գ�֮�󴫵ݸ�Ƭ����ɫ��
    "out vec4 ourColor;\n" 
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   ourColor = vec4(aColor, 1.0);\n"
    "}\n";
const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec4 ourColor;\n" // ���ͺ����Ʊ����봫�ݵı�����ͬ
    "void main()\n"
    "{\n"
    "   FragColor = ourColor;\n"
    "}\n";


int main()
{
	GLFWwindow* window = initAndCreateWindow(1800, 1200, "OpenGL 3.3"); // ��ʼ������
    if (window == nullptr) {
		std::cout << "���ڴ���ʧ��" << std::endl;
		return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebufferSizeChangeCallback); // ע�ᴰ�ڱ仯�ص�

	unsigned int vertexShaderID = createShader("VERTEX", vertexShaderSource);
	unsigned int fragmentShaderID = createShader("FRAGMENT", fragmentShaderSource);
	unsigned int shaderProgramID = createShaderProgram(vertexShaderID, fragmentShaderID);

    //===
    float vertices[] = {
        // ������, ÿ��ǰ����Ϊ����λ�ã�������Ϊ������ɫ
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

        glUseProgram(shaderProgramID); // �����ɫ����������location=0��location=1�ı���
        glBindVertexArray(VAO); // ��������м�¼��������������ֵӦ����λ�ȡ
        glDrawArrays(GL_TRIANGLES, 0, 3); // ����ʱ����ǰ���е�ָʾ��ȡ��Ӧ��ֵ���л���

        jobBeforRenderLoopEnd(window);
    }

    //glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);
    //glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}


