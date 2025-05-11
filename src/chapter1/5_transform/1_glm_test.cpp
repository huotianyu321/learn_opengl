// 向量a×b叉乘得到垂直于两个向量相交的面的法向量(右手定则,从a握向b)
// 使向量缩放的矩阵
// 使向量位移的矩阵 (向量必须有第四个分量w(齐次坐标))
// w分量为0的向量叫方向向量,不能位移. 
// 使向量旋转的矩阵, 当向量绕x, y, z轴旋转时, 该矩阵有不同的形式. 绕任意单位向量作为旋转轴时的矩阵(复杂)

#include <HEADER/utils.hpp>
#include <iostream>

int main() {
	// 初始化一个单位矩阵
	glm::mat4 trans; 

	printMat4(trans);

	glm::mat4 test(2.0f);
	printMat4(test);


	// 定义一个vec4向量， 之后将这个向量位移（1，1，0）个单位
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f); // 齐次坐标设置为1.0

	// 将trans单位矩阵变成一个能够使向量发生位移的矩阵
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));

	// 应用矩阵变换
	vec = trans * vec;

	std::cout << vec.x << vec.y << vec.z << std::endl; // 打印210
	return 0;
}
