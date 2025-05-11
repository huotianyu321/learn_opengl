// ����a��b��˵õ���ֱ�����������ཻ����ķ�����(���ֶ���,��a����b)
// ʹ�������ŵľ���
// ʹ����λ�Ƶľ��� (���������е��ĸ�����w(�������))
// w����Ϊ0�������з�������,����λ��. 
// ʹ������ת�ľ���, ��������x, y, z����תʱ, �þ����в�ͬ����ʽ. �����ⵥλ������Ϊ��ת��ʱ�ľ���(����)

#include <HEADER/utils.hpp>
#include <iostream>

int main() {
	// ��ʼ��һ����λ����
	glm::mat4 trans; 

	printMat4(trans);

	glm::mat4 test(2.0f);
	printMat4(test);


	// ����һ��vec4������ ֮���������λ�ƣ�1��1��0������λ
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f); // �����������Ϊ1.0

	// ��trans��λ������һ���ܹ�ʹ��������λ�Ƶľ���
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));

	// Ӧ�þ���任
	vec = trans * vec;

	std::cout << vec.x << vec.y << vec.z << std::endl; // ��ӡ210
	return 0;
}
