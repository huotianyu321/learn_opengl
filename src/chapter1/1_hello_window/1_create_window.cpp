// GLAD��GLFW
// һЩ��ʼ������
// �ص���
// ��������
// ˫����
// NDC����

#include <glad/glad.h> // ����������ȡʵ��OpenGL�淶��������������д�ĸ���������λ��
#include <GLFW/glfw3.h> // GLFW��һ��ר�����OpenGL��C���Կ⣬���ṩ��һЩ��Ⱦ�������������޶ȵĽӿڣ����紴��һ�����ڣ�
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int newWidth, int newHeight);
void processInupt(GLFWwindow* window);

int main()
{
    // ��ʼ��GLFW
    glfwInit();
    // ��ʼ����GLFW
    // ����GLFW��ǰʹ�õ�OpenGL�汾Ϊ3.3�����汾��.�ΰ汾�ţ�
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // ʹ�ú���ģʽ��ֻ����OpenGL���ܵ��Ӽ�������Ҫ�����ݵ����ԣ�
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    // �������ڶ��󣨿��ߡ���������, ������������gpt
    GLFWwindow* window = glfwCreateWindow(800, 600, "learn_opengl", NULL, NULL);
    if (NULL == window) {
        std::cout << "����GLFW����ʧ��" << std::endl;
        glfwTerminate();
        return -1;
    }
    // ֪ͨGLFW���ô�������λ����Ϊ��ǰ�̵߳���������
    glfwMakeContextCurrent(window);

    // ��ʼ��GLAD
    // ��GLAD������������ϵͳ��ص�OpenGL����ָ���ַ�ĺ�����GLFW�����ǵ���glfwGetProcAddress�������ݱ����ϵͳ��������ȷ�ĺ�����
    int isSuccess = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (!isSuccess) {
        std::cout << "��ʼ��GLADʧ��" << std::endl;
        return -1;
    }

    // �����ӿڣ���Ⱦ���ڣ���С
    // ����ǰ�����������ƴ������½ǵ�λ�ã��������͵��ĸ�����������Ⱦ���ڵĿ�Ⱥ͸߶ȣ����أ�
    /*
    * ==���±���λ��ŵ�gpt����һ�¿����� �˽���NDC����ĺ���==
    * OpenGLĻ��ʹ��glViewport�ж����λ�úͿ�߽���2D�����ת������OpenGL�е�λ������ת��Ϊ�����Ļ���ꡣ
    * ���磬OpenGL�е�����(-0.5, 0.5)�п��ܣ����գ���ӳ��Ϊ��Ļ�е�����(200,450)��
    * ע�⣬�������OpenGL���귶ΧֻΪ-1��1�����������ʵ�Ͻ�(-1��1)��Χ�ڵ�����ӳ�䵽(0, 800)��(0, 600)��
    */
    glViewport(0, 0, 800, 600);


    // ע�ắ�����ڴ��ڴ�С�仯ʱ������
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // ����render loop
    // ÿ��ѭ������Ƿ�Ҫ���˳�
    while (!glfwWindowShouldClose(window)) {
        // ����һ����Ⱦѭ��֮ǰ����ϴε���Ⱦ���
        // ���������ɫ����ʱ��������ɫ��ɫ����
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // �����ɫ���壬�����Ļ���λ���û���GL_DEPTH_BUFFER_BIT��GL_STENCIL_BUFFER_BIT
        glClear(GL_COLOR_BUFFER_BIT);


        // ���������¼�
        processInupt(window);

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
