#include "Application.h"
#include "ImGui/imgui.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "ModuleEditor.h"


Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	physics = new ModulePhysics3D(this);
	editor = new ModuleEditor(this);
	geometry = new ModuleGeometry(this);
	fsystem = new M_FileSystem(this);
	resourceManager = new ModuleResourceManager(this);


	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(fsystem);
	AddModule(physics);
	AddModule(editor);
	AddModule(geometry);
	AddModule(resourceManager);

	//scenes
	AddModule(scene_intro);
	// Renderer last!
	AddModule(renderer3D);

	onStart = true;
	editorMode = true;
	gameMode = false;
	gameModePaused = false;
}

Application::~Application()
{
	for (uint i = 0; i < list_modules.size(); i++)
	{
		RELEASE(list_modules[i]);
	}

}

bool Application::Init()
{
	bool ret = true;
	frames_log.resize(100);
	ms_log.resize(100);
	// Call Init() in all modules
	LOG("-------------- Application Init --------------");
	for (uint i = 0; i < list_modules.size(); i++)
	{
		if (list_modules[i]->IsActive())
			ret = list_modules[i]->Init();
	}

	// After all Init calls we call Start() in all modules
	LOG("-------------- Application Start --------------");
	for (uint i = 0; i < list_modules.size(); i++)
	{
		if (list_modules[i]->IsActive())
			ret = list_modules[i]->Start();
	}

	startup_time.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	frame_count++;
	last_sec_frame_count++;
	dt = (float)startup_time.Read() / 1000.0f;

	frame_time.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	//Do Framerate n MS Calculations
	if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	avg_fps = float(frame_count) / startup_time.Read();
	last_frame_ms = frame_time.Read();
	frames_on_last_update = prev_last_sec_frame_count;

	frames_on_last_update = editor->slider_frames;
	SetMaxFramesCap(editor->slider_frames);

	FillFrameLog();
	FillMSLog();
}



// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	for (uint i = 0; i < list_modules.size() && ret == UPDATE_CONTINUE; i++)
	{
		ret = list_modules[i]->PreUpdate(dt);
	}	

	for (uint i = 0; i < list_modules.size() && ret == UPDATE_CONTINUE; i++)
	{
		ret = list_modules[i]->Update(dt);
	}	

	for (uint i = 0; i < list_modules.size() && ret == UPDATE_CONTINUE; i++)
	{
		ret = list_modules[i]->PostUpdate(dt);
	}


	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for (uint i = 0; i < list_modules.size(); i++)
	{
		ret = list_modules[i]->CleanUp();
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

void Application::RequestBrowser(const char* path)
{
	ShellExecuteA(0, "Open", path, 0, "", 3);
}

void Application::FillFrameLog()
{
	if (frames_iterator >= frames_log.size() - 1)
	{
		for (int i = frames_log.size() - 1; i >= 0; i--)
		{

			if (i == 0)
			{
				frames_log.at(i) = frames_on_last_update;
			}
			else
			{
				frames_log.at(i) = frames_log.at(i - 1);
			}
		}
	}
	else
	{
		frames_log.at(frames_iterator) = frames_on_last_update;
		frames_iterator++;
	}
}

void Application::FillMSLog()
{
	if (ms_iterator >= ms_log.size() - 1)
	{
		for (int i = ms_log.size() - 1; i >= 0; i--)
		{

			if (i == 0)
			{
				ms_log.at(i) = last_frame_ms;
			}
			else
			{
				ms_log.at(i) = ms_log.at(i - 1);
			}
		}
	}
	else
	{
		ms_log.at(frames_iterator) = last_frame_ms;
		ms_iterator++;
	}
}

void Application::SetMaxFramesCap(int frames_to_cap)
{
	maxcapTime = 1000 / frames_to_cap;
	int delay = MAX(0, (int)maxcapTime - (int)last_frame_ms);
	SDL_Delay(delay);
}