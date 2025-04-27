#ifndef CALLBACK_H
#define CALLBACK_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// 在窗口大小变化时被调用
void framebufferSizeChangeCallback(GLFWwindow* window, int newWidth, int newHeight);

// 检查ESCAPE键是否按下
void processEscapeKey(GLFWwindow* window);

#endif // CALLBACK_H