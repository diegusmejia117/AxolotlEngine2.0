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
	inline void ClearConsole() {
		consoleLogs.clear();
	}
	std::vector<const char*> consoleLogs;

	void ModelUploadedWindow(const char* modelPath);

private:

	int fpsCaptures = 120;
	int currentIndex = 0;
	std::vector<float> fpsHistoric;
	const Model* model = nullptr;

	std::string sdlVersion;
	std::string cpusAndCache;
	std::string ram;
	std::string gpuVendor;
	std::string gpuBrand;
	std::vector<const char*> caps;
	
	bool scrollAuto;
	bool scrollEnd;
};
	



