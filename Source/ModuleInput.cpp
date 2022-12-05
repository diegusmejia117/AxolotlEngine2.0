#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "ModuleRenderExercise.h"
#include "ModuleEditor.h"

#include "SDL.h"

#include "imgui_impl_sdl.h"


ModuleInput::ModuleInput()
{}


ModuleInput::~ModuleInput()
{}

// Called before render is available
bool ModuleInput::Init()
{

	bool ret = true;
	SDL_Init(0);
	SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
	
	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::Update()
{
    SDL_Event sdlEvent;
	const Uint8* keyboard = SDL_GetKeyboardState(NULL);
	Uint32 SDL_GetMouseState(int* x, int* y);
	
	float3 deltaPos = float3::zero;
	float3 deltaRot = float3::zero;
	char* droppedFile_dir;

    while (SDL_PollEvent(&sdlEvent) != 0)
    {
		ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
        switch (sdlEvent.type)
        {
		
            case SDL_QUIT:
                return UPDATE_STOP;

				break;
            case SDL_WINDOWEVENT:
                if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED || sdlEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                    App->renderer->WindowResized(sdlEvent.window.data1, sdlEvent.window.data2);

				if (sdlEvent.window.event == SDL_WINDOWEVENT_CLOSE) {
					return UPDATE_STOP;
				}
                break;


			case SDL_MOUSEBUTTONUP:
				
				if (sdlEvent.button.button == SDL_BUTTON_RIGHT) {
					deltaPos.z -= 0.2f;
					App->camera->Translate(deltaPos);
					
				}
				
				break;

		    case SDL_MOUSEWHEEL:

			if (sdlEvent.wheel.y > 0) // scroll up
			{
				deltaPos.z -= 0.2f;
				App->camera->Translate(deltaPos);
			}

			else if (sdlEvent.wheel.y < 0) // scroll down
			{
				deltaPos.z += 0.2f;
				App->camera->Translate(deltaPos);
			}

			break;


			case SDL_DROPFILE:

				droppedFile_dir = sdlEvent.drop.file;
				App->renderer2->SetModel(droppedFile_dir);
				App->editor->ModelUploadedWindow(droppedFile_dir);
				printf("%s\n", droppedFile_dir);
				SDL_free(droppedFile_dir);

			break;
				
		
			
        }
		
		
		
    
	}


	if (keyboard[SDL_SCANCODE_W]) {

		if (keyboard[SDL_SCANCODE_LSHIFT]) 
		{
			deltaPos.z -= velocity * 2;
			App->camera->Translate(deltaPos);
		}
		else 
		{
			deltaPos.z -= velocity;
			App->camera->Translate(deltaPos);
		}
	}
	if (keyboard[SDL_SCANCODE_S]) {

		if (keyboard[SDL_SCANCODE_LSHIFT])
		{
			deltaPos.z += velocity * 2;
			App->camera->Translate(deltaPos);
		}
		else
		{
			deltaPos.z += velocity;
			App->camera->Translate(deltaPos);
		}
	}
	if (keyboard[SDL_SCANCODE_Q]) {
		if (keyboard[SDL_SCANCODE_LSHIFT]) {
			deltaPos.y += velocity * 2;
			App->camera->Translate(deltaPos);
		}
		else 
		{
			deltaPos.y += velocity;
			App->camera->Translate(deltaPos);
		}
	}
	if (keyboard[SDL_SCANCODE_E]) {
		if(keyboard[SDL_SCANCODE_LSHIFT])
		{
			deltaPos.y -= velocity * 2;
			App->camera->Translate(deltaPos);

		}
		else 
		{
			deltaPos.y -= velocity;
			App->camera->Translate(deltaPos);
		}
	}
	if (keyboard[SDL_SCANCODE_D]) {
		if (keyboard[SDL_SCANCODE_LSHIFT])
		{
			deltaPos.x += velocity * 2;
			App->camera->Translate(deltaPos);
		
		}
		else 
		{
			deltaPos.x += velocity;
			App->camera->Translate(deltaPos);
		}
	}
	if (keyboard[SDL_SCANCODE_A]) {
		if (keyboard[SDL_SCANCODE_LSHIFT])
		{
			deltaPos.x -= velocity * 2;
			App->camera->Translate(deltaPos);

		}
		else
		{
			deltaPos.x -= velocity;
			App->camera->Translate(deltaPos);
		}
	}

	if (keyboard[SDL_SCANCODE_F]) {
		App->camera->Focus();
	}
	

	//rotate camera

	
		if (keyboard[SDL_SCANCODE_LEFT]) {
			deltaRot.y += rotVelocity;
			App->camera->Rotate(deltaRot);
		}
		if (keyboard[SDL_SCANCODE_RIGHT]) {
			deltaRot.y -= rotVelocity;
			App->camera->Rotate(deltaRot);
		}
		if (keyboard[SDL_SCANCODE_UP]) {
			deltaRot.x += rotVelocity;
			App->camera->Rotate(deltaRot);
		}
		if (keyboard[SDL_SCANCODE_DOWN]) {
			deltaRot.x -= rotVelocity;
			App->camera->Rotate(deltaRot);
		}
	
	
	
	
    return UPDATE_CONTINUE;
}


bool ModuleInput::CleanUp()
{
	//LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}
