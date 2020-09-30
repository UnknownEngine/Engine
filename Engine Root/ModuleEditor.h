#pragma once

#include "Module.h"
#include "Globals.h"
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
	
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

private:
	

public:

private:
	std::vector<WindowFrame*> windowFrames;

	ImGuiWindowClass* frameWindowClass = nullptr;
	ImGuiWindowClass* normalWindowClass = nullptr;

	bool using_keyboard;
	bool using_mouse;

};