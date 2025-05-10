// ����a��b��˵õ���ֱ�����������ཻ����ķ�����(���ֶ���,��a����b)
// ʹ�������ŵľ���
// ʹ����λ�Ƶľ��� (���������е��ĸ�����w(�������))
// w����Ϊ0�������з�������,����λ��. 
// ʹ������ת�ľ���, ��������x, y, z����תʱ, �þ����в�ͬ����ʽ. �����ⵥλ������Ϊ��ת��ʱ�ľ���(����)

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

int main() {
	// ��ʼ��һ����λ����
	glm::mat4 trans; 

	// ��ӡ��λ����
    	std::cout << trans[0][0] << trans[0][1] << trans[0][2] << trans[0][3] << std::endl;
    	std::cout << trans[1][0] << trans[1][1] << trans[1][2] << trans[1][3] << std::endl;
    	std::cout << trans[2][0] << trans[2][1] << trans[2][2] << trans[2][3] << std::endl;
    	std::cout << trans[3][0] << trans[3][1] << trans[3][2] << trans[3][3] << std::endl;


	// ����һ��vec4������ ֮���������λ�ƣ�1��1��0������λ
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f); // �����������Ϊ1.0

	// ��trans��λ������һ���ܹ�ʹ��������λ�Ƶľ���
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));

	// Ӧ�þ���任
	vec = trans * vec;

	std::cout << vec.x << vec.y << vec.z << std::endl; // ��ӡ210
	return 0;
}
