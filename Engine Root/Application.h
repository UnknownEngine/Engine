#pragma once

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleResources.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "ModuleSceneIntro.h"
#include "ModuleEditor.h"
#include "ModuleGeometry.h"
#include "Primitive.h"
#include "ModuleFSystem.h"

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleSceneIntro* scene_intro;
	ModulePhysics3D* physics;
	ModuleEditor* editor;
	ModuleGeometry* geometry;
	M_FileSystem* fsystem;
	ModuleResourceManager* resourceManager;


	Timer	startup_time;
	Timer	frame_time;
	Timer	last_sec_frame_time;

	int		frame_count = 0;
	int		miliseconds_count = 0;
	int		frames_iterator = 0;
	int		ms_iterator = 0;

	float	last_sec_frame_count = 0;
	float	last_frame_ms = 0;
	float	frames_on_last_update = 0;
	float	prev_last_sec_frame_count = 0;
	float	avg_fps = 0;
	float	seconds_since_startup = 0;
	float	maxcapTime;

	bool editorMode;
	bool gameMode;
	bool gameModePaused;
	bool onStart;

	std::vector<float> frames_log;
	std::vector<float> ms_log;
	std::vector<float> primitive_list;

private:

	float	dt;
	std::vector<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void RequestBrowser(const char* path);

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
	void FillFrameLog();
	void FillMSLog();
	void SetMaxFramesCap(int frames_to_cap);
};

