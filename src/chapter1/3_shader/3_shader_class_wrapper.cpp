// ��shader��װ��һ������

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include "3_shader_class.h"

void framebuffer_size_callback(GLFWwindow* window, int newWidth, int newHeight);
void processInupt(GLFWwindow* window);

const char* vertexCodePath = "./src/chapter1/3_shader/3_vertex_shader_code.txt";
const char* fragmentCodePath = "./src/chapter1/3_shader/3_fragment_shader_code.txt";

int main()
{
    // ===
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // ���汾��
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // �ΰ汾��
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // ����ģʽ

    GLFWwindow* window = glfwCreateWindow(800, 600, "learn_opengl", NULL, NULL);
    if (NULL == window) {
        std::cout << "����GLFW����ʧ��" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // ��������
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // ע�ᴰ�ڱ仯�ص�

    // ===
    int isSuccess;
    isSuccess = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (!isSuccess) {
        std::cout << "��ʼ��GLADʧ��" << std::endl;
        return -1;
    }
    else {
        std::cout << "GLAD��ʼ���ɹ�" << std::endl;
    }
    glViewport(0, 0, 800, 600);

    Shader* myShaderWrapper = new Shader(vertexCodePath, fragmentCodePath);

    //===
    float vertices[] = {
        // ������, ÿ��ǰ����Ϊ����λ�ã�������Ϊ������ɫ
        0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
       -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    // ===
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // ָ��������ɫ����location=0�Ķ�������, ȡ3��Ԫ�أ����ͣ� ����һ��, 6��Ԫ�صĿ�ȣ�ƫ����Ϊ0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // ָ��������ɫ����location=1�Ķ�������, ȡ3��Ԫ�أ����ͣ� ����һ��, 6��Ԫ�صĿ�ȣ�ƫ����Ϊ3
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);


    // ===
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        processInupt(window);

        myShaderWrapper->use();
        glBindVertexArray(VAO); // ��������м�¼��������������ֵӦ����λ�ȡ
        glDrawArrays(GL_TRIANGLES, 0, 3); // ����ʱ����ǰ���е�ָʾ��ȡ��Ӧ��ֵ���л���

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

// �ڴ��ڴ�С�仯ʱ������
void framebuffer_size_callback(GLFWwindow* window, int newWidth, int newHeight) {
    glViewport(0, 0, newWidth, newHeight);
}
// ���ESCAPE���Ƿ���
void processInupt(GLFWwindow* window) {
    // ��鴰����ĳ�����İ���״̬����������˷���GLFW_PRESS, ���򷵻�GLFW_RELEASE
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}


