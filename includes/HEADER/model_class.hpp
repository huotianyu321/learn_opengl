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
* ���ļ���������
* @param path �����ļ������ģ���ļ�����Ŀ¼����·��
* @param directory ģ���ļ�����Ŀ¼
* @return ����ID
*/
unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

/*
* ģ����
*/
class Model {
public:
	std::vector<Texture> textures_loaded; // ��¼��ģ���Ѽ��ص���������ģ���еĶ��mesh�Ĳ��ʹ�������ʱ�ظ�ִ�м��ض���(TextureFromFile)�����Ч��
	std::vector<Mesh> meshes; // ����ģ�͵�����Mesh����assimp����������ת�����Զ����Mesh����
	std::string directory; // ģ���ļ����ڵ�Ŀ¼����������ʱ��Ҫʹ��
	bool gammaCorrection; // �Ƿ����٤��У��

	Model(const std::string& path, bool gamma = false);

	void Draw(Shader& shader);

private:
	void loadModel(const std::string& path); // ����ģ���ļ�������aiScene����
	 
	void processNode(aiNode* node, const aiScene* scene); // �ݹ鴦��ÿһ���ڵ�

	Mesh processMesh(aiMesh* mesh, const aiScene* scene); // ����ڵ���������������е�ÿһ������ת��assimp���������ݵ��Զ����Mesh����

	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName); // ���ز��ʵ�����
};

