#ifndef PROCESS_EVENTS_H
#define PROCESS_EVENTS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// 检查ESCAPE键是否按下
void processEscapeKey(GLFWwindow* window);


// 4-exercise4
void processUpDownKey(GLFWwindow* window, float* number);


#endif // PROCESS_EVENTS_H