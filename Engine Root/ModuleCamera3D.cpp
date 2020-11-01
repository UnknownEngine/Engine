#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"

#include "Glew/include/glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "TransformComponent.h"
#include <math.h>

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
	CalculateViewMatrix();

	//ball = new Arcball(App->window->width, App->window->height, 5.0f);
	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
		vec3 target(0, 0, 0);
		vec3 newPos(0, 0, 0);
		float speed = 3.0f * dt;
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed = 0.01f;

		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) newPos.y += speed;
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos.y -= speed;


		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z ;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z ;


		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X;

		Position += newPos/6;
		Reference += newPos/6;

		if (App->scene_intro->selected != NULL)
		{
			TransformComponent* transformComponent = App->scene_intro->selected->GetTransformComponent();
			target.x=transformComponent->position.x;
			target.y=transformComponent->position.y;
			target.z=transformComponent->position.z;
			if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
			{
				Position = target - (normalize(target - Position) * 15);
				LookAt(target);
			}
		}

		// Mouse motion ----------------


		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{

			if (App->input->GetKey(SDL_SCANCODE_LALT)==KEY_REPEAT)
			{
				LookAt(target);
			}
			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();
			float Sensitivity = 0.25f;

			Position -= Reference;

			if (dx != 0)
			{
				float DeltaX = (float)dx * Sensitivity;

				X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			}

			if (dy != 0)
			{
				float DeltaY = (float)dy * Sensitivity;

				Y = rotate(Y, DeltaY, X);
				Z = rotate(Z, DeltaY, X);
			}

			Position = Reference + Z * length(Position);
		}

	

	CalculateViewMatrix();
	
	
	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;
	
	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	
	ViewMatrixInverse = inverse(ViewMatrix);
}



























































//Arcball::Arcball()
//{
//	this->ballRadius = 600;
//	isRotating = false;
//	width = height = 0;
//	reset();
//}
//
//void Arcball::setWidthHeight(int w, int h)
//{
//	width = w;
//	height = h;
//	ballRadius = min((int)(w / 2), (int)(h / 2));
//}
//
//void Arcball::setRadius(float newRadius)
//{
//	ballRadius = newRadius;
//}
//
//void Arcball::startRotation(int _x, int _y)
//{
//	int x = ((_x)-(width / 2));
//	int y = ((height / 2) - _y);
//
//	startRotationVector = convertXY(x, y);
//	normalize(startRotationVector);
//
//	currentRotationVector = startRotationVector;
//	isRotating = true;
//}
//
//void Arcball::updateRotation(int _x, int _y)
//{
//	int x = ((_x)-(width / 2));
//	int y = ((height / 2) - _y);
//
//	currentRotationVector = convertXY(x, y);
//
//	normalize(currentRotationVector);
//}
//
//void Arcball::applyRotationMatrix()
//{
//	if (isRotating)
//	{  // Do some rotation according to start and current rotation vectors
//	   //cerr << currentRotationVector.transpose() << " " << startRotationVector.transpose() << endl;
//		//if ((currentRotationVector � startRotationVector).norm() > 1E-6)
//		vec3 start_to_current = currentRotationVector - startRotationVector;
//		if (norm(start_to_current) > OneDividedSixTimesTen)
//		{
//			vec3 rotationAxis = cross(currentRotationVector, startRotationVector);
//			normalize(rotationAxis);
//
//			double val = dot(currentRotationVector,startRotationVector);
//			//val > (1�1E - 10) ? val = 1.0 : val = val;
//			double rotationAngle = acos(val) * 180.0f / (float)M_PI;
//
//			// rotate around the current position
//			applyTranslationMatrix(true);
//			glRotatef(rotationAngle * 2, -rotationAxis.x(), -rotationAxis.y(), -rotationAxis.z());
//			applyTranslationMatrix(false);
//		}
//	}
//	glMultMatrixf(startMatrix);
//}
//
//void Arcball::stopRotation()
//{
//
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	applyRotationMatrix();
//	// set the current matrix as the permanent one
//	glGetFloatv(GL_MODELVIEW_MATRIX, startMatrix);
//	isRotating = false;
//}
//
//
//void Arcball::applyTranslationMatrix(bool reverse)
//{
//	float factor = (reverse ? -1.0f : 1.0f);
//	float tx = transX + (currentTransX � startTransX) * TRANSLATION_FACTOR;
//	float ty = transY + (currentTransY � startTransY) * TRANSLATION_FACTOR;
//	glTranslatef(factor * tx, factor * (-ty), 0);
//}
//
//Vector3d Arcball::convertXY(int x, int y)
//{
//
//	int d = x * x + y * y;
//	float radiusSquared = ballRadius * ballRadius;
//	if (d > radiusSquared)
//	{
//		return Vector3d((float)x, (float)y, 0);
//	}
//	else
//	{
//		return Vector3d((float)x, (float)y, sqrt(radiusSquared � d));
//	}
//}
//
///**
// * \ingroup GLVisualization
// * Reset the current transformation to the identity
//**/
//void Arcball::reset()
//{
//	fov = INITIAL_FOV;
//	// reset matrix
//	memset(startMatrix, 0, sizeof(startMatrix));
//	startMatrix[0] = 1;
//	startMatrix[1] = 0;
//	startMatrix[2] = 0;
//	startMatrix[3] = 0;
//	startMatrix[4] = 0;
//	startMatrix[5] = 1;
//	startMatrix[6] = 0;
//	startMatrix[7] = 0;
//	startMatrix[8] = 0;
//	startMatrix[9] = 0;
//	startMatrix[10] = 1;
//	startMatrix[11] = 0;
//	startMatrix[12] = 0;
//	startMatrix[13] = 0;
//	startMatrix[14] = 0;
//	startMatrix[15] = 1;
//
//	transX = transY = 0;
//	startTransX = startTransY = currentTransX = currentTransY = 0;
//}
//
//float Arcball::norm(vec3 vector)
//{
//	float norm = 0.0f;
//	norm = sqrt(pow(vector.x, 2) + pow(vector.y, 2) + pow(vector.z, 2));
//	return norm;
//}
//
//
//const float Arcball::INITIAL_FOV = 30;
//const float Arcball::TRANSLATION_FACTOR = 0.01f;