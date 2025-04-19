#ifndef CALL_BACK_H
#define CALL_BACK_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// 在窗口大小变化时被调用
void framebuffer_size_callback(GLFWwindow* window, int newWidth, int newHeight);
// 检查ESCAPE键是否按下
void processInupt(GLFWwindow* window);

#endif