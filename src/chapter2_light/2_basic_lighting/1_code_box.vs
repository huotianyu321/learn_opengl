#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal; // ������

out vec3 FragPos; // Ƭ��λ��
out vec3 Normal; // Ƭ�η�����

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // ������λ��ת������������ϵ
    FragPos = vec3(model * vec4(aPos, 1.0)); // ����Ƭ��λ��

    // ʹ��ģ�;������Ͻ�3*3�����������ת�þ���Է��������б任����ֹ��ͬ���ű����Է�������Ӱ��
    Normal = mat3(transpose(inverse(model))) * aNormal; // ���㷨����

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}