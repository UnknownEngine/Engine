#include "CameraComponent.h"
#include "Application.h"
#include "ModuleInput.h"
#include "Globals.h"
#include "Glew/include/glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "TransformComponent.h"
#include <math.h>
#include "ModuleWindow.h"


CameraComponent::CameraComponent() : Component()
{
    type = ComponentType::Camera;
    frustum.type = FrustumType::PerspectiveFrustum;
    frustum.nearPlaneDistance = 0.1f;
    frustum.farPlaneDistance = 500.f;
    frustum.front = float3::unitZ;
    frustum.up = float3::unitY;
    fov = 60.f;
    frustum.verticalFov = fov * DEGTORAD;
    
    frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov / 2.0f) * 1.25f);

    frustum.pos = float3::zero;
}

CameraComponent::~CameraComponent()
{}


void CameraComponent::UpdateFrustum(float width, float height)
{
    frustum.verticalFov = fov * DEGTORAD;
    frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov / 2.0f) * (width / height));
    frustum.ProjectionMatrix();
    frustum.ViewMatrix();
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
//		//if ((currentRotationVector – startRotationVector).norm() > 1E-6)
//		vec3 start_to_current = currentRotationVector - startRotationVector;
//		if (norm(start_to_current) > OneDividedSixTimesTen)
//		{
//			vec3 rotationAxis = cross(currentRotationVector, startRotationVector);
//			normalize(rotationAxis);
//
//			double val = dot(currentRotationVector,startRotationVector);
//			//val > (1–1E - 10) ? val = 1.0 : val = val;
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
//	float tx = transX + (currentTransX – startTransX) * TRANSLATION_FACTOR;
//	float ty = transY + (currentTransY – startTransY) * TRANSLATION_FACTOR;
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
//		return Vector3d((float)x, (float)y, sqrt(radiusSquared – d));
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