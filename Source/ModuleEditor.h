#pragma once
#include "Module.h"
#include "Globals.h"
#include "Model.h"
#include <vector>
#include <string>

typedef unsigned __int8 Uint8;

class ModuleEditor : public Module
{
public:

	ModuleEditor();
	~ModuleEditor();

	bool Init();
	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

private:

	int fpsCaptures = 120;
	int currentIndex = 0;
	std::vector<float> fpsHistoric;
	const Model* m_model = nullptr;

	std::string m_sdlVersion;
	std::string m_cpusAndCache;
	std::string m_ram;
	std::string m_gpuVendor;
	std::string m_gpuBrand;
};
	



