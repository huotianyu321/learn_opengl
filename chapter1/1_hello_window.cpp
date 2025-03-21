// GLAD和GLFW
// 一些初始化工作
// 回调绑定
// 处理输入
// 双缓冲
// NDC坐标

#include <glad/glad.h> // 用来帮助获取实现OpenGL规范的驱动开发商所写的各个函数的位置
#include <GLFW/glfw3.h> // GLFW是一个专门针对OpenGL的C语言库，它提供了一些渲染物体所需的最低限度的接口（比如创建一个窗口）
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int newWidth, int newHeight);
void processInupt(GLFWwindow* window);

int main()
{
    // 初始化GLFW
    glfwInit();
    // 开始配置GLFW
    // 告诉GLFW当前使用的OpenGL版本为3.3（主版本号.次版本号）
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // 使用核心模式（只是用OpenGL功能的子集，不需要向后兼容的特性）
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    // 创建窗口对象（宽、高、窗口名）, 后两个参数问gpt
    GLFWwindow* window = glfwCreateWindow(800, 600, "learn_opengl", NULL, NULL);
    if (NULL == window) {
        std::cout << "创建GLFW窗口失败" << std::endl;
        glfwTerminate();
        return -1;
    }
    // 通知GLFW将该窗口上下位设置为当前线程的主上下文
    glfwMakeContextCurrent(window);

    // 初始化GLAD
    // 给GLAD传入用来加载系统相关的OpenGL函数指针地址的函数。GLFW给我们的是glfwGetProcAddress，它根据编译的系统定义了正确的函数。
    int isSuccess = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (!isSuccess) {
        std::cout << "初始化GLAD失败" << std::endl;
        return -1;
    }

    // 设置视口（渲染窗口）大小
    // 函数前两个参数控制窗口左下角的位置，第三个和第四个参数控制渲染窗口的宽度和高度（像素）
    /*
    * ==把下边这段话放到gpt解释一下看看， 了解下NDC坐标的含义==
    * OpenGL幕后使用glViewport中定义的位置和宽高进行2D坐标的转换，将OpenGL中的位置坐标转换为你的屏幕坐标。
    * 例如，OpenGL中的坐标(-0.5, 0.5)有可能（最终）被映射为屏幕中的坐标(200,450)。
    * 注意，处理过的OpenGL坐标范围只为-1到1，因此我们事实上将(-1到1)范围内的坐标映射到(0, 800)和(0, 600)。
    */
    glViewport(0, 0, 800, 600);


    // 注册函数，在窗口大小变化时被调用
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 定义render loop
    // 每次循环检查是否要求被退出
    while (!glfwWindowShouldClose(window)) {
        // 在下一个渲染循环之前清空上次的渲染结果
        // 设置清空颜色缓冲时用哪种颜色颜色覆盖
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // 清空颜色缓冲，其他的缓冲位设置还有GL_DEPTH_BUFFER_BIT和GL_STENCIL_BUFFER_BIT
        glClear(GL_COLOR_BUFFER_BIT);


        // 处理输入事件
        processInupt(window);

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
