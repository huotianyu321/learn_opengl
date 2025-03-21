// �ǳ���Ҫ��һ��
// VAO VBO EBO �󶨹�ϵ ״̬��¼
// ��ɫ������ ��ɫ������ ��ɫ���������
// ִ��һ����Ⱦ��������İ��뼤��˳��

#include <glad/glad.h> // ����������ȡʵ��OpenGL�淶��������������д�ĸ���������λ��
#include <GLFW/glfw3.h> // GLFW��һ��ר�����OpenGL��C���Կ⣬���ṩ��һЩ��Ⱦ�������������޶ȵĽӿڣ����紴��һ�����ڣ�
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int newWidth, int newHeight);
void processInupt(GLFWwindow* window);

// ������ɫ������, ע�������layout (location=0)
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n";
// Ƭ����ɫ������
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";

// �ɹ�У��
int isSuccess;
// ��־
char infoLog[512];

int main() 
{
    // ========================��ʼ��============================
    // ��ʼ��GLFW
    glfwInit();
    // ��ʼ����GLFW
    // ����GLFW��ǰʹ�õ�OpenGL�汾Ϊ3.3�����汾��.�ΰ汾�ţ�
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // ʹ�ú���ģʽ��ֻ����OpenGL���ܵ��Ӽ�������Ҫ�����ݵ����ԣ�
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // �������ڶ��󣨿��ߡ���������, ������������gpt
    GLFWwindow* window = glfwCreateWindow(800, 600, "learn_opengl", NULL, NULL);
    if (NULL == window) {
        std::cout << "����GLFW����ʧ��" << std::endl;
        glfwTerminate();
        return -1;
    }
    // ֪ͨGLFW���ô�������λ����Ϊ��ǰ�̵߳���������
    glfwMakeContextCurrent(window);
    // ע�ắ�����ڴ��ڴ�С�仯ʱ������
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // ��ʼ��GLAD����������OpenGL����ָ��
    // ��GLAD������������ϵͳ��ص�OpenGL����ָ���ַ�ĺ�����GLFW�����ǵ���glfwGetProcAddress�������ݱ����ϵͳ��������ȷ�ĺ�����
    isSuccess = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    if (!isSuccess) {
        std::cout << "��ʼ��GLADʧ��" << std::endl;
        return -1;
    } else {
        std::cout << "GLAD��ʼ���ɹ�" << std::endl;
    }

    // �����ӿڣ���Ⱦ���ڣ���С
    // ����ǰ�����������ƴ������½ǵ�λ�ã��������͵��ĸ�����������Ⱦ���ڵĿ�Ⱥ͸߶ȣ����أ�
    glViewport(0, 0, 800, 600);

    // =============================����,������ɫ��=================================
    // ������ɫ��
    // ������ɫ������ָ��Ϊ������ɫ�����ͣ���¼����id
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // ����ɫ�����루id, ��ɫ����������, ��ɫ������, ?)
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // ����
    glCompileShader(vertexShader);
    // ����Ƿ����ɹ�
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isSuccess);
    if (!isSuccess) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "������ɫ������ʧ�ܣ�" << infoLog << std::endl;
    } else {
        std::cout << "������ɫ������ɹ�, vertexShader: " << vertexShader << std::endl;
    }
    // Ƭ����ɫ��
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isSuccess);
    if (!isSuccess) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Ƭ����ɫ������ʧ�ܣ�" << infoLog << std::endl;
    } else {
        std::cout << "Ƭ����ɫ������ɹ�, fragmentShader: " << fragmentShader << std::endl;
    }
    // ==========================������ɫ���������, ������ɫ��===================
    unsigned int shaderProgram = glCreateProgram();
    // ǰһ����ɫ�������,��������һ����ɫ����������ƥ��
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // �鿴�Ƿ�ɹ�
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isSuccess);
    if (!isSuccess) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "��ɫ������ʧ��: " << infoLog << std::endl;
    } else {
        std::cout << "��ɫ�����ӳɹ�, shaderProgram: " << shaderProgram << std::endl;
    }
    // ������ɺ�Ϳ���ɾ����ɫ����
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ========================׼���ö�������,������������==========================
    // һ�������ε��ĸ�����
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // ����
         0.5f, -0.5f, 0.0f,  // ����
        -0.5f, -0.5f, 0.0f,  // ����
        -0.5f,  0.5f, 0.0f   // ���� 
    };
    // ͨ���������������εõ�һ��������, ָ����������
    unsigned int indices[] = {
        0, 1, 3, // ���Ϸ�������(˳ʱ�붥��)
        1, 2, 3 // ���·�
    };

    // ========================��ʼ����VAO, VBO, EBO, �Լ��� ����=================
    // ���������������, ���㻺�����, Ԫ�ػ������(�����������)
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO); // ��߻��vbo��ebo�󶨵���ͬ�Ļ���������
    glGenBuffers(1, &EBO);

    // *****VAO���*****
    // ��ס: ���Ȱ�VAO, ������VBO��EBO�İ󶨺����ù������ᱻ��¼�����VAO��
    glBindVertexArray(VAO);

    // ***VBO���****
    // VBO��
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // ���ƶ�������
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // ���ö�������ָ��(����GPU���ʹ��VBO��Ӧ�Դ��е�����)
    // ע������: 
    // 1. ��һ������0�붥����ɫ��������Layout (location=0)���Ӧ, ��˼��������������������Ǹ�aPos���
    // ʹ�����������ɫ��ʱ, aPos������Ӧ�ð�����ָ���ķ�ʽ�Ӱ󶨵�VBO�л�ȡ
    // Layout (location=0)�൱��Ϊ��ɫ�������еı�������һ����ʶ
    // 2. �ڶ���������ʾ��ǰ������������������(Ŀǰֻ�Ǽ򵥵�3d����,��û��������Ϣ)
    // 3. ���һ��������ʾƫ����Ϊ0, ����������GPT
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // ������,�Ϳ��Խ��VBO��(��ʵ�����Ҳû��ϵ,���Ҫ��������VBOҲ��Ҫ�Ȱ󶨵�)
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // ***EBO���****
    // ��, ָ����һ���Ļ�������
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // ���ƶ�����������
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // ���VAO, ���һ��Ϊ�˷�ֹ�����, Ȼ��Ҳ���԰�������VAO, ��������VBO, EBO����
    // Ҫʹ�����������,�´��ٰ����VAO�Ϳ�����
    // ע��, �ڽ��VAO֮ǰǧ��Ҫ���EBO, EBO��VBO�İ󶨻��Ʋ�һ��
    glBindVertexArray(0);
    // ����EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    // ==============================�������===============================
    
    
    // =============================��ʼʹ��===============================
    while (!glfwWindowShouldClose(window)) {
        // ����һ����Ⱦѭ��֮ǰ����ϴε���Ⱦ���
        // ���������ɫ����ʱ��������ɫ��ɫ����
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // �����ɫ���壬�����Ļ���λ���û���GL_DEPTH_BUFFER_BIT��GL_STENCIL_BUFFER_BIT
        glClear(GL_COLOR_BUFFER_BIT);

        // ���������¼�
        processInupt(window);

        // ****ʹ����ɫ�������Լ�����,ִ�л���****
        // ��ΪĿǰֻ��һ����ɫ������һ��VAO, ��ʵ��ѭ������һ�ΰ�Ҳ����
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); 
        // ͼԪ����, �������, ������������������, ����������ƫ����
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // PS: �������д����ֻʹ��VBO����Ⱦ, ��EBO�������޹�
        // ͼԪ����, ����������ʼ����, �������
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        // ����ֻ���һ��������,�����Ҫ�������,��Ҫ��vertices������д6����
        // �����Ļ������������ظ�, �޷����ö���, ��Ҳ��EBO���ڵ�����


        // �����û�д���ʲô�¼�
        // glfwPollEvents ��������ϵͳ������¼����細�ڹرա�������С�ȣ�, ���뱻����
        glfwPollEvents();

        // ������ɫ���� 
        // ��������ܴ���ͼ����˸������
        // ˫�����У�ǰ���屣�����������ͼ��; ������Ⱦָ���ں󻺳���ƣ���Ⱦָ��ִ������Ȼ�ǰ����ͺ󻺳�
        glfwSwapBuffers(window);
    }

    // �����ͷ���Դ
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
