// �ǳ���Ҫ��һ��
// VAO VBO EBO �󶨹�ϵ ״̬��¼
// ��ɫ������ ��ɫ������ ��ɫ���������
// ִ��һ����Ⱦ��������İ��뼤��˳��

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
		std::cout << "��ʼ������ʧ��" << std::endl;
		return -1;
	}

    glfwSetFramebufferSizeCallback(window, framebufferSizeChangeCallback);

	unsigned int vertexShaderID = createShader("VERTEX", vertexShaderSource);
    unsigned int fragmentShaderID = createShader("FRAGMENT", fragmentShaderSource);
	unsigned int shaderProgramID = createShaderProgram(vertexShaderID, fragmentShaderID);

    // ��������
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // ����
         0.5f, -0.5f, 0.0f,  // ����
        -0.5f, -0.5f, 0.0f,  // ����
        -0.5f,  0.5f, 0.0f   // ���� 
    };
    // ��������
    unsigned int indices[] = {
        0, 1, 3, // ���Ϸ�������(˳ʱ�붥��)
        1, 2, 3 // ���·�
    };

	unsigned int VAO = set_VAO_VBO_EBO(
        vertices, sizeof vertices, indices, sizeof indices, 0, 3, 3, 0);
    
    while (!glfwWindowShouldClose(window)) {
		jobAtRenderLoopStart(window);

        // ��ΪĿǰֻ��һ����ɫ������һ��VAO, ��ʵ��ѭ������һ�ΰ�Ҳ����
        glUseProgram(shaderProgramID);

        glBindVertexArray(VAO); 

        // ʹ��EBO���ƶ��㣬ͼԪ����, �������, ������������������, ����������ƫ����
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // PS: �������д����ֻʹ��VBO����Ⱦ, ��EBO�������޹�
        // ����ֻ���һ��������,�����Ҫ�������,��Ҫ��vertices������д6����
        // �����Ļ������������ظ�, �޷����ö���, ��Ҳ��EBO���ڵ�����
        // ͼԪ����, ����������ʼ����, �������
        glDrawArrays(GL_TRIANGLES, 0, 3);

		jobBeforRenderLoopEnd(window);
    }

    // �����ͷ���Դ
    glfwTerminate();
    return 0;
}
