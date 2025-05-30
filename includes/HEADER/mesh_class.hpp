#pragma once

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <HEADER/shader_class.hpp>

#include <string>
#include <vector>

#define MAX_BONE_INFLUENCE 4
struct Vertex {
	glm::vec3 Position; // λ��
	glm::vec3 Normal; // ����
	glm::vec2 TexCoords; // ��������
	glm::vec3 Tangent; // ����
	glm::vec3 Bitangent; // �����ߣ���ʱ��Ҳ�и�����BiNormal
	int m_BoneIDs[MAX_BONE_INFLUENCE]; // ����ID, ��ʾ�������ᱻ���4����Ӱ��
	float m_Weights[MAX_BONE_INFLUENCE]; // ����Ȩ�أ�����Ȩ�ؼ���������1
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path; // ����·��(�����ģ���ļ�Ŀ¼����·��)
};

/*
* ������
*/
class Mesh {
public:
	// ������������
	std::vector<Vertex> vertices;
	// ������������
	std::vector<unsigned int> indices;
	// ������������
	std::vector<Texture> textures;
	// 
	unsigned int VAO;

	// ���캯��
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

	// ����
	void Draw(Shader& shader);
private:
	// ��Ⱦ��Ҫ������
	unsigned int VBO, EBO;
	// ��ʼ��mesh
	void setupMesh();
};