#ifndef UTILS_H
#define UTILS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// 宽度、高度、标题
GLFWwindow* initAndCreateWindow(int width, int height, const char* title);

// 在渲染循环开始时调用的函数
void jobAtRenderLoopStart(GLFWwindow* window);

// 在渲染循环结束前调用
void jobBeforRenderLoopEnd(GLFWwindow* window);

#endif // UTILS_H