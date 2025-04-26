// �Լ�ʵ��һ���������lookAt����(��λ����ת���󿴲�����


#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// ����
glm::mat4 getMyLookAt(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp)
{
	// �����������ϵ
	glm::vec3 zaxis = glm::normalize(position - target);
	glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize(worldUp), zaxis));
	glm::vec3 yaxis = glm::cross(zaxis, xaxis);

	// λ�ƾ���, glm�еڶ�������������, ��һ������������
	// Ҫ��������������λ��Ч���������ø�ֵ�ƶ���������
	glm::mat4 translation = glm::mat4(1.0f);
	translation[3][0] = -position.x; // ��1�е�4��
	translation[3][1] = -position.y;
	translation[3][2] = -position.z;

	// ��ת����GPT��һ��Ϊʲô�����ܲ�����ת����
	glm::mat4 rotation = glm::mat4(1.0f);
	rotation[0][0] = xaxis.x; // First column, first row
	rotation[1][0] = xaxis.y;
	rotation[2][0] = xaxis.z;
	rotation[0][1] = yaxis.x; // First column, second row
	rotation[1][1] = yaxis.y;
	rotation[2][1] = yaxis.z;
	rotation[0][2] = zaxis.x; // First column, third row
	rotation[1][2] = zaxis.y;
	rotation[2][2] = zaxis.z;

	// ����������ƽ�ƺ���ת
	return rotation * translation;
}


// ������ƶ�����
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Ĭ�����������
const float YAW = -90.0f; // ƫ����
const float PITCH = 0.0f; // ������
const float SPEED = 2.5f; // �ƶ��ٶ�
const float SENSITIVITY = 0.1f; // ���������
const float ZOOM = 45.0f; // ���ű���

class Camera {
public:
	glm::vec3 Position; // �����λ��
	glm::vec3 Front; // ���������
	glm::vec3 Up; // ������Ϸ���
	glm::vec3 Right; // ������ҷ���
	glm::vec3 WorldUp; // ��������ϵ���Ϸ���
	float Yaw; // ƫ����
	float Pitch; // ������
	float MovementSpeed; // �ƶ��ٶ�
	float MouseSensitivity; // ���������
	float Zoom; // ���ű���

	// ���캯��1
	Camera(
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = YAW,
		float pitch = PITCH
	) : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
		MovementSpeed(SPEED),
		MouseSensitivity(SENSITIVITY),
		Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	// ���캯��2
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
	* ��ȡ����Ĺ۲����
	*/
	glm::mat4 GetViewMatrix() {
		return getMyLookAt(Position, Front, WorldUp);
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
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f) {
			Zoom = 1.0f;
		}
		if (Zoom > 45.0f) {
			Zoom = 45.0f;
		}
	}

private:
	/*
	* ��������ķ�������
	*/
	void updateCameraVectors() {
		// ���ݸ����Ǻ�ƫ���Ǽ���ǰ����
		glm::vec3 front;
		front.y = sin(glm::radians(Pitch));
		front.x = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
		front.z = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
		Front = glm::normalize(front);

		// �������������������� ���ֶ��� ���
		Right = glm::normalize(glm::cross(Front, WorldUp)); // ������
		Up = glm::normalize(glm::cross(Right, Front)); // ������
	}
};

#endif