/*
* �κ���ϰ3 ʹ��������ɫ�����Ʋ�ͬ��ɫ��������
*/
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int newWidth, int newHeight);
void processInupt(GLFWwindow* window);

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
const char* fragmentShaderSource2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\n";

int isSuccess;
char infoLog[512];

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
    isSuccess = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (!isSuccess) {
        std::cout << "��ʼ��GLADʧ��" << std::endl;
        return -1;
    }
    else {
        std::cout << "GLAD��ʼ���ɹ�" << std::endl;
    }
    glViewport(0, 0, 800, 600);

    // ===
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isSuccess);
    if (!isSuccess) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "������ɫ������ʧ�ܣ�" << infoLog << std::endl;
    }
    else {
        std::cout << "������ɫ������ɹ�, vertexShader: " << vertexShader << std::endl;
    }
    // ===
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isSuccess);
    if (!isSuccess) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Ƭ����ɫ������ʧ�ܣ�" << infoLog << std::endl;
    }
    else {
        std::cout << "Ƭ����ɫ������ɹ�, fragmentShader: " << fragmentShader << std::endl;
    }
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isSuccess);
    if (!isSuccess) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "��ɫ������ʧ��: " << infoLog << std::endl;
    }
    else {
        std::cout << "��ɫ�����ӳɹ�, shaderProgram: " << shaderProgram << std::endl;
    }
    // ===
    unsigned int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
    glCompileShader(fragmentShader2);
    glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &isSuccess);
    if (!isSuccess) {
        glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
        std::cout << "Ƭ����ɫ��2����ʧ�ܣ�" << infoLog << std::endl;
    }
    else {
        std::cout << "Ƭ����ɫ��2����ɹ�, fragmentShader2: " << fragmentShader2 << std::endl;
    }
    unsigned int shaderProgram2 = glCreateProgram();
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, fragmentShader2);
    glLinkProgram(shaderProgram2);
    glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &isSuccess);
    if (!isSuccess) {
        glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
        std::cout << "��ɫ������ʧ��: " << infoLog << std::endl;
    }
    else {
        std::cout << "��ɫ�����ӳɹ�, shaderProgram2: " << shaderProgram2 << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(fragmentShader2);

    //===
    float vertices[] = {
        // ������1
        0.5f,  0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
       -0.5f, -0.5f, 0.0f,

    };

    float vertices2[] = {
        // ������2
         0.6f,  0.25f, 0.0f,
        0.75f, 0.0f, 0.0f,
        0.6f, -0.25f, 0.0f,
    };
    // ===
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);

    unsigned int VAO2, VBO2;
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);

    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // ===
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        processInupt(window);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgram2);
        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

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


