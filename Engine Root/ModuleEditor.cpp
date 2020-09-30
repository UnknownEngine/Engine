#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"



ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Start()
{
	return false;
}

update_status ModuleEditor::Update(float dt)
{
	return update_status();
}


bool ModuleEditor::CleanUp()
{
	return false;
}
