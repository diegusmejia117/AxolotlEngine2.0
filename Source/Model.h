#pragma once

#include <string>
#include <list>

#include "scene.h"

#include "Mesh.h"

class Model
{
public:
	~Model();

	static const std::string s_modelFolderPath;

	static Model* FileLoad(std::string i_fileName);
	std::vector<GLuint> materialTextures;
	std::list<Mesh*> meshes;
	void Draw();

	

private:
	

	Model();

	void LoadMaterials(const aiScene* i_scene);
	void LoadMeshes(aiMesh** i_meshes, int i_numMeshes);

	
};

