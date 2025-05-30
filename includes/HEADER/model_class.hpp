#pragma once

#include <assimp/Importer.hpp> 
#include <assimp/scene.h>      
#include <assimp/postprocess.h>

#include <HEADER/shader_class.hpp>
#include <HEADER/mesh_class.hpp>

#include <vector>
#include <string>
#include <iostream>
/*
* 从文件加载纹理
* @param path 纹理文件相对于模型文件所在目录的子路径
* @param directory 模型文件所在目录
* @return 纹理ID
*/
unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

/*
* 模型类
*/
class Model {
public:
	std::vector<Texture> textures_loaded; // 记录该模型已加载的纹理，避免模型中的多个mesh的材质共用纹理时重复执行加载动作(TextureFromFile)，提高效率
	std::vector<Mesh> meshes; // 属于模型的所有Mesh，将assimp的网格数据转换到自定义的Mesh类中
	std::string directory; // 模型文件所在的目录，加载纹理时需要使用
	bool gammaCorrection; // 是否进行伽马校正

	Model(const std::string& path, bool gamma = false);

	void Draw(Shader& shader);

private:
	void loadModel(const std::string& path); // 加载模型文件，创建aiScene对象
	 
	void processNode(aiNode* node, const aiScene* scene); // 递归处理每一个节点

	Mesh processMesh(aiMesh* mesh, const aiScene* scene); // 处理节点的网格引用数组中的每一个网格，转换assimp的网格数据到自定义的Mesh类中

	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName); // 加载材质的纹理
};

