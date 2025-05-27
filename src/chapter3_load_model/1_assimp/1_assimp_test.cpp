#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <iostream>

void LoadFinish(const aiScene* scene)
{
	std::cout << "LoadFinish ! NumVertices : " << (*(scene->mMeshes))->mNumVertices << std::endl;
}

bool LoadModel(const std::string& pFile)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(pFile,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if (!scene)
	{
		std::cout << importer.GetErrorString() << std::endl;
		return false;
	}

	LoadFinish(scene);

	return true;
}

int main()
{
	LoadModel("./resources/m.ply");

	return 0;
}
