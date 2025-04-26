// 自己实现一个函数获得lookAt矩阵
// lookAt矩阵的形式看教程LookAt

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// 参数是相机的方向向量（Front)、右向量、上向量和位置
glm::mat4 getMyLookAt(glm::vec3 Front, glm::vec3 Right, glm::vec3 Up, glm::vec3 Pos) {
	float Rx = Right.x;
	float Ry = Right.y;
	float Rz = Right.z;
	float Ux = Up.x;
	float Uy = Up.y;
	float Uz = Up.z;
	float Dx = -Front.x;
	float Dy = -Front.y;
	float Dz = -Front.z;
	float Px = Pos.x;
	float Py = Pos.y;
	float Pz = Pos.z;
	glm::mat4 m1 = glm::mat4(1.0f);
	glm::mat4 m2 = glm::mat4(1.0f);
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			if (i == j) {
				m1[i][j] = 1.0f;
				m2[i][j] = 1.0f;
			}
			else {
				m1[i][j] = 0.0f;
				m2[i][j] = 0.0f;
			}
		}
	}
	m1[0][0] = Rx;
	m1[0][1] = Ry;
	m1[0][2] = Rz;

	m1[1][0] = Ux;
	m1[1][1] = Uy;
	m1[1][2] = Uz;
	
	m1[2][0] = Dx;
	m1[2][1] = Dy;
	m1[2][2] = Dz;

	m2[0][3] = -Px;
	m2[1][3] = -Py;
	m2[2][3] = -Pz;

	return m1 * m2;

}