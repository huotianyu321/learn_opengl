#include <assimp/Importer.hpp> 
#include <assimp/scene.h>      
#include <assimp/postprocess.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

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
	std::vector<Texture> textures_loaded; //记录已加载的纹理，避免重复执行加载动作(TextureFromFile)，提高效率
	std::vector<Mesh> meshes;
	std::string directory;

	Model(char* path) {
		loadModel(path);
	}

	void Draw(Shader& shader) {
		for (unsigned int i = 0; i < meshes.size(); i++) {
			meshes[i].Draw(shader);
		}
	}
private:
	void loadModel(std::string path) {
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;

		}

		directory = path.substr(0, path.find_last_of('/'));
		if (scene != nullptr) {
			processNode(scene->mRootNode, scene);
		}
	}

	void processNode(aiNode* node, const aiScene* scene) {
		// 从节点的网格索引数组中获取网格对象，处理每一个网格
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			unsigned int meshIndex = node->mMeshes[i];
			aiMesh* mesh = scene->mMeshes[meshIndex];
			Mesh selfDefinedMesh = processMesh(mesh, scene);
			meshes.push_back(selfDefinedMesh);
		}
		// 从节点的子节点索引数组中获取子节点对象，递归处理每一个子节点
		for (unsigned int i = 0; i < node->mNumChildren; i++) {
			aiNode* childNode = node->mChildren[i];
			processNode(childNode, scene);
		}

	}

	Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		// 处理顶点
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			Vertex vertex;

			glm::vec3 v1;
			v1.x = mesh->mVertices[i].x;
			v1.y = mesh->mVertices[i].y;
			v1.z = mesh->mVertices[i].z;
			vertex.Position = v1;

			glm::vec3 v2;
			v2.x = mesh->mNormals[i].x;
			v2.y = mesh->mNormals[i].y;
			v2.z = mesh->mNormals[i].z;
			vertex.Normal = v2;

			// assimp可以有八组纹理坐标，只关心第一组
			if (mesh->mTextureCoords[0]) {
				glm::vec2 v3;
				v3.x = mesh->mTextureCoords[0][i].x;
				v3.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = v3;
			}
			else {
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			}

			vertices.push_back(vertex);
		}

		// 处理顶点索引
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
			}
		}

		// 处理材质


	}
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};


unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false)
{
	std::string subPath = std::string(path);
	std::string filename = directory + '/' + subPath;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data) {

		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}
	else {

		std::cout << "Texture failed to load at path: " << path << std::endl;

	}

	stbi_image_free(data);
	return textureID;
}