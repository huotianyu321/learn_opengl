#pragma once

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <HEADER/shader_class.hpp>

#include <string>
#include <vector>

#define MAX_BONE_INFLUENCE 4
struct Vertex {
	glm::vec3 Position; // 位置
	glm::vec3 Normal; // 法线
	glm::vec2 TexCoords; // 纹理坐标
	glm::vec3 Tangent; // 切线
	glm::vec3 Bitangent; // 副切线，有时候也叫副法线BiNormal
	int m_BoneIDs[MAX_BONE_INFLUENCE]; // 骨骼ID, 表示这个顶点会被最多4骨骼影响
	float m_Weights[MAX_BONE_INFLUENCE]; // 骨骼权重，所有权重加起来等于1
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path; // 纹理路径(相对于模型文件目录的子路径)
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
	// 
	unsigned int VAO;

	// 构造函数
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

	// 绘制
	void Draw(Shader& shader);
private:
	// 渲染需要的数据
	unsigned int VBO, EBO;
	// 初始化mesh
	void setupMesh();
};