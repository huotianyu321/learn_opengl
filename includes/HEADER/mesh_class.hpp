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
* 网格类
*/
class Mesh {
public:
	// 顶点数据数组
	std::vector<Vertex> vertices;
	// 索引数据数组
	std::vector<unsigned int> indices;
	// 纹理数据数组
	std::vector<Texture> textures;

	// 构造函数
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

	// 绘制
	void Draw(Shader shader);
private:
	// 渲染需要的数据
	unsigned int VAO, VBO, EBO;
	// 初始化mesh
	void setupMesh();

};