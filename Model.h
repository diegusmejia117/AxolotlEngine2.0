#pragma once

#include <string>
#include <list>
#include <vector>
#include "scene.h"
#include "ModuleTexture.h"
#include "Mesh.h"

class Model
{
public:
	~Model();

	static const std::string s_modelFolderPath;

	  
	//static Model* FileLoad(std::string i_fileName);
	static Model* FileLoad(const char* fileName);

	std::vector<GLuint> materialTextures;
	std::vector<TexID> materialID;
	std::vector<Mesh*> meshes;
	void Draw();
	
	

private:
	

	Model();

	void LoadMaterials(const aiScene* i_scene);
	void LoadMeshes(aiMesh** i_meshes, int i_numMeshes);

	
};

