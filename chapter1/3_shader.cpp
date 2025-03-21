// uniform值的使用方法
// 着色器之间通过输入输出传值的方法

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int newWidth, int newHeight);
void processInupt(GLFWwindow* window);

const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "out vec4 colorFromVertexShader;\n" // 指定一个输出，传递给fragmentShader
    "uniform vec4 ourColor;\n" // 通过代码寻找uniform的位置并为它设置值
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   colorFromVertexShader = ourColor;\n"
    "}\n";
const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec4 colorFromVertexShader;\n" // 类型和名称必须与传递的变量相同
    //"uniform vec4 ourColor;\n"
    "void main()\n"
    "{\n"
    //"   FragColor = ourColor;\n"
    "   FragColor = colorFromVertexShader;\n"
    "}\n";

int isSuccess;
char infoLog[512];

int main()
{
    // ===
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 次版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 核心模式

    GLFWwindow* window = glfwCreateWindow(800, 600, "learn_opengl", NULL, NULL);
    if (NULL == window) {
        std::cout << "创建GLFW窗口失败" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // 绑定上下文
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // 注册窗口变化回调

    // ===
    isSuccess = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (!isSuccess) {
        std::cout << "初始化GLAD失败" << std::endl;
        return -1;
    }
    else {
        std::cout << "GLAD初始化成功" << std::endl;
    }
    glViewport(0, 0, 800, 600);

    // ===
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isSuccess);
    if (!isSuccess) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "顶点着色器编译失败：" << infoLog << std::endl;
    }
    else {
        std::cout << "顶点着色器编译成功, vertexShader: " << vertexShader << std::endl;
    }
    // ===
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isSuccess);
    if (!isSuccess) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "片段着色器编译失败：" << infoLog << std::endl;
    }
    else {
        std::cout << "片段着色器编译成功, fragmentShader: " << fragmentShader << std::endl;
    }
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isSuccess);
    if (!isSuccess) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "着色器链接失败: " << infoLog << std::endl;
    }
    else {
        std::cout << "着色器链接成功, shaderProgram: " << shaderProgram << std::endl;
    }


    //===
    float vertices[] = {
        // 三角形1
        0.5f,  0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
       -0.5f, -0.5f, 0.0f,

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

    
    // ===
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        processInupt(window);

        double timeValue = glfwGetTime();
        // 用sin获取一个随时间变化的0-1的值
        float greenValue = static_cast<float>(sin(timeValue) / 2.0 + 0.5);
        // 查询uniform变量的位置
        // 查询的时候不要求激活着色器程序
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        // 设置uniform的值之前，必须要激活着色器程序
        // 因为是在当前激活的着色器程序中设置uniform的。
        glUseProgram(shaderProgram);
        // 设置uniform的值, 根据函数后缀，这里传入4个float
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

// 在窗口大小变化时被调用
void framebuffer_size_callback(GLFWwindow* window, int newWidth, int newHeight) {
    glViewport(0, 0, newWidth, newHeight);
}
// 检查ESCAPE键是否按下
void processInupt(GLFWwindow* window) {
    // 检查窗口中某个键的按下状态，如果按下了返回GLFW_PRESS, 否则返回GLFW_RELEASE
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}


