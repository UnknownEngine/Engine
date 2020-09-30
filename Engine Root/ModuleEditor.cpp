#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"
#include <SDL/include/SDL.h>
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
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

update_status ModuleEditor::PostUpdate(float dt)
{
	//Begin new ImGui Frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {

			if (ImGui::MenuItem("Exit", NULL, false, true)) {
				return UPDATE_STOP;
			}
			ImGui::EndMenu();
		}


		ImGui::EndMainMenuBar();
	}

	ImGui::Begin("Holi",NULL);
	float width = ImGui::GetWindowContentRegionWidth();
	float height = ImGui::GetWindowHeight();
	if (ImGui::Button("Eudald", ImVec2(200, 40))) {
		LOG("Click");
	}
	ImGui::End();

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
