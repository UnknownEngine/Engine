#include "Application.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	physics = new ModulePhysics3D(this);
	editor = new ModuleEditor(this);


	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(physics);
	AddModule(editor);
	
	//scenes
	AddModule(scene_intro);
	// Renderer last!
	AddModule(renderer3D);
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

	
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
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

void Application::RequestBrowser(char* path)
{
	ShellExecuteA(0, "Open", path, 0, "", 3);
}