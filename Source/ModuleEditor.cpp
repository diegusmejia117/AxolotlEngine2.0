#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleRenderExercise.h"
#include "ModuleCamera.h"
#include "ModuleInput.h"
#include "Mesh.h"
#include "Model.h"


#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "GL/glew.h"
#include <stdio.h>
#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

ModuleEditor::ModuleEditor()
{
}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Init()
{
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_MakeCurrent(App->window->window, App->renderer->context);
	SDL_GL_SetSwapInterval(1); // Enable vsync
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
	io.ConfigWindowsMoveFromTitleBarOnly = true;

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		io.ConfigViewportsNoAutoMerge = false;
		io.ConfigViewportsNoTaskBarIcon = true;
	}

	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
		io.ConfigDockingTransparentPayload = true;
	}

	fpsHistoric = std::vector<float>(fpsCaptures);

	//SDL HARDWARE ACCESS

	sdlVersion = std::to_string(SDL_MAJOR_VERSION) + "." + std::to_string(SDL_MINOR_VERSION) + "." + std::to_string(SDL_PATCHLEVEL);

	int cacheSizeInB = SDL_GetCPUCacheLineSize();
	int cacheSizeInKB = std::ceil(cacheSizeInB / 1000.f);
	int cacheSizeInKb = cacheSizeInKB * 8;
	cpusAndCache = std::to_string(SDL_GetCPUCount()) + " (Cache: " + std::to_string(cacheSizeInKb) + "kb)";

	int ramInMB = SDL_GetSystemRAM();
	float ramInGB = ramInMB / 1000.f;
	float ramInGb = ramInGB * 8.f;

	int ramInGbOneDecimalAux = std::ceil(ramInGb * 10.f);
	std::string ramInGbOneDecimal = std::to_string(ramInGbOneDecimalAux);

	ramInGbOneDecimal.insert(ramInGbOneDecimal.length() - 1, ".");
	ram = ramInGbOneDecimal + "Gb";

	
	if (SDL_Has3DNow()) {
		caps.emplace_back("3DNow");
	}
	if (SDL_HasAltiVec()) {
		caps.emplace_back("AltiVec");
	}
	if (SDL_HasAVX()) {
		caps.emplace_back("AVX");
	}
	if (SDL_HasAVX2()) {
		caps.emplace_back("AVX2");
	}
	if (SDL_HasMMX()) {
		caps.emplace_back("MMX");
	}
	if (SDL_HasRDTSC()) {
		caps.emplace_back("RDTSC");
	}
	if (SDL_HasSSE()) {
		caps.emplace_back("SSE");
	}
	if (SDL_HasSSE2()) {
		caps.emplace_back("SSE2");
	}
	if (SDL_HasSSE3()) {
		caps.emplace_back("SSE3");
	}
	if (SDL_HasSSE41()) {
		caps.emplace_back("SSE41");
	}
	if (SDL_HasSSE42()) {
		caps.emplace_back("SSE42");
	}

	


	return true;
}

bool ModuleEditor::Start()
{
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->context);
	ImGui_ImplOpenGL3_Init(GLSL_VERSION);

	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.0f, 1.0f, 1.0f, 0.10f));
	ImGui::PushStyleColor(ImGuiCol_PlotHistogram, (ImVec4)ImColor(0.54f, 0.43f, 0.12f, 0.97f));

	//GPU STARTUP INFO
	char glVendor[128];
	sprintf(glVendor, "%s", glGetString(GL_VENDOR));
	gpuVendor = _strdup(glVendor);
	char glRenderer[128];
	sprintf(glRenderer, "%s", glGetString(GL_RENDERER));
	gpuBrand = _strdup(glRenderer);

	return true;
}



update_status ModuleEditor::PreUpdate()
{
	/*bool show_demo_window = true;
	bool show_another_window = false;*/


	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		if (event.type == SDL_QUIT)
			return UPDATE_STOP;
		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(App->window->window))
			return UPDATE_STOP;
	}


	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();



	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update()
{

	

	static float f = 0.0f;
	static int counter = 0;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	
	static bool showTerminal = true;
	static bool showConfig = true;
	static bool showMeshInfo = true;
	static bool showFPS = true;
	static bool showHardwareInfo = true;
	static bool showAbout = true;
	static bool showConsole = true;

	static bool fullScreen = false;
	static bool reSize = true;
	static bool borderLess = false;

	
	Model* model3D = App->renderer2->GetModel3D();
	


	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Window"))
		{

			if (ImGui::MenuItem("Enable", "MeshInfo"))
			{ 
				if (showMeshInfo)
				{
					showMeshInfo = false;
				}
				else
				{
					showMeshInfo = true;
				}
			}

			if (ImGui::MenuItem("Enable", "FPS"))
			{
				if (showFPS)
				{
					showFPS = false;
				}
				else
				{
					showFPS = true;
				}
			}

			if (ImGui::MenuItem("Enable", "Hardware Info"))
			{
				if (showHardwareInfo)
				{
					showHardwareInfo = false;
				}
				else
				{
					showHardwareInfo = true;
				}
			}

			if (ImGui::MenuItem("Enable", "About"))
			{
				if (showAbout)
				{
					showAbout = false;
				}
				else
				{
					showAbout = true;
				}
			}

			if (ImGui::MenuItem("Enable", "Console"))
			{
				if (showConsole)
				{
					showConsole = false;
				}
				else
				{
					showConsole = true;
				}
			}


			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Controls"))
		{
			if (ImGui::MenuItem("Move", "WASD")) { }
			if (ImGui::MenuItem("Rotate Camera", "Directional Arrows")) {}
			if (ImGui::MenuItem("Increase Speed", "Hold LSHIFT + WASD")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Pan Camera", "MiddleMouseButton + Mouse Movement")) {}
			if (ImGui::MenuItem("Zoom", "Mouse Wheel Scroll + UP/DOWN")) {}

			ImGui::EndMenu();

			ImGui::EndMainMenuBar();
		}
	}

	//ImGui::ShowDemoWindow();

	ImGui::SetNextWindowSize(ImVec2(420, 500), ImGuiCond_Always);

	if (showTerminal)
	{

		if (ImGui::Begin("Axolotl Engine Terminal", &showTerminal)) {

			if (showConfig)
			{
				if (ImGui::CollapsingHeader("Configuration", &showConfig))
				{
					ImGui::Checkbox("MeshInfo", &showMeshInfo);
					ImGui::SameLine();
					ImGui::Checkbox("FPS", &showFPS);
					ImGui::SameLine();
					ImGui::Checkbox("Hardware Info", &showHardwareInfo);
					ImGui::SameLine();
					ImGui::Checkbox("About", &showAbout);
					ImGui::SameLine();
					ImGui::Checkbox("Console", &showConsole);

					if (ImGui::Checkbox("Fullscreen", &fullScreen))
					{
						App->window->SetFullScreen(fullScreen);
					}
					ImGui::SameLine();

					if (ImGui::Checkbox("Resizable", &reSize))
					{
						App->window->SetFullScreen(reSize);
					}
					ImGui::SameLine();

					if (ImGui::Checkbox("Borderless", &borderLess))
					{
						App->window->SetFullScreen(borderLess);
					}

				}

			}

			if (showMeshInfo)
			{
				if (ImGui::CollapsingHeader("Mesh INFO", &showMeshInfo))
				{

					ImGui::Text("Model Name");
					ImGui::SameLine();
					//ImGui::Text(model3D->GetMeshName());
					//ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), model3D->GetMeshName());

					for (Mesh* mesh : model3D->GetMeshInfo())
					{
						ImGui::Text("Vertex Number:");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "%u", mesh->GetNumVertices());
						ImGui::Text("Index Number:");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "%u", mesh->GetNumIndices());
						ImGui::Text("Material Index:");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "%u", mesh->GetMaterialIndex());
					}

				}
			}

			if (showFPS)
			{

				if (ImGui::CollapsingHeader("FPS", &showFPS))
				{
					ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
					float maxFPS = App->GetFPS();
					ImGui::SliderFloat("Max FPS", &maxFPS, 0.f, 120.f, "%.1f", ImGuiSliderFlags_AlwaysClamp);
					App->SetFPS(maxFPS);

					int deltaTime = App->GetDeltaTime();
					fpsHistoric[currentIndex] = 1000.f / deltaTime;

					char title[50];
					sprintf_s(title, 50, "Framerate %.1f", fpsHistoric[currentIndex]);
					ImGui::PlotHistogram("##framerate", &fpsHistoric[0], fpsCaptures, 0, title, 0.0f, 120.f, ImVec2(310, 100));

					if (currentIndex < fpsCaptures - 1)
					{
						++currentIndex;
					}
					else
					{

						fpsHistoric.erase(fpsHistoric.begin());
						fpsHistoric.push_back(0);
					}
				}
			}

			if (showHardwareInfo)
			{

				if (ImGui::CollapsingHeader("HARDWARE INFO", &showHardwareInfo))
				{


					ImGui::Text(("CPU: " + cpusAndCache).c_str());
					ImGui::Text(("System RAM: " + ram).c_str());
					ImGui::Text(("GPU: " + gpuVendor).c_str());
					ImGui::Text(("Brand: " + gpuBrand).c_str());
					ImGui::Separator();
					ImGui::Text(("SDL Version: " + sdlVersion).c_str());
					ImGui::Separator();
					ImGui::Text(("Caps"));
					for (int i = 0; i < caps.size(); i++)
					{
						if (i % 4 == 0 && i != 0)
						{
							ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "%s", caps[i]);
						}
						else
						{
							ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "%s", caps[i]);
							ImGui::SameLine();
						}
					}
				}
			}


			if (showAbout)
			{
				if (ImGui::CollapsingHeader("About", &showAbout))
				{
					ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Axolotl Engine");
					ImGui::SameLine();
					ImGui::Text("made by Diego Aaron Mejia Ramirez");

					ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Barcelona, Spain");
					ImGui::SameLine();
					ImGui::Text("2022");

					ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Github repository:");
					
					ImGui::Text("https://github.com/diegusmejia117/AxolotlEngine2.0");

				}
			}
			ImGui::EndMenu();


		}


		ImGui::SetNextWindowSize(ImVec2(420, 200), ImGuiCond_Always);

		if (showConsole)
		{

			if (ImGui::Begin("Console", &showConsole))
			{
				if (ImGui::BeginPopup("Options"))
				{
					ImGui::Checkbox("Auto", &scrollAuto);
					ImGui::EndPopup();
				}

				if (ImGui::Button("Clear console")) { App->editor->ClearConsole(); }
				ImGui::SameLine();
				bool copy_to_clipboard = ImGui::Button("Copy to clipboard");

				ImGui::Separator();





				const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
				if (ImGui::BeginChild("Console Section", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar))
				{


					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(3, 1));
					if (copy_to_clipboard)
						ImGui::LogToClipboard();
					for (int i = 0; i < consoleLogs.size(); i++)
					{
						const char* log = consoleLogs[i];

						ImVec4 color;
						bool is_colored = false;
						if (strstr(log, "/error/")) { color = ImVec4(2.0f, 0.8f, 0.8f, 2.0f); is_colored = true; }
						else if (strncmp(log, "# ", 2) == 0) { color = ImVec4(2.0f, 0.2f, 0.2f, 2.0f); is_colored = true; }
						if (is_colored)
							ImGui::PushStyleColor(ImGuiCol_Text, color);
						ImGui::TextUnformatted(log);
						if (is_colored)
							ImGui::PopStyleColor();
					}
					if (copy_to_clipboard)
						ImGui::LogFinish();

					if (scrollEnd || (scrollAuto && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
						ImGui::SetScrollHereY(1.0f);
					scrollEnd = false;

					ImGui::PopStyleVar();


				}

				ImGui::EndChild();
			}

		}




		ImGui::EndMenu();

		ImGui::End();

		return UPDATE_CONTINUE;

	}
}

update_status ModuleEditor::PostUpdate()
{


	ImGuiIO& io = ImGui::GetIO(); (void)io;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);



	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	SDL_GL_MakeCurrent(App->window->window, App->renderer->context);

	return UPDATE_CONTINUE;
}




bool ModuleEditor::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	return false;
}
