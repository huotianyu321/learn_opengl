#include <stb_image/stb_image.h>
#include <HEADER/model_class.hpp>

/*
* ģ����
*/


Model::Model(const std::string& path, bool gamma) : gammaCorrection(gamma) {

	loadModel(path);

	std::cout << "ģ�ͼ������, ��" << meshes.size() << "������, " << textures_loaded.size() << "������" << std::endl;
	for (unsigned int i = 0; i < textures_loaded.size(); i++) {
		const Texture& texture = textures_loaded[i];
		std::cout << i << ": " << texture.type << std::endl;
	}
}

void Model::Draw(Shader& shader) {

	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].Draw(shader);
	}

}

void Model::loadModel(const std::string& path) {

	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));
	if (scene != nullptr) {
		processNode(scene->mRootNode, scene);
	}

}

void Model::processNode(aiNode* node, const aiScene* scene) {

	// �ӽڵ���������������л�ȡ������󣬴���ÿһ������
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		unsigned int meshIndex = node->mMeshes[i];
		aiMesh* mesh = scene->mMeshes[meshIndex];
		Mesh selfDefinedMesh = processMesh(mesh, scene);
		meshes.push_back(selfDefinedMesh);
	}
	// �ӽڵ���ӽڵ����������л�ȡ�ӽڵ���󣬵ݹ鴦��ÿһ���ӽڵ�
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		aiNode* childNode = node->mChildren[i];
		processNode(childNode, scene);
	}

}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {

	// ���ڹ���Mesh����
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// ������
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;

		// ����λ��
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector; // �����ǿ�����ֵ�����Ժ����޸�v1����Ӱ��vertex.Position

		// ���㷨��
		if (mesh->HasNormals()) {
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}

		// ����UV���꣬���ߣ������ߡ�assimp�����а����������ֻ꣬���ĵ�һ��
		if (mesh->mTextureCoords[0]) {
			// ��������
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
			// ����
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;
			// ������
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;
		}
		else {
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	// ����������
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) { // mesh�ж��ٸ���
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {  // ÿ�����ж��ٸ���������
			indices.push_back(face.mIndices[j]);
		}
	}

	// �������
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	// ����������
	std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// ���������
	std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// ��������
	std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// �߶�����
	std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	return Mesh(vertices, indices, textures);

}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {

	std::vector<Texture> textures;
	// �������������ÿһ��ָ�����͵�����
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString texturePath;
		mat->GetTexture(type, i, &texturePath);
		bool skip = false; // ��������Ѿ������ع�������������

		for (unsigned int j = 0; j < textures_loaded.size(); j++) {
			const Texture& loadedTexture = textures_loaded[j];

			if (strcmp(loadedTexture.path.c_str(), texturePath.C_Str()) == 0) {
				textures.push_back(loadedTexture);
				skip = true;
				break;
			}

		}

		if (!skip) {
			Texture texture;
			texture.id = TextureFromFile(texturePath.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = texturePath.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}

	};

	return textures;
}


unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
	std::string subPath = std::string(path);
	std::string filename = directory + '/' + subPath;
	std::cout << "loading texture: " << filename << std::endl;

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