#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	// Called before quitting
	bool Init();

	// Called before quitting
	bool CleanUp();

	void SetFullScreen(bool fs);
	void SetBorderless(bool br);
	void SetResizable(bool resize);

	/*void SetWidth(int width);
	void SetHeight(int height);*/

public:
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;
};

#endif // __ModuleWindow_H__