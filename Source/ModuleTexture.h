#pragma once
#include "Module.h"
#include "SDL.h"
#include "GL/glew.h"
#include <string>
#include "DirectXTex/DirectXTex.h"

struct TexID
{
	GLuint texID;
	std::string texPath;
};


class ModuleTexture :   public Module
{
public:
	ModuleTexture();
	~ModuleTexture();


	bool            Start();
	update_status   Update();
	bool            CleanUp();
	void LoadTex(std::string nameTexture, TexID& texData);

	static const std::string texFolderPath;

	GLuint GetTexture();

private:
	DirectX::ScratchImage* returnImage = new DirectX::ScratchImage;
	GLuint texture;
};

