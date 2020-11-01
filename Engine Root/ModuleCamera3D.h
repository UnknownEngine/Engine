#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

class Arcball;
class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();

private:

	void CalculateViewMatrix();

public:
	
	vec3 X, Y, Z, Position, Reference;
	Arcball* ball=nullptr;

private:

	mat4x4 world;
	mat4x4 view;
	mat4x4 ViewMatrix, ViewMatrixInverse;
	double last_x = 0;
	double last_y = 0;

	float x_rot = 0;
	float y_rot = 0;

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
