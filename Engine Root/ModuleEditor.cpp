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
#include <vector>
#include <algorithm>
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "MaterialComponent.h"
#include "ImGuizmo/ImGuizmo.h"
#include "CameraComponent.h"

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
	showLoadWindow = false;
	showAssets = false;
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

	App->fsystem->DiscoverFiles("", uselessFiles, rootFolders);
	App->fsystem->DiscoverFiles("Assets", uselessFiles, assetFolders);

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

	ImGuiIO& io = ImGui::GetIO();

	ImGuizmo::BeginFrame();
	if (App->scene_intro->selected != nullptr && App->scene_intro->selected != App->camera->cameraObject) {

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
			finalOperation = ImGuizmo::OPERATION::TRANSLATE;
		}
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
			finalOperation = ImGuizmo::OPERATION::ROTATE;
		}
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
			finalOperation = ImGuizmo::OPERATION::SCALE;
		}

		GameObject* gameObject = App->scene_intro->selected;
		float4x4 viewMatrix = App->camera->camera->frustum.ViewMatrix();
		viewMatrix.Transpose();
		float4x4 projectionMatrix = App->camera->camera->frustum.ProjectionMatrix().Transposed();
		float4x4 modelProjection = gameObject->GetTransformComponent()->global_transform.Transposed();

		//cornerPos = Vec2(img_corner.x, Engine->window->windowSize.y - img_corner.y - img_size.y);
		ImGuizmo::SetRect(ImGui::GetMainViewport()->Pos.x, ImGui::GetMainViewport()->Pos.y, ImGui::GetMainViewport()->Size.x, ImGui::GetMainViewport()->Size.y);

		float modelPtr[16];
		memcpy(modelPtr, modelProjection.ptr(), 16 * sizeof(float));
		ImGuizmo::MODE finalMode = ImGuizmo::MODE::WORLD;
		ImGuizmo::Manipulate(viewMatrix.ptr(), projectionMatrix.ptr(), finalOperation, finalMode, modelPtr);

		if (ImGuizmo::IsUsing())
		{
			float4x4 newMatrix;
			newMatrix.Set(modelPtr);
			modelProjection = newMatrix.Transposed();

			float4x4 localTransform = gameObject->parent->GetTransformComponent()->transform.Inverted() * modelProjection;
			gameObject->GetTransformComponent()->transform = localTransform;
			gameObject->GetTransformComponent()->global_transform = modelProjection;

			localTransform.Decompose(gameObject->GetTransformComponent()->position, gameObject->GetTransformComponent()->rotation, gameObject->GetTransformComponent()->scale);
		}
	}


	//if (App->scene_intro->selected != nullptr) {
	//	float4x4 viewMatrix = App->camera->camera->frustum.ViewMatrix();
	//	viewMatrix.Transpose();
	//	float modelPtr[16];
	//	float4x4 modelProjection = App->scene_intro->selected->GetTransformComponent()->global_transform.Transposed();
	//	memcpy(modelPtr, modelProjection.ptr(), 16 * sizeof(float));
	//	float4x4 projectionMatrix = App->camera->camera->frustum.ProjectionMatrix().Transposed();

	//}


	CreateFileInspector();

	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {

			if (ImGui::MenuItem("Save Scene"))
			{
				char* buffer;
				App->fsystem->SaveScene(&buffer);
			}

			if (ImGui::MenuItem("Load Scene",nullptr,nullptr,&showLoadWindow))
			{
				showLoadWindow = !showLoadWindow;
			}
			

			if (ImGui::MenuItem("Exit", NULL, false, true)) {
				return UPDATE_STOP;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help")) {

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
				showHardwareWindow = !showHardwareWindow;
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
		if (ImGui::BeginMenu("Primitives"))
		{
			if (ImGui::MenuItem("Sphere")){
				App->fsystem->CreatePrimitives("Assets/FBXs/Sphere.fbx", "Sphere");
			}
			if (ImGui::MenuItem("Cylinder")) {
				App->fsystem->CreatePrimitives("Assets/FBXs/Cylinder.fbx", "Cylinder");
			}
			if (ImGui::MenuItem("Cube")) {
				App->fsystem->CreatePrimitives("Assets/FBXs/Cube.fbx", "Cube");
			}
			if (ImGui::MenuItem("Cone")) {
				App->fsystem->CreatePrimitives("Assets/FBXs/Cube.fbx", "Cone");
			}
			ImGui::EndMenu();
		}
		

	ImGui::EndMainMenuBar();
	}
	ImGui::Begin("Hierarchy");

	if (ImGui::Button("Create Empty", ImVec2(200, 20)))
	{
		gobIndex ++;
		std::string gobName = "Empty Gobject ";
		gobName += std::to_string(gobIndex);
		GameObject* emptyGob = new GameObject(gobName.c_str());
		emptyGob->UID = LCG().Int();

		float3 pos(0, 0, 0);
		float3 scale(1, 1, 1);
		Quat rot(0, 0, 0, 1);

		TransformComponent* transform = new TransformComponent(pos, rot, scale);
		transform->UID = LCG().Int();
		emptyGob->AddComponent(transform);

		App->scene_intro->gameObjectsList.push_back(emptyGob);
	}

	for (uint i = 0; i < App->scene_intro->gameObjectsList.size(); i++)
	{
		CreateHierarchy(App->scene_intro->gameObjectsList[i]);
	}
	ImGui::End();

	if (ImGui::Begin("Inspector"))
	{
		if (App->scene_intro->selected != NULL)
		{
			FillInspector(App->scene_intro->selected);
		}
		else
		{
			hasTransform = false;
			hasMesh = false;
			hasMaterial = false;
		}
		DrawInspector();
		
		ImGui::End();
	}

	if (ImGui::Begin("Time Manager"))
	{
		if (ImGui::Button("Play", ImVec2(100, 20)))
		{
			if (App->editorMode)
			{
				char* buffer;
				App->fsystem->SaveScene(&buffer);
				App->editorMode = !App->editorMode;
				App->gameMode = !App->gameMode;
			}
		}
		ImGui::SameLine(150.f);
		if (ImGui::Button("Pause", ImVec2(100, 20)))
		{
			if (App->gameMode)
			{
				App->gameModePaused = !App->gameModePaused;
			}
		}
		ImGui::SameLine(300.f);
		if (ImGui::Button("Stop", ImVec2(100, 20)))
		{
			if (App->gameMode)
			{
				App->scene_intro->selected = NULL;
				App->gameModePaused = false;
				App->editorMode = !App->editorMode;
				App->gameMode = !App->gameMode;
				App->fsystem->LoadScene(App->scene_intro->sceneBuffer);
			}
		}
	}
	ImGui::End();
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
	if (showLoadWindow)
		LoadWindow();
	if (showAboutWindow)
		AboutWindow();

	if (showcase)
		ImGui::ShowDemoWindow();


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
	if (App->scene_intro->selected != NULL)
	{
		if (ImGui::Button("Delete Game Object", ImVec2(200, 20)))
		{
			for (int i = 0; i < App->scene_intro->gameObjectsList.size(); i++)
			{
				if (App->scene_intro->selected->parent != NULL)
				{
					for (int j = 0; j < App->scene_intro->gameObjectsList.at(i)->childs.size(); j++)
					{
						if (App->scene_intro->selected->UID == App->scene_intro->gameObjectsList.at(i)->childs.at(j)->UID)
						{
							delete App->scene_intro->gameObjectsList.at(i)->childs.at(j);
							auto it = std::find(App->scene_intro->gameObjectsList.at(i)->childs.begin(), App->scene_intro->gameObjectsList.at(i)->childs.end(), App->scene_intro->gameObjectsList.at(i)->childs.at(j));
							App->scene_intro->gameObjectsList.at(i)->childs.erase(it);
							App->scene_intro->selected = nullptr;
						}
					}
				}
				else
				{
					if (App->scene_intro->selected->UID == App->scene_intro->gameObjectsList.at(i)->UID)
					{
						delete App->scene_intro->gameObjectsList.at(i);
						auto it = std::find(App->scene_intro->gameObjectsList.begin(), App->scene_intro->gameObjectsList.end(), App->scene_intro->gameObjectsList.at(i));
						App->scene_intro->gameObjectsList.erase(it);
						App->scene_intro->selected = nullptr;
					}
				}
			}
		}
	}
	if (App->scene_intro->selected != NULL)
	{
		//if (App->scene_intro->selected == App->camera->cameraObject) {
		//	DrawCameraComponents();
		//	return;
		//}
		if (ImGui::Button("Create Empty Child", ImVec2(200, 20)))
		{
			childIndex++;
			std::string childName = "Empty Child ";
			childName += std::to_string(childIndex);
			GameObject* emptychild = new GameObject(childName.c_str());
			emptychild->UID = LCG().Int();

			float3 pos(App->scene_intro->selected->GetTransformComponent()->position.x, App->scene_intro->selected->GetTransformComponent()->position.y, App->scene_intro->selected->GetTransformComponent()->position.z);
			float3 scale(App->scene_intro->selected->GetTransformComponent()->scale.x, App->scene_intro->selected->GetTransformComponent()->scale.y, App->scene_intro->selected->GetTransformComponent()->scale.z);
			Quat rot(App->scene_intro->selected->GetTransformComponent()->rotation.x, App->scene_intro->selected->GetTransformComponent()->rotation.y, App->scene_intro->selected->GetTransformComponent()->rotation.z, App->scene_intro->selected->GetTransformComponent()->rotation.w);

			TransformComponent* transform = new TransformComponent(pos,rot,scale);
			transform->UID = LCG().Int();
			emptychild->AddComponent(transform);
			App->scene_intro->selected->childs.push_back(emptychild);
		}
		if (hasTransform)
		{
			ImGui::CollapsingHeader("Transform");
			ImGui::PushItemWidth(60);
			ImGui::Text("");

			ImGui::Text("Position:");
			ImGui::PushItemWidth(150);
			ImGui::Text("X:");
			ImGui::SameLine(72.f);
			ImGui::InputFloat("##posx", &App->scene_intro->selected->GetTransformComponent()->position.x, 0.5, 1);
			ImGui::Text("Y:");
			ImGui::SameLine(72.f);
			ImGui::InputFloat("##posy", &App->scene_intro->selected->GetTransformComponent()->position.y, 0.5, 1);
			ImGui::Text("Z:");
			ImGui::SameLine(72.f);
			ImGui::InputFloat("##posz", &App->scene_intro->selected->GetTransformComponent()->position.z, 0.5, 1);
			ImGui::PopItemWidth();


			ImGui::Text("");
			ImGui::Separator();
			ImGui::Text("");

			ImGui::Text("Rotation:");
			ImGui::Text("X:");
			ImGui::SameLine(22.f);
			ImGui::Text("%f", App->scene_intro->selected->GetTransformComponent()->rotation.x);
			ImGui::SameLine(92.f);
			ImGui::PushItemWidth(1);
			if (ImGui::InputFloat("##rotx", &rotx, 1, 1))
			{
				rotx = rotx * pi / 180;
				Quat q2(float3(1, 0, 0), rotx);

				App->scene_intro->selected->GetTransformComponent()->rotation = App->scene_intro->selected->GetTransformComponent()->rotation * q2;
				App->scene_intro->selected->GetTransformComponent()->rotation.Normalize();
			}
			ImGui::PopItemWidth();

			ImGui::Text("Y:");
			ImGui::SameLine(22.f);
			ImGui::Text("%f", App->scene_intro->selected->GetTransformComponent()->rotation.y);
			ImGui::SameLine(92.f);
			ImGui::PushItemWidth(1);
			if (ImGui::InputFloat("##roty", &roty, 1, 1))
			{
				roty = roty * pi / 180;
				Quat q2(float3(0, 1, 0), roty);

				App->scene_intro->selected->GetTransformComponent()->rotation = App->scene_intro->selected->GetTransformComponent()->rotation * q2;
				App->scene_intro->selected->GetTransformComponent()->rotation.Normalize();
			}
			ImGui::PopItemWidth();
			ImGui::Text("Z:");
			ImGui::SameLine(22.f);
			ImGui::Text("%f", App->scene_intro->selected->GetTransformComponent()->rotation.z);
			ImGui::SameLine(92.f);
			ImGui::PushItemWidth(1);
			if (ImGui::InputFloat("##rotz", &rotz, 1, 1))
			{
				rotz = rotz * pi / 180;
				Quat q2(float3(0, 0, 1), rotz);

				App->scene_intro->selected->GetTransformComponent()->rotation = App->scene_intro->selected->GetTransformComponent()->rotation * q2;
				App->scene_intro->selected->GetTransformComponent()->rotation.Normalize();
			}
			ImGui::PopItemWidth();

			ImGui::Text("");
			ImGui::Separator();
			ImGui::Text("");
			if (App->scene_intro->selected != App->camera->cameraObject) {
				ImGui::Text("Scale:");
				ImGui::PushItemWidth(150);
				ImGui::InputFloat("##scalex", &App->scene_intro->selected->GetTransformComponent()->scale.x, 0.1, 0.3);
				ImGui::InputFloat("##scaley", &App->scene_intro->selected->GetTransformComponent()->scale.y, 0.1, 0.3);
				ImGui::InputFloat("##scalez", &App->scene_intro->selected->GetTransformComponent()->scale.z, 0.1, 0.3);
				ImGui::PopItemWidth();
			ImGui::PopItemWidth();

			ImGui::Text("");
			ImGui::Separator();
			ImGui::Text("");
			}

			ImGui::Text("");
			ImGui::SameLine(72.f);
			if (ImGui::Button("Reset Transforms", ImVec2(200, 20)))
			{
				App->scene_intro->selected->GetTransformComponent()->position = float3(0, 0, 0);
				App->scene_intro->selected->GetTransformComponent()->scale = float3(1, 1, 1);
				App->scene_intro->selected->GetTransformComponent()->rotation = Quat(0, 0, 0, 1);
			}
			ImGui::Text("");
		}

		if (hasMesh)
		{
			ImGui::CollapsingHeader("Mesh");
			ImGui::Checkbox("Active", &App->scene_intro->selected->GetMeshComponent()->active);
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
			if (ImGui::Button("Show normals vertices", ImVec2(200, 20))) {
				App->scene_intro->selected->GetMeshComponent()->r_mesh->showNormalsVertices = !App->scene_intro->selected->GetMeshComponent()->r_mesh->showNormalsVertices;
			}
			ImGui::Text("");

			ImGui::TextWrapped("Vertexs");
			ImGui::SameLine(100);
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(233, 233, 43)));
			ImGui::TextWrapped("%i", numvertices);
			ImGui::PopStyleColor(1);

			ImGui::TextWrapped("Normals");
			ImGui::SameLine(100);
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(233, 233, 43)));
			ImGui::TextWrapped("%i", numnormals);
			ImGui::PopStyleColor(1);
			ImGui::Text("");

			ImGui::Text("");
			ImGui::SameLine(72.f);
			if (ImGui::Button("Delete Mesh", ImVec2(200, 20)))
			{
				for (int i = 0; i < App->scene_intro->selected->components.size(); i++)
				{
					if (App->scene_intro->selected->components.at(i)->type == ComponentType::Mesh)
					{
						delete App->scene_intro->selected->components.at(i);
						auto it = std::find(App->scene_intro->selected->components.begin(), App->scene_intro->selected->components.end(), App->scene_intro->selected->components.at(i));
						App->scene_intro->selected->components.erase(it);

					}
				}
			}
			ImGui::Text("");
		}
		if (hasMaterial)
		{
			ImGui::CollapsingHeader("Material");

			ImGui::Checkbox("Active##material", &App->scene_intro->selected->GetMaterialComponent()->active);

			ImGui::TextWrapped("File Path:");
			ImGui::SameLine(100);
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(233, 233, 43)));
			ImGui::TextWrapped(material_path.c_str());
			ImGui::PopStyleColor(1);

			ImGui::Button("Change Source", ImVec2(200, 20));

			ImGui::PushItemWidth(95);
			ImGui::Text("");

			if (ImGui::Button("Use Checkers", ImVec2(200, 20))) {
				App->scene_intro->selected->GetMaterialComponent()->r_texture->useChecker = !App->scene_intro->selected->GetMaterialComponent()->r_texture->useChecker;
			}

			ImGui::Text("");
			ImGui::Text("");
			ImGui::SameLine(72.f);
			if (ImGui::Button("Delete Material", ImVec2(200, 20)))
			{
				for (int i = 0; i < App->scene_intro->selected->components.size(); i++)
				{
					if (App->scene_intro->selected->components.at(i)->type == ComponentType::Material)
					{
						delete App->scene_intro->selected->components.at(i);
						auto it = std::find(App->scene_intro->selected->components.begin(), App->scene_intro->selected->components.end(), App->scene_intro->selected->components.at(i));
						App->scene_intro->selected->components.erase(it);
					}
				}
			}

			ImGui::Text("");
			ImGui::Separator();
			ImGui::Text("");

			ImGui::Text("Size: (%i,%i)", material_width, material_height);
			ImGui::Text("Bpp: %i", material_bpp);
			if (App->scene_intro->selected != NULL && App->scene_intro->selected->GetMaterialComponent() != NULL)
			{
				if (!App->scene_intro->selected->GetMaterialComponent()->r_texture->useChecker)
				{
					ImGui::Image((void*)(intptr_t)material_buffer, ImVec2(256, 256));
				}
				else {
					ImGui::Image((void*)(intptr_t)App->geometry->bufferCheckerTexture, ImVec2(256, 256));
				}
			}
		}
	}
}

void ModuleEditor::DrawCameraComponents()
{
	ImGui::CollapsingHeader("Transform");
	ImGui::PushItemWidth(60);
	ImGui::Text("");

	ImGui::Text("Position:");
	ImGui::PushItemWidth(150);
	ImGui::Text("X:");
	ImGui::SameLine(72.f);
	ImGui::InputFloat("##posx", &App->scene_intro->selected->GetTransformComponent()->position.x, 0.5f, 0.1f);
	ImGui::Text("Y:");
	ImGui::SameLine(72.f);
	ImGui::InputFloat("##posy", &App->scene_intro->selected->GetTransformComponent()->position.y, 0.5, 1);
	ImGui::Text("Z:");
	ImGui::SameLine(72.f);
	ImGui::InputFloat("##posz", &App->scene_intro->selected->GetTransformComponent()->position.z, 0.5, 1);
	ImGui::PopItemWidth();
}

void ModuleEditor::FillInspector(GameObject* selected)
{
		TransformComponent* transformComponent = selected->GetTransformComponent();
		MaterialComponent* materialComponent = selected->GetMaterialComponent();
		MeshComponent* meshComponent = selected->GetMeshComponent();
	
	if (transformComponent != NULL)
	{
		//float tfposx = transformComponent->position.x;
		//float tfposy = transformComponent->position.y;
		//float tfposz = transformComponent->position.z;
		//tposx = transformComponent->position.x;
		//tposy = transformComponent->position.y;
		//tposz = transformComponent->position.z;
		//sprintf(tposy, "%f", tfposy);
		//sprintf(tposz, "%f", tfposz);


		float tfrotx = transformComponent->rotation.x;
		float tfroty = transformComponent->rotation.y;
		float tfrotz = transformComponent->rotation.z;
		//rotx = transformComponent->rotation.x;
		//roty = transformComponent->rotation.y;
		//rotz = transformComponent->rotation.z;

		float tfscalex = transformComponent->scale.x;
		float tfscaley = transformComponent->scale.y;
		float tfscalez = transformComponent->scale.z;
		//sprintf(tscalex, "%f", tfscalex);
		//sprintf(tscaley, "%f", tfscaley);
		//sprintf(tscalez, "%f", tfscalez);
		hasTransform = true;
	}
	else
	{
		hasTransform = false;
	}
	if (materialComponent != nullptr)
	{
		//material_name = materialComponent->name;
		//material_path = materialComponent->path;
		//material_width = materialComponent->width;
		//material_height = materialComponent->height;
		//material_bpp = materialComponent->bpp;
		//material_buffer = materialComponent->bufferTexture;
		//hasMaterial = true;
	}
	else
	{
		hasMaterial = false;
	}
	if (meshComponent != nullptr)
	{
		numvertices = meshComponent->r_mesh->num_vertices;
		numnormals = meshComponent->r_mesh->num_normals;
		meshname = meshComponent->r_mesh->name;
		meshpath = meshComponent->r_mesh->path;
		hasMesh = true;
	}
	else
	{
		hasMesh = false;
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
		HyperLink("Unkown Engine Github", "https://github.com/UnknownEngine/Engine");
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
		if (ImGui::Checkbox("Lighting", &App->renderer3D->gl_lightning)) { App->renderer3D->ActivateCheckBoxs(!App->renderer3D->gl_lightning, 3); }
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

void ModuleEditor::LoadWindow()
{
	if (ImGui::Begin("Are you sure?", &showLoadWindow)) {

		if (ImGui::Button("Yeah, sure", ImVec2(100, 20)))
		{
			App->fsystem->LoadScene(App->scene_intro->sceneBuffer);
			showLoadWindow = !showLoadWindow;
		}
		ImGui::SameLine(150.f);
		if (ImGui::Button("Wait, no", ImVec2(100, 20)))
		{
			showLoadWindow = !showLoadWindow;
		}
	}
	ImGui::End();
}

void ModuleEditor::CreateHierarchy(GameObject* gameobject)
{
	if (gameobject != NULL)
	{

		if (ImGui::TreeNodeEx(gameobject->nameID.c_str(),ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_None))
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

void ModuleEditor::CreateFileInspector()
{
	//ImGui::ShowDemoWindow();
	std::string assets = "Assets";
	ImGui::Begin("Files");

	if (ImGui::TreeNode(rootFolders[0].c_str())) {

		for (uint i = 0; i < assetFolders.size(); i++)
		{
			if (ImGui::TreeNode(assetFolders[i].c_str()))
			{
				// FBX //
				if (i == 0) {
					ShowFbxList(i);
				}
				// TEXTURES //
				if (i == 1) {
					ShowTexturesList(i);
				}
				ImGui::TreePop();

			}
		}

		ImGui::TreePop();
	}
	

	//for (uint i = 0; i < rootFolders.size(); i++)
	//{

	//	bool open1 = ImGui::TreeNode(((void*)(intptr_t)i,rootFolders[i].c_str()));
	//	if (ImGui::IsItemClicked(0)) {
	//		LOG("%s, %d",rootFolders[i].c_str(),i);
	//		node_clicked = i;
	//	}
	//	if (open1 && node_clicked == 1) 
	//	{
	//		
	//		if (node_clicked == 1) {
	//			for (uint j = 0; j < assetFolders.size(); j++)
	//			{
	//				bool open2 = ImGui::TreeNode(((void*)(intptr_t)j, assetFolders[j].c_str()));
	//				if (open2) {

	//					ImGui::TreePop();
	//				}
	//			}
	//		}

	//		ImGui::TreePop();
	//	}
	//			
	//}
	//for (int i = 0; i < rootFolders.size(); i++)
	//{
	//	if (ImGui::TreeNode(rootFolders.at(i).c_str()))
	//	{
	//		if (ImGui::IsItemClicked(0)) {
	//			showAssets = !showAssets;
	//		}
	//		if (showAssets)
	//		{
	//			if (assetFolders.size() <= 0) return;
	//			for (int j=0;j<assetFolders.size(); j++)
	//			{
	//				if (ImGui::TreeNode(assetFolders.at(j).c_str()))
	//				{
	//					if (j == 0)
	//					{
	//						ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	//					}
	//					ImGui::TreePop();
	//				}
	//			}
	//		}
	//		if (i == 0)
	//		{
	//			ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	//		}
	//		ImGui::TreePop();
	//	}
	//	
	//	
	//}
	ImGui::End();
}
void ModuleEditor::ShowFbxList(const uint& i)
{
	for (uint j = 0; j < App->resourceManager->fbxList.size(); j++)
	{
		std::string name = App->resourceManager->fbxList.at(j);
		std::string finalName = name.erase(name.size() - 4);
		if (ImGui::TreeNodeEx(finalName.c_str(), ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Bullet))
		{
			if (ImGui::IsItemClicked(0)) {
				std::string resourceDir = rootFolders[0] + std::string("/") + assetFolders[i] + std::string("/") + finalName + std::string(".mta");
				/*App->scene_intro->selected->GetMeshComponent()->r_mesh =*/ static_cast<ResourceMesh*>(App->resourceManager->RequestResource(resourceDir.c_str(), ResourceType::mesh));
			}
			ImGui::TreePop();
		}
	}
}
void ModuleEditor::ShowTexturesList(const uint& i)
{
	for (uint j = 0; j < App->resourceManager->textureList.size(); j++)
	{
		std::string name = App->resourceManager->textureList.at(j);
		std::string finalName = name.erase(name.size() - 4);
		if (ImGui::TreeNodeEx(finalName.c_str(), ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Bullet))
		{
			if (ImGui::IsItemClicked(0)) {
				std::string resourceDir = rootFolders[0] + std::string("/") + assetFolders[i] + std::string("/") + finalName + std::string(".mta");
				if (App->scene_intro->selected != nullptr) {
					ResourceTexture* r_texture = App->scene_intro->selected->GetMaterialComponent()->r_texture;
					if (r_texture != nullptr) {
						r_texture->instances--;
						if (r_texture->instances <= 0) {
							App->resourceManager->resourceMap.erase(r_texture->UID);
						}
					}
					App->scene_intro->selected->GetMaterialComponent()->r_texture = static_cast<ResourceTexture*>(App->resourceManager->RequestResource(resourceDir.c_str(),ResourceType::texture));
				}

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
