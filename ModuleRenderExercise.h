#pragma once
#include "Module.h"
#include "Globals.h"
#include "Math/float4x4.h"
#include "Model.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRenderExercise : public Module
{
public:
	ModuleRenderExercise();
	~ModuleRenderExercise();

	inline Model* GetModel3D() {
		return model3D;
	}


	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void WindowResized(unsigned width, unsigned height);
	unsigned CreateTriangleVBO();
	void DestroyVBO(unsigned vbo);
	unsigned GetProgram();
	void SetModel(const char* modelPath);
	//void SetModelActive(const Model* newModel);
private:
	unsigned vbo;
	unsigned program;
	Model* model3D;
};

