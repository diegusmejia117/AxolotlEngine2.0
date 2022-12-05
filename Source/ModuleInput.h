#pragma once
#include "Module.h"
#include "Globals.h"
#include "Geometry/Frustum.h"


typedef unsigned __int8 Uint8;

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status Update();
	bool CleanUp();
	//char* droppedFile_dir;


	

private:
	const Uint8 *keyboard = NULL;
	//Frustum* GetFrustum();
	float velocity = 0.2f;
	float rotVelocity = 0.003f;
};