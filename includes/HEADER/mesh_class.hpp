#pragma once

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <HEADER/shader_class.hpp>

#include <string>
#include <vector>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
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

	// ���캯��
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

	// ����
	void Draw(Shader shader);
private:
	// ��Ⱦ��Ҫ������
	unsigned int VAO, VBO, EBO;
	// ��ʼ��mesh
	void setupMesh();

};