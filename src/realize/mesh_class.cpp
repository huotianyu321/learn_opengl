#include <HEADER/mesh_class.hpp>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	this->setupMesh();
}

void Mesh::Draw(Shader shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;

	// Ϊÿһ���������һ�����������
	for (unsigned int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string type = textures[i].type;

		if (type == "texture_diffuse") {
			number = std::to_string(diffuseNr++);
		}
		else if (type == "texture_specular") {
			number = std::to_string(specularNr++);
		}

		// ��Ӧ��shader�е����������Ӧ��Ϊ��
		/*
		* struct Material {
		*	uniform sampler2D texture_diffuse1,
		*	uniform sampler2D texture_diffuse2,
		*	uniform sampler2D texture_specular1,
		*	uniform sampler2D texture_specular2,
		*	...
		* }
		*/

		std::string uniformName = "material." + type + number;
		shader.setInt(uniformName.c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
}

void Mesh::setupMesh() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// ����λ��
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Position)));

	// ������
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Normal)));

	// ��������
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, TexCoords)));

	glBindVertexArray(0);

}
