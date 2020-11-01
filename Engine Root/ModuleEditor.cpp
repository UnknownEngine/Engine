#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include <Glew/include/glew.h>
#include "MathGeoLib/include/MathGeoLibFwd.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"
#include <SDL/include/SDL.h>
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <string>
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "MaterialComponent.h"

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	showcase = false;
	showAboutWindow = false;
	showApplicationWindow = false;
	showWindowWindow = false;
	showInputWindow = false;
	showHardwareWindow = false;
	showConsoleWindow = false;
	showOpenGLWindow = false;
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
	AddLog("Hola");

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
	//uint tex = App->renderer3D->fb;
	//ImGui::Begin("Viewport");
	//ImGui::Image(ImTextureID(tex), ImVec2(512, 512));
	//ImGui::End();
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

			if (ImGui::MenuItem("About",nullptr,nullptr,&showAboutWindow)) 
				showAboutWindow = !showAboutWindow;
				
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Settings"))
		{
			if (ImGui::MenuItem("Unfold All"))
			{
				showWindowWindow = !showWindowWindow;
				showApplicationWindow = !showApplicationWindow;
				showInputWindow = !showInputWindow;
				showConsoleWindow = !showConsoleWindow;
				showOpenGLWindow = !showOpenGLWindow;
			}

			if (ImGui::MenuItem("Application"))
				showApplicationWindow = !showApplicationWindow;

			if (ImGui::MenuItem("Window"))
				showWindowWindow = !showWindowWindow;

			if (ImGui::MenuItem("Hardware"))
				showHardwareWindow = !showHardwareWindow;

			if (ImGui::MenuItem("Input"))
				showInputWindow = !showInputWindow;
			
			if (ImGui::MenuItem("Console"))
				showConsoleWindow = !showConsoleWindow;

			if (ImGui::MenuItem("OpenGL Settings"))
				showOpenGLWindow = !showOpenGLWindow;

			ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
	}
	ImGui::Begin("Hierarchy");

	CreateHierarchy(App->geometry->firstGameObject);
	ImGui::End();

	if (ImGui::Begin("Inspector"))
	{
		if (App->scene_intro->selected != NULL)
		{
			FillInspector();
		}
		DrawInspector();
		
		ImGui::End();
	}
		
	if (showApplicationWindow)
		ApplicationWindow();
	if (showWindowWindow)
		WindowWindow();
	if (showHardwareWindow)
		HardwareWindow();
	if (showOpenGLWindow)
		OpenGLSettings();
	if (showInputWindow)
		InputWindow();
	if (showConsoleWindow)
		ConsoleWindow();

	if (showAboutWindow)
		AboutWindow();

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

void ModuleEditor::AddLog(char* path)
{
	items.push_back(strdup(path));
}



void ModuleEditor::DrawInspector()
{
	ImGui::CollapsingHeader("Transform");
	ImGui::PushItemWidth(60);
	ImGui::Text("");

	ImGui::Text("Position:");
	ImGui::SameLine(75.f);
	ImGui::InputText("##posx", tposx, 64);
	ImGui::SameLine(142.f);
	ImGui::InputText("##posy", tposy, 64);
	ImGui::SameLine(208.f);
	ImGui::InputText(" ##posz", tposz, 64);

	ImGui::Text("");
	ImGui::Separator();
	ImGui::Text("");

	ImGui::Text("Rotation:");
	ImGui::SameLine(75.f);
	ImGui::SliderFloat("##rotx", &rotx, -360, 360, "X: %1.f", 0.5f);
	ImGui::SameLine(142.f);
	ImGui::SliderFloat("##roty", &roty, -360, 360, "Y: %1.f", 0.5f);
	ImGui::SameLine(208.f);
	ImGui::SliderFloat("##rotz", &rotz, -360, 360, "Z: %1.f", 0.5f);

	ImGui::Text("");
	ImGui::Separator();
	ImGui::Text("");

	ImGui::Text("Scale:");
	ImGui::SameLine(75.f);
	ImGui::InputText("##scalex", tscalex, 64);
	ImGui::SameLine(142.f);
	ImGui::InputText("##scaley", tscaley, 64);
	ImGui::SameLine(208.f);
	ImGui::InputText("##scalez", tscalez, 64);

	ImGui::PopItemWidth();

	ImGui::Text("");
	ImGui::Separator();
	ImGui::Text("");

	ImGui::Text("");
	ImGui::SameLine(72.f);
	ImGui::Button("Reset Transforms", ImVec2(200, 20));
	ImGui::Text("");



	ImGui::CollapsingHeader("Mesh");

	ImGui::TextWrapped("Name");
	ImGui::SameLine(100);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(233, 233, 43)));
	ImGui::TextWrapped(meshname.c_str());
	ImGui::PopStyleColor(1);

	ImGui::TextWrapped("Path");
	ImGui::SameLine(100);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(233, 233, 43)));
	ImGui::TextWrapped(meshpath.c_str());
	ImGui::PopStyleColor(1);

	ImGui::Button("Change Source", ImVec2(200, 20));

	ImGui::TextWrapped("Vertexs");
	ImGui::SameLine(100);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(233, 233, 43)));
	ImGui::TextWrapped("%i",numvertices);
	ImGui::PopStyleColor(1);

	ImGui::TextWrapped("Normals");
	ImGui::SameLine(100);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(233, 233, 43)));
	ImGui::TextWrapped("%i",numnormals);
	ImGui::PopStyleColor(1);
	ImGui::Text("");

	ImGui::Text("");
	ImGui::SameLine(72.f);
	ImGui::Button("Delete Component", ImVec2(200, 20));
	ImGui::Text("");

	ImGui::CollapsingHeader("Material");

	ImGui::Checkbox("Active##material", &IsActive);

	ImGui::TextWrapped("File Path:");
	ImGui::SameLine(100);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(233, 233, 43)));
	ImGui::TextWrapped(mpath.c_str());
	ImGui::PopStyleColor(1);

	ImGui::Button("Change Source", ImVec2(200, 20));

	ImGui::PushItemWidth(95);
	ImGui::Text("");

	ImGui::Text("");
	ImGui::SameLine(72.f);
	ImGui::Button("Delete Component", ImVec2(200, 20));

	ImGui::Text("");
	ImGui::Separator();
	ImGui::Text("");

	ImGui::Text("Size: (%i,%i)", mwidth, mheight);
	ImGui::Text("Bpp: %i", mbpp);
	if (mbuffer != 0)
	{
		ImGui::Image((void*)(intptr_t)mbuffer, ImVec2(256, 256));
	}
}

void ModuleEditor::FillInspector()
{
	TransformComponent* transformComponent = App->scene_intro->selected->GetTransformComponent();
	MaterialComponent* materialComponent = App->scene_intro->selected->GetMaterialComponent();
	MeshComponent* meshComponent = App->scene_intro->selected->GetMeshComponent();

	if (transformComponent != NULL)
	{
		float tfposx = transformComponent->position.x;
		float tfposy = transformComponent->position.y;
		float tfposz = transformComponent->position.z;
		sprintf(tposx, "%f", tfposx);
		sprintf(tposy, "%f", tfposy);
		sprintf(tposz, "%f", tfposz);


		float tfrotx = transformComponent->rotation.x;
		float tfroty = transformComponent->rotation.y;
		float tfrotz = transformComponent->rotation.z;
		rotx = tfrotx;
		roty = tfroty;
		rotz = tfrotz;

		float tfscalex = transformComponent->scale.x;
		float tfscaley = transformComponent->scale.y;
		float tfscalez = transformComponent->scale.z;
		sprintf(tscalex, "%f", tfscalex);
		sprintf(tscaley, "%f", tfscaley);
		sprintf(tscalez, "%f", tfscalez);
	}
	else
	{
		AddLog("Game Object does not have a transform component!");
	}
	if (materialComponent != nullptr)
	{
		mname = materialComponent->name;
		mpath = materialComponent->path;
		mwidth = materialComponent->width;
		mheight = materialComponent->height;
		mbpp = materialComponent->bpp;
		mbuffer = materialComponent->bufferTexture;
	}
	else
	{
		AddLog("Game Object does not have a material component!");
	}
	if (meshComponent != nullptr)
	{
		numvertices = meshComponent->num_vertices;
		numnormals = meshComponent->num_normals;
		meshname = meshComponent->name;
		meshpath = meshComponent->path;
	}
	else
	{
		AddLog("Game Object does not have a mesh component!");
	}
}

void ModuleEditor::PrintConsole()
{
	ImGui::Text("----- Console -----");
	for (uint i = 0; i < items.size(); i++)
	{
		ImGui::TextWrapped(items[i]);
	}
}

void ModuleEditor::CleanConsole() 
{
	for (int i = 0; i < items.size(); i++)
		free(items[i]);
	items.clear();
}

void ModuleEditor::AboutWindow() 
{	
	if (ImGui::Begin("About",&showAboutWindow)) {
		ImGui::TextWrapped("Unknown Engine v0.1");
		HyperLink("Unkown Engine Github", "https://github.com/UnknownEngine/Engine/wiki");
		ImGui::Spacing();
		ImGui::TextWrapped("UnknownEngine Unkown Engine is our project for Engines subject at CITM's videogames design & development."
			"This time, we will be working on the creation of the core of a videogames engine, focusing on basic level stuff in order to understand how a high level engine work. ");
		ImGui::Spacing();
		ImGui::TextWrapped("Team");
		ImGui::SameLine();
		ImGui::TextWrapped("Jordi Pardo Gutierrez");
		HyperLink("Jordi's Github", "https://github.com/Jordi-Pardo");
		ImGui::SameLine();
		ImGui::TextWrapped("and");
		ImGui::SameLine();
		ImGui::TextWrapped("Eudald Garrofe Flix");
		HyperLink("Eudald's Github", "https://github.com/Hevne");
		ImGui::Spacing();
		SDL_version linked;
		SDL_VERSION(&linked);
		ImGui::BulletText("SDL %d.%d.%d",linked.major,linked.minor,linked.patch);
		HyperLink("SDL Web", "https://www.libsdl.org");
		ImGui::BulletText("GLEW  %s",glewGetString(GLEW_VERSION));
		HyperLink("Glew Web", "http://glew.sourceforge.net/");
		ImGui::BulletText("OpenGL %s",glGetString(GL_VERSION));
		HyperLink("OpenGL Web", "https://www.opengl.org/");
		ImGui::BulletText("ImGui %s", ImGui::GetVersion());
		HyperLink("ImGui Web", "https://github.com/ocornut/imgui");
		ImGui::BulletText("MathGeoLib 1.5");
		HyperLink("MathGeoLib Web", "https://github.com/juj/MathGeoLib/tags");


		ImGui::TextWrapped("\n\nLicense:");
		ImGui::TextWrapped("\nMIT License");
		ImGui::Spacing();
		ImGui::TextWrapped("Copyright 2020 Eudald Garrofe & Jordi Pardo\n\n"
			
			"Permission is hereby granted, free of charge, to any person obtaining a copy of this softwareand associated"
			"documentation files(the 'Software'), to deal in the Software without restriction, including without limitation"
			"the rights to use, copy, modify, merge, publish, distribute, sublicense, and /or sell copies of the Software,"
			"to permit persons to whom the Software is furnished to do so, subject to the following conditions :\n\n"

			"The above copyright noticeand this permission notice shall be included in all copies or substantial portions"
			"of the Software.\n\n"

			"THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES"
			"OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE"
			"FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH"
			"THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.");
			
	}
	ImGui::End();
}

void ModuleEditor::ApplicationWindow()
{
	if (ImGui::Begin("Application", &showApplicationWindow)) {
		float width = ImGui::GetWindowContentRegionWidth();
		char title[25];

		if (ImGui::Button("Unknown Engine              ")) { App->RequestBrowser("https://github.com/UnknownEngine/Engine/wiki"); }

		if (ImGui::Button("CITM                        ")) { App->RequestBrowser("https://www.citm.upc.edu/"); }
		ImVec2(100, 100);

		ImGui::SliderInt("FPS Cap", &slider_frames, 1, 60);

		sprintf_s(title, 25, "Framerate %.1f", App->frames_log[App->frames_log.size() - 1]);
		ImGui::PlotHistogram("##FPS Ratio:", &App->frames_log[0], App->frames_log.size(), 0, title, 0.0f, 100, ImVec2(310, 100));
		sprintf_s(title, 25, "Milliseconds %.1f", App->ms_log[App->ms_log.size() - 1]);
		ImGui::PlotHistogram("##Milliseconds:", &App->ms_log[0], App->ms_log.size(), 0, title, 0.0f, 100, ImVec2(310, 100));
	}
	ImGui::End();
}

void ModuleEditor::WindowWindow()
{
	if (ImGui::Begin("Window Settings", &showWindowWindow)) {
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
	ImGui::End();
}

void ModuleEditor::OpenGLSettings()
{
	if (ImGui::Begin("OpenGL Settings", &showOpenGLWindow)) {
		if (ImGui::Checkbox("Depth Test", &App->renderer3D->gl_depth_test)) { App->renderer3D->ActivateCheckBoxs(App->renderer3D->gl_depth_test, 0); }
		ImGui::SameLine(150);
		if (ImGui::Checkbox("Cull Face", &App->renderer3D->gl_cull_face)) { App->renderer3D->ActivateCheckBoxs(App->renderer3D->gl_cull_face, 1); }
		if (ImGui::Checkbox("Color Material", &App->renderer3D->gl_color_material)) { App->renderer3D->ActivateCheckBoxs(App->renderer3D->gl_color_material, 2); }
		ImGui::SameLine(150);
		if (ImGui::Checkbox("Lighting", &App->renderer3D->gl_lightning)) { App->renderer3D->ActivateCheckBoxs(App->renderer3D->gl_lightning, 3); }
		if (ImGui::Checkbox("Texture 2D", &App->renderer3D->gl_texture_2d)) { App->renderer3D->ActivateCheckBoxs(App->renderer3D->gl_texture_2d, 4); }
		ImGui::SameLine(150);
		if (ImGui::Checkbox("Ambient", &App->renderer3D->gl_ambient)) { App->renderer3D->ActivateCheckBoxs(App->renderer3D->gl_ambient, 5); }
		if (ImGui::Checkbox("Ambient & Diffuse", &App->renderer3D->gl_ambient_diffuse)) { App->renderer3D->ActivateCheckBoxs(App->renderer3D->gl_ambient_diffuse, 6); }
		ImGui::SameLine(150);
		if (ImGui::Checkbox("Render Wireframe", &App->renderer3D->gl_wireframe)) { App->renderer3D->ActivateCheckBoxs(App->renderer3D->gl_wireframe, 7); }
	}
	ImGui::End();
}

void ModuleEditor::HardwareWindow()
{
	if (ImGui::Begin("Hardware Specs", &showHardwareWindow)) {
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
	ImGui::End();
}

void ModuleEditor::InputWindow()
{
	if (ImGui::Begin("Input", &showInputWindow)) {
		ImGui::TextUnformatted(input_log.begin());
		if (scroll)
		{
			ImGui::SetScrollHere(1.0f);
		}
		scroll = false;
	}
	ImGui::End();
}

void ModuleEditor::ConsoleWindow()
{
	if (ImGui::Begin("Console", &showConsoleWindow)) {
		if (ImGui::Button("Clear Console")) {
			CleanConsole();
		}
		PrintConsole();
	}
	ImGui::End();
}

void ModuleEditor::CreateHierarchy(GameObject* gameobject)
{
	if (gameobject != NULL)
	{
		if (ImGui::TreeNode(gameobject->nameID.c_str()))
		{
			if (ImGui::IsItemClicked(0)) {
				App->scene_intro->selected = gameobject;
			}
			
			for (int i = 0; i < gameobject->childs.size(); i++)
			{
				if (i == 0)
				{
					ImGui::SetNextItemOpen(true, ImGuiCond_Once);
				}
				GameObject* gobject = gameobject->childs[i];
				CreateHierarchy(gobject);		
			}
		ImGui::TreePop();
		}
	}

}

void ModuleEditor::HyperLink(const char* tooltip, const char* url)
{
	if (ImGui::IsItemHovered()) {
		if (ImGui::IsItemClicked(0)) {
			App->RequestBrowser(url);
		}
		
		ImGui::SetTooltip(tooltip);
	}
}

bool ModuleEditor::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	return true;
}
