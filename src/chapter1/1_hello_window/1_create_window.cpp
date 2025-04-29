// GLAD��GLFW
// һЩ��ʼ������
// �ص���
// ��������
// ˫����
// NDC����

#include <HEADER/utils.hpp>
#include <HEADER/call_backs.hpp>
#include <iostream>

const unsigned int SCREEN_WIDTH = 2000;
const unsigned int SCREEN_HEIGHT = 1500;

int main() {
	// ��ʼ��GLFW��GLAD�� �������ڣ�����viewport
	GLFWwindow* window = initAndCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "learn_opengl");
	if (window == NULL) {
		glfwTerminate();
		return -1;
	}

	// ע�ᴰ�ڴ�С�Ļص�����
	glfwSetFramebufferSizeCallback(window, framebufferSizeChangeCallback);

	while (!glfwWindowShouldClose(window)) {
		jobAtRenderLoopStart(window);

		// do something....
		std::cout << "rendering...\n";

		jobBeforRenderLoopEnd(window);
	}

	// �����ͷ���Դ
	glfwTerminate();
	return 0;
}