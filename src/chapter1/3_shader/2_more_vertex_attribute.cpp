// �������ݰ�������Ķ������� ��ֻ��λ����Ϣ
// �ص���Ƭ�β�ֵ

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int newWidth, int newHeight);
void processInupt(GLFWwindow* window);

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
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


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
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);


    // ===
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        processInupt(window);

        glUseProgram(shaderProgram); // �����ɫ����������location=0��location=1�ı���
        glBindVertexArray(VAO); // ��������м�¼��������������ֵӦ����λ�ȡ
        glDrawArrays(GL_TRIANGLES, 0, 3); // ����ʱ����ǰ���е�ָʾ��ȡ��Ӧ��ֵ���л���

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
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


