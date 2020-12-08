#pragma once
#include "Module.h"
#include "Globals.h"
#include "CameraComponent.h"
#include "MathGeoLib/include/MathGeoLib.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	void UpdateCameraPos(math::float3& newPos);
	void Zoom(math::float3& newPos, float zoom_speed, float dt);
	void PanCamera(math::float3& newPos, float speed, float dragSpeed, float dt);
	void RotateCamera(float sensitivity);
	void OrbitalCamera(float sensitivity, float dt, float orbital_speed);
	void KeyboardMove(math::float3& newPos, float speed);
	void ModifySpeed(float& speed);
	void FocusCamera();
	bool CleanUp();

	void Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference = false);
	void LookAt(const float3 &Spot);
	float3x4 GetViewMatrix();

	void OnMouseClick();
	float2 ScreenToWorld(float2 point);

public:

	float3 Position, Reference;
	CameraComponent* camera;

private:

	double last_x = 0;
	double last_y = 0;

	float x_rot = 0;
	float y_rot = 0;

	GameObject* cameraObject;

};






































//
//class Arcball
//{
//private:
//	float fov;
//	int fovStartY;
//	int fovCurrentY;
//
//	float transX, transY;
//	float currentTransX, currentTransY;
//	float startTransX, startTransY;
//
//	mat4x4 startMatrix;
//	mat4x4 currentMatrix;
//	vec3 startRotationVector;
//	vec3 currentRotationVector;
//	bool isRotating;
//	float ballRadius;
//	double residualSpin;
//	static const float INITIAL_FOV;
//	static const float MINIMAL_FOV;
//	static const float TRANSLATION_FACTOR;
//
//	float OneDividedSixTimesTen = 0.000001;
//
//
//	vec3 convertXY(int x, int y);
//	int width, height;
//public:
//	Arcball();
//
//	void setWidthHeight(int w, int h);
//	void startRotation(int x, int y);
//	void updateRotation(int x, int y);
//	void stopRotation();
//
//
//	void applyTranslationMatrix(bool reverse = false);
//	void applyRotationMatrix();
//
//	void setRadius(float newRadius);
//	void reset();
//
//	float norm(vec3 vector);
//};
