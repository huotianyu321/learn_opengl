#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

// 摄像机移动方向
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

// 默认摄像机参数
const float YAW = -90.0f; // 偏航角
const float PITCH = 0.0f; // 俯仰角
const float SPEED = 2.5f; // 移动速度
const float SENSITIVITY = 0.1f; // 鼠标灵敏度
const float ZOOM = 45.0f; // 缩放比例

class Camera {
public:
	glm::vec3 Position; // 摄像机位置
	glm::vec3 Front; // 前向量：摄像机朝向，摄像机坐标系z轴负方向
	glm::vec3 Up; // 上向量：摄像机坐标系y轴正方向
	glm::vec3 Right; // 右向量：摄像机坐标系x轴正方向
	glm::vec3 WorldUp; // 世界坐标系的上方向
	float Yaw; // 偏航角
	float Pitch; // 俯仰角
	float MovementSpeed; // 移动速度
	float MouseSensitivity; // 鼠标灵敏度
	float Zoom; // 缩放比例

	// 构造函数1
	Camera(
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = YAW,
		float pitch = PITCH
	) : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
		MovementSpeed(SPEED),
		MouseSensitivity(SENSITIVITY),
		Zoom(ZOOM)
	{
		Position = position;
		WorldUp = worldUp;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	// 构造函数2
	Camera(
		float posX, float posY, float posZ,
		float upX, float upY, float upZ,
		float yaw, float pitch
	) : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
		MovementSpeed(SPEED),
		MouseSensitivity(SENSITIVITY),
		Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	/*
	* 获取相机的观察矩阵
	*/
	glm::mat4 GetViewMatrix() {
		return glm::lookAt(Position, Position + Front, WorldUp);
	}


	void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
			Position += Front * velocity;
		if (direction == BACKWARD)
			Position -= Front * velocity;
		if (direction == LEFT)
			Position -= Right * velocity;
		if (direction == RIGHT)
			Position += Right * velocity;
		if (direction == UP) 
			Position += WorldUp * velocity;
		if (direction == DOWN)
			Position -= WorldUp * velocity;
	}


	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;
		Yaw += xoffset;
		Pitch += yoffset;
		if (constrainPitch) {
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}
		updateCameraVectors();
	}

	void ProcessMouseScroll(float yoffset) {
		Zoom -= yoffset;
		if (Zoom < 1.0f) {
			Zoom = 1.0f;
		}
		if (Zoom > 45.0f) {
			Zoom = 45.0f;
		}
	}

private:
	/*
	* 更新相机的方向向量
	*/
	void updateCameraVectors() {
		// 根据俯仰角和偏航角计算前向量
		glm::vec3 front;
		front.y = sin(glm::radians(Pitch));
		front.x = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
		front.z = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
		Front = glm::normalize(front);

		// 计算右向量和上向量， 右手定则， 叉乘
		Right = glm::normalize(glm::cross(Front, WorldUp)); // 右向量
		Up = glm::normalize(glm::cross(Right, Front)); // 上向量
	}
};



#endif