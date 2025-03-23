// 非常重要的一节
// VAO VBO EBO 绑定关系 状态记录
// 着色器代码 着色器对象 着色器程序对象
// 执行一次渲染各个对象的绑定与激活顺序

#include <glad/glad.h> // 用来帮助获取实现OpenGL规范的驱动开发商所写的各个函数的位置
#include <GLFW/glfw3.h> // GLFW是一个专门针对OpenGL的C语言库，它提供了一些渲染物体所需的最低限度的接口（比如创建一个窗口）
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int newWidth, int newHeight);
void processInupt(GLFWwindow* window);

// 顶点着色器代码, 注意这里的layout (location=0)
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n";
// 片段着色器代码
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";

// 成功校验
int isSuccess;
// 日志
char infoLog[512];

int main() 
{
    // ========================初始化============================
    // 初始化GLFW
    glfwInit();
    // 开始配置GLFW
    // 告诉GLFW当前使用的OpenGL版本为3.3（主版本号.次版本号）
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // 使用核心模式（只是用OpenGL功能的子集，不需要向后兼容的特性）
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建窗口对象（宽、高、窗口名）, 后两个参数问gpt
    GLFWwindow* window = glfwCreateWindow(800, 600, "learn_opengl", NULL, NULL);
    if (NULL == window) {
        std::cout << "创建GLFW窗口失败" << std::endl;
        glfwTerminate();
        return -1;
    }
    // 通知GLFW将该窗口上下位设置为当前线程的主上下文
    glfwMakeContextCurrent(window);
    // 注册函数，在窗口大小变化时被调用
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 初始化GLAD：加载所有OpenGL函数指针
    // 给GLAD传入用来加载系统相关的OpenGL函数指针地址的函数。GLFW给我们的是glfwGetProcAddress，它根据编译的系统定义了正确的函数。
    isSuccess = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    if (!isSuccess) {
        std::cout << "初始化GLAD失败" << std::endl;
        return -1;
    } else {
        std::cout << "GLAD初始化成功" << std::endl;
    }

    // 设置视口（渲染窗口）大小
    // 函数前两个参数控制窗口左下角的位置，第三个和第四个参数控制渲染窗口的宽度和高度（像素）
    glViewport(0, 0, 800, 600);

    // =============================创建,编译着色器=================================
    // 顶点着色器
    // 创建着色器对象，指定为顶点着色器类型，记录它的id
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // 绑定着色器代码（id, 着色器代码数量, 着色器代码, ?)
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // 编译
    glCompileShader(vertexShader);
    // 检查是否编译成功
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isSuccess);
    if (!isSuccess) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "顶点着色器编译失败：" << infoLog << std::endl;
    } else {
        std::cout << "顶点着色器编译成功, vertexShader: " << vertexShader << std::endl;
    }
    // 片段着色器
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isSuccess);
    if (!isSuccess) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "片段着色器编译失败：" << infoLog << std::endl;
    } else {
        std::cout << "片段着色器编译成功, fragmentShader: " << fragmentShader << std::endl;
    }
    // ==========================创建着色器程序对象, 链接着色器===================
    unsigned int shaderProgram = glCreateProgram();
    // 前一个着色器的输出,必须与下一个着色器的输入相匹配
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // 查看是否成功
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isSuccess);
    if (!isSuccess) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "着色器链接失败: " << infoLog << std::endl;
    } else {
        std::cout << "着色器链接成功, shaderProgram: " << shaderProgram << std::endl;
    }
    // 链接完成后就可以删除着色器了
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ========================准备好顶点数据,顶点索引数据==========================
    // 一个正方形的四个顶点
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // 右上
         0.5f, -0.5f, 0.0f,  // 右下
        -0.5f, -0.5f, 0.0f,  // 左下
        -0.5f,  0.5f, 0.0f   // 左上 
    };
    // 通过绘制两个三角形得到一个正方形, 指定索引数据
    unsigned int indices[] = {
        0, 1, 3, // 右上方三角形(顺时针顶点)
        1, 2, 3 // 左下方
    };

    // ========================开始创建VAO, VBO, EBO, 以及绑定 配置=================
    // 创建顶点数组对象, 顶点缓冲对象, 元素缓冲对象(索引缓冲对象)
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO); // 后边会把vbo和ebo绑定到不同的缓冲类型上
    glGenBuffers(1, &EBO);

    // *****VAO相关*****
    // 记住: 首先绑定VAO, 接下来VBO和EBO的绑定和配置工作都会被记录在这个VAO中
    glBindVertexArray(VAO);

    // ***VBO相关****
    // VBO绑定
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 复制顶点数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 配置顶点属性指针(告诉GPU如何使用VBO对应显存中的数据)
    // 注意这里: 
    // 1. 第一个参数0与顶点着色器代码中Layout (location=0)相对应, 意思就是这个顶点属性配置是给aPos配的
    // 使用这个顶点着色器时, aPos的数据应该按这样指定的方式从绑定的VBO中获取
    // Layout (location=0)相当于为着色器代码中的变量做了一个标识
    // 2. 第二个参数表示当前顶点属性有三个分量(目前只是简单的3d坐标,还没有其他信息)
    // 3. 最后一个参数表示偏移量为0, 其他参数问GPT
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 到这里,就可以解绑VBO了(其实不解绑也没关系,如果要配置其他VBO也是要先绑定的)
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // ***EBO相关****
    // 绑定, 指定不一样的缓冲类型
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // 复制顶点索引数据
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 解绑VAO, 解绑一是为了防止误操作, 然后也可以绑定其他的VAO, 做其他的VBO, EBO配置
    // 要使用上面的配置,下次再绑定这个VAO就可以了
    // 注意, 在解绑VAO之前千万不要解绑EBO, EBO和VBO的绑定机制不一样
    glBindVertexArray(0);
    // 后解绑EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    // ==============================配置完成===============================
    
    
    // =============================开始使用===============================
    while (!glfwWindowShouldClose(window)) {
        // 在下一个渲染循环之前清空上次的渲染结果
        // 设置清空颜色缓冲时用哪种颜色颜色覆盖
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // 清空颜色缓冲，其他的缓冲位设置还有GL_DEPTH_BUFFER_BIT和GL_STENCIL_BUFFER_BIT
        glClear(GL_COLOR_BUFFER_BIT);

        // 处理输入事件
        processInupt(window);

        // ****使用着色器程序以及配置,执行绘制****
        // 因为目前只有一个着色器程序一个VAO, 其实在循环外做一次绑定也可以
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); 
        // 图元类型, 顶点个数, 顶点索引的数据类型, 顶点索引的偏移量
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // PS: 如果这样写就是只使用VBO来渲染, 与EBO的配置无关
        // 图元类型, 顶点数据起始索引, 顶点个数
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        // 这里只绘出一个三角形,如果想要绘出矩形,就要在vertices数组中写6个点
        // 这样的话会有两个点重复, 无法复用顶点, 这也是EBO存在的意义


        // 检查有没有触发什么事件
        // glfwPollEvents 负责处理窗口系统级别的事件（如窗口关闭、调整大小等）, 必须被调用
        glfwPollEvents();

        // 交换颜色缓冲 
        // 单缓冲可能存在图像闪烁的问题
        // 双缓冲中，前缓冲保存最终输出的图像; 所有渲染指令在后缓冲绘制，渲染指令执行完后先换前缓冲和后缓冲
        glfwSwapBuffers(window);
    }

    // 结束释放资源
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
