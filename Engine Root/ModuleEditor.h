#pragma once

#include "Module.h"
#include "Globals.h"
#include "ImGui/imgui.h"
#include <vector>

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
	void PrintConsole();
	void CleanConsole();
	void AboutWindow();

public:

	int test = 60;
	ImGuiTextBuffer input_log;
	bool scroll = false;

private:
	std::vector<WindowFrame*> windowFrames;

	ImGuiWindowClass* frameWindowClass = nullptr;
	ImGuiWindowClass* normalWindowClass = nullptr;

	bool using_keyboard;
	bool using_mouse;
	bool showcase;
	bool showAboutWindow;

	std::vector<char*> items;
};