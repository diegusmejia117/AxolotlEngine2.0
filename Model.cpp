#include "Model.h"

#include <vector>

#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleTexture.h"


#include "cimport.h"
#include "postprocess.h"



Model::Model(std::string filename)
{


}

Model::~Model()
{
	for (int i = 0; i < materialID.size(); i++) {
		glDeleteTextures(1, &materialID[i].texID);
	}

	for (int i = 0; i < meshes.size(); i++) {
		delete meshes[i];
	}
}

Model* Model::FileLoad(const char* fileName)
{
	Model* model = new Model(fileName);
	const char* meshName = fileName;
	
	const aiScene* scene = aiImportFile(fileName, aiProcessPreset_TargetRealtime_MaxQuality);
	
	if (scene)
	{
		model->LoadMaterials(scene);
		model->LoadMeshes(scene->mMeshes, scene->mNumMeshes);
		aiReleaseImport(scene);
		
		return model;
	}
	else
	{
		
		App->editor->consoleLogs.emplace_back("errorLoading");
		return nullptr;
	}
}

void Model::Draw()
{
	for (std::vector<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it) {
		(*it)->Draw(materialTextures);
	}
}

void Model::LoadMaterials(const aiScene* scene)
{
	aiString file;
	materialTextures.reserve(scene->mNumMaterials);
	App->editor->consoleLogs.emplace_back("Loading textures...");

	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		
		if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &file) == AI_SUCCESS)
		{
			std::string texName = std::string(file.data);
			const size_t lastSlashIdx = texName.find_last_of("\\/");
			if (std::string::npos != lastSlashIdx)
			{
				texName.erase(0, lastSlashIdx + 1);
			}
			texName = "assets/" + texName;
			TexID texData;
			App->texture->LoadTex(texName.c_str(), texData);
			materialID.push_back(texData);
			App->editor->consoleLogs.emplace_back("Loaded texture");
			
		}
	}
}

void Model::LoadMeshes(aiMesh** i_meshes, int i_numMeshes)
{
	App->editor->consoleLogs.emplace_back("Loading mesh...");
	for (int i = 0; i < i_numMeshes; ++i) {
		Mesh* mesh = Mesh::Load(i_meshes[i]);
		meshes.push_back(mesh);
	}
	App->editor->consoleLogs.emplace_back("Loading mesh...");
}

