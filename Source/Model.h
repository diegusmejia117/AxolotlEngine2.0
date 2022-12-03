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

	static Model* LoadFromFile(std::string i_fileName);
	std::vector<GLuint> m_textures;
	std::list<Mesh*> m_meshes;
	void Draw();

private:
	

	Model();

	void LoadMaterials(const aiScene* i_scene);
	void LoadMeshes(aiMesh** i_meshes, int i_numMeshes);

	
};

