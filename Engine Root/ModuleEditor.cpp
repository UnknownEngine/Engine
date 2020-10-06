#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"
#include <SDL/include/SDL.h>
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <string>

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	showcase = false;
}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Init() 
{

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	/*io.ConfigViewportsNoDecoration = false;
	io.ConfigViewportsNoAutoMerge = true;
	io.ConfigWindowsMoveFromTitleBarOnly = true;
	io.ConfigDockingTransparentPayload = true;*/

	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[2] = ImVec4(0.2196079, 0.2196079, 0.2196079, 1);
	style.Colors[10] = ImVec4(0.1568628, 0.1568628, 0.1568628, 1);
	style.Colors[11] = ImVec4(0.1568628, 0.1568628, 0.1568628, 1);
	style.Colors[27] = ImVec4(0.09803922, 0.09803922, 0.09803922, 1);
	style.Colors[28] = ImVec4(0.09803922, 0.09803922, 0.09803922, 1);
	style.Colors[29] = ImVec4(0.09803922, 0.09803922, 0.09803922, 1);
	style.Colors[33] = ImVec4(0.1568628, 0.1568628, 0.1568628, 1);
	style.Colors[34] = ImVec4(0.1882353, 0.1882353, 0.1882353, 1);
	style.Colors[35] = ImVec4(0.2352941, 0.2352941, 0.2352941, 1);
	style.Colors[37] = ImVec4(0.1568628, 0.1568628, 0.1568628, 1);
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	//style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init();

	//io.IniFilename = "Engine/imgui.ini";
	io.MouseDrawCursor = false;

	return true;
}

bool ModuleEditor::Start()
{
	//frameWindowClass = new ImGuiWindowClass();
	//frameWindowClass->ClassId = 1;
	//frameWindowClass->DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoSplit;

	//normalWindowClass = new ImGuiWindowClass();
	//normalWindowClass->ClassId = 2;

	ImGuiIO& io = ImGui::GetIO();

	return true;
}

update_status ModuleEditor::PreUpdate(float dt) {
	//Begin new ImGui Frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);

	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate(float dt)
{

	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {

			if (ImGui::MenuItem("Exit", NULL, false, true)) {
				return UPDATE_STOP;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help")) {

			if (ImGui::MenuItem("Gui Demo"))
				showcase = !showcase;


			if (ImGui::MenuItem("Documentation"))
				App->RequestBrowser("https://github.com/UnknownEngine/Engine/wiki");

			if (ImGui::MenuItem("Download latest"))
				App->RequestBrowser("https://github.com/UnknownEngine/Engine/releases");

			if (ImGui::MenuItem("Report a bug"))
				App->RequestBrowser("https://github.com/UnknownEngine/Engine/issues");

			if (ImGui::MenuItem("About")) {
				ImGui::TextWrapped("# UnknownEngine Unkown Engine is our project for Engines subject at CITM's videogames design & development. This time, we will be working on the creation of the core of a videogames engine, focusing on basic level stuff in order to understand how a high level engine work.## Team* Jordi Pardo Gutiérrez* Eudald Garrofé Flix");
			}

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	//Application Window ------------------------------------
	if(ImGui::CollapsingHeader("Application"))
	{
		float width = ImGui::GetWindowContentRegionWidth();
		char title[25];

		if (ImGui::Button("Unknown Engine              ")) { App->RequestBrowser("https://github.com/UnknownEngine/Engine/wiki"); }
		ImGui::SameLine(220);
		ImGui::Text("Project Name");

		if (ImGui::Button("CITM                        ")) { App->RequestBrowser("https://www.citm.upc.edu/"); }
		ImVec2(100, 100);
		ImGui::Text("Organization");

		ImGui::SliderInt("FPS Cap", &test, 30, 60);

		sprintf_s(title, 25, "Framerate %.1f", App->frames_log[App->frames_log.size() - 1]);
		ImGui::PlotHistogram("##FPS Ratio:", &App->frames_log[0], App->frames_log.size(), 0, title, 0.0f, 100, ImVec2(310, 100));
		sprintf_s(title, 25, "Milliseconds %.1f", App->ms_log[App->ms_log.size() - 1]);
		ImGui::PlotHistogram("##Milliseconds:", &App->ms_log[0], App->ms_log.size(), 0, title, 0.0f, 100, ImVec2(310, 100));

	}
	//Window Window --------------------------
	if (ImGui::CollapsingHeader("Window"))
	{
		float brightness = SDL_GetWindowBrightness(App->window->window);

		if (App->window->resizable)
		{
			App->window->width_resize = ImGui::SliderInt("Width", &App->window->width, 1280, 1920);
			App->window->height_resize = ImGui::SliderInt("Height", &App->window->height, 1024, 1080);
		}
		bool bright_resize = ImGui::SliderFloat("Bright", &brightness, 1.f, 3.794f);

		ImGui::Text("");

		if (ImGui::Checkbox("Fullscreen", &App->window->fullscreen)) { App->window->SetFullScreen(App->window->fullscreen); }
		ImGui::SameLine(150);
		if (ImGui::Checkbox("Resizable", &App->window->resizable)) { App->window->SetResizability(App->window->resizable); }

		if (ImGui::Checkbox("Borderless", &App->window->borderless)) { App->window->SetBorderless(App->window->borderless); }
		ImGui::SameLine(150);
		if (ImGui::Checkbox("Desktop", &App->window->desktop)) { App->window->SetFullDesktop(App->window->desktop); }


		if (App->window->width_resize || App->window->height_resize)
		{
			SDL_SetWindowSize(App->window->window, App->window->width, App->window->height);
			glViewport(0, 0, App->window->width, App->window->height);
		}
		else if (bright_resize)
		{
			SDL_SetWindowBrightness(App->window->window, brightness);
		}
	}

	//Hardware Window
	if (ImGui::CollapsingHeader("Hardware"))
	{
		ImGui::Text("CPU Cache Line: %d", SDL_GetCPUCacheLineSize());
		ImGui::Text("Number of CPUs cores: %d", SDL_GetCPUCount());
		ImGui::Text("System RAM: %d", SDL_GetSystemRAM());
		ImGui::Text("Caps:  ");
		std::string caps;
		if (SDL_Has3DNow() == SDL_TRUE) { std::string threednow = "3DNow";		caps += threednow; caps += ", "; }
		if (SDL_HasAVX() == SDL_TRUE) { std::string avx = "AVX";				caps += avx; caps += ", "; }
		if (SDL_HasAVX2() == SDL_TRUE) { std::string avx2 = "AVX2";				caps += avx2; caps += ", "; }
		if (SDL_HasAltiVec() == SDL_TRUE) { std::string altivec = "AltiVec";	caps += altivec; caps += ", "; }
		if (SDL_HasMMX() == SDL_TRUE) { std::string mmx = "MMX";				caps += mmx; caps += ", "; }
		if (SDL_HasRDTSC() == SDL_TRUE) { std::string rdtsc = "RDTSC";			caps += rdtsc; caps += ", "; }
		if (SDL_HasSSE() == SDL_TRUE) { std::string sse = "SSE";				caps += sse; caps += ", "; }
		if (SDL_HasSSE2() == SDL_TRUE) { std::string sse2 = "SSE2";				caps += sse2; caps += ", "; }
		if (SDL_HasSSE3() == SDL_TRUE) { std::string sse3 = "SSE3";				caps += sse3; caps += ", "; }
		if (SDL_HasSSE41() == SDL_TRUE) { std::string sse41 = "SSE41";			caps += sse41; caps += ", "; }
		if (SDL_HasSSE42() == SDL_TRUE) { std::string sse42 = "SSE42";			caps += sse42; caps += ", "; }

		ImGui::SameLine(50);
		
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(233, 233, 43)));
		ImGui::TextWrapped(caps.c_str());
		ImGui::PopStyleColor(1);
		ImGui::Text("----------------------------------------------------------");
		const char* vendor = (const char*)glGetString(GL_VENDOR);
		ImGui::Text("System RAM: "); 	ImGui::SameLine(100);
		ImGui::TextWrapped(vendor);

		const char* model = (const char*)glGetString(GL_RENDERER);
		ImGui::Text("Driver Model: "); 	ImGui::SameLine(100);
		ImGui::TextWrapped(model);
	}
	if (ImGui::CollapsingHeader("Input"))
	{
		ImGui::TextUnformatted(input_log.begin());
		if (scroll)
		{
			ImGui::SetScrollHere(1.0f);
		}
		scroll = false;
	}
	if (ImGui::CollapsingHeader("Console"))
	{

	}

	if (showcase)
		ImGui::ShowDemoWindow();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update(float dt)
{

	return UPDATE_CONTINUE;
}


bool ModuleEditor::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	return true;
}
