#ifndef PROCESS_EVENTS_H
#define PROCESS_EVENTS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <HEADER/camera_class.hpp>

// 检查ESCAPE键是否按下
void processEscapeKey(GLFWwindow* window);


// 4-exercise4
void processUpDownKey(GLFWwindow* window, float* number);

// 处理wasd键盘输入 7-2
void processWASD(
	GLFWwindow* window,
	float deltaTime,
	glm::vec3& cameraPos,
	const glm::vec3& cameraFront,
	const glm::vec3& cameraUp
);

// 处理wasd键盘输入 7-4
void processWASD(
	GLFWwindow* window,
	Camera& camera,
	float deltaTime
);


#endif // PROCESS_EVENTS_H