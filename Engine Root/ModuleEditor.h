#pragma once

#include "Module.h"
#include "Globals.h"
#include "ImGui/imgui.h"
#include <vector>
#include "ImGuizmo/ImGuizmo.h"
class WindowFrame;
struct ImGuiWindowClass;


class ModuleEditor : public Module
{

public:
	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Init();
	bool Start();

	void AddLog(char* path);



	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

private:
	void FillInspector(GameObject* selected);
	void DrawInspector();
	void PrintConsole();
	void CleanConsole();
	void AboutWindow();
	void ApplicationWindow();
	void WindowWindow();
	void OpenGLSettings();
	void HardwareWindow();
	void InputWindow();
	void ConsoleWindow();
	void LoadWindow();

	void CreateHierarchy(GameObject* gameobject);

	void CreateFileInspector();

	void ShowTexturesList(const uint& i);

	void ShowFbxList(const uint& i);

	void HyperLink(const char* tooltip, const char* url);

public:

	int slider_frames = 60;

	bool IsActive = true;
	ImGuiTextBuffer input_log;
	bool scroll = false;

	ImGuizmo::OPERATION finalOperation = ImGuizmo::OPERATION::TRANSLATE;
private:
	std::vector<WindowFrame*> windowFrames;

	ImGuiWindowClass* frameWindowClass = nullptr;
	ImGuiWindowClass* normalWindowClass = nullptr;

	bool using_keyboard;
	bool using_mouse;
	bool showcase;
	bool showAboutWindow;
	bool showApplicationWindow;
	bool showWindowWindow;
	bool showOpenGLWindow;
	bool showInputWindow;
	bool showHardwareWindow;
	bool showConsoleWindow;
	bool showLoadWindow;

	bool showAssets;

	float rotx = 0.0;
	float roty = 0.0;
	float rotz = 0.0;
	float rotxInsp = 0.0;
	float rotyInsp = 0.0;
	float rotzInsp = 0.0;

	int childIndex = 0;
	int gobIndex = 0;

	std::string meshname = "";
	std::string meshpath = "";
	std::string material_name = "";
	std::string material_path = "";
	int numvertices = 0;
	int numnormals = 0;

	int material_width = 0;
	int material_height = 0;
	int material_bpp = 0;
	uint material_buffer = 0;

	bool hasTransform = false;
	bool hasMaterial = false;
	bool hasMesh = false;

	std::vector<std::string> uselessFiles;
	std::vector<std::string> rootFolders;
	std::vector<std::string> libFolders;
	std::vector<std::string> assetFolders;
	std::vector<std::string> libMeshFolders;
	std::vector<std::string> libTextureFolders;
	std::vector<std::string> assetFBXsFolders;
	std::vector<std::string> assetTextureFolders;


	std::vector<char*> items;

	int node_clicked = -1;
};