//Krizia Houston Buck 
//from in class code 

#include "Camera.h" 
#include <GL/glew.h> 
#include <include/GLFW/glfw3.h> 
#include <stdio.h> 
#include <iostream> 
#include <fstream> 
#include <string> 
#include <vector> 
#include <math.h> 
#include <glm/glm.hpp> 
#include <glm/vec3.hpp> 
#include <glm/vec4.hpp> 
#include <glm/mat4x4.hpp> 
#include <glm/gtc/type_ptr.hpp> 
#include <vector> 
using namespace std; 

Camera::Camera(CameraType cameraType, glm::mat4 projectionMatrix):cameraType(cameraType), mProjection(projectionMatrix) {
	cameraPosition = glm::vec3(0.0f, 0.0f, 1.0f); 
	cameraAt = glm::vec3(0.0f, 0.0f, 0.0f); //look at origin 
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //determines which axis is "up" for the camera 
	mView = glm::lookAt(cameraPosition, cameraAt, cameraUp); //model view matrix 
}

void Camera::scale(float zoomFactor) {
	glm::mat4 scale(1.0f); //identity matrix 
	scale = glm::scale(scale, glm::vec3(zoomFactor, zoomFactor, zoomFactor)); //zoom and scale 
	mProjection = scale * mProjection; //looking from 1.0 distance away * scale factor 
	mScaleProjection = scale * mScaleProjection; 
}

void Camera::translate(glm::vec3& offset) {
	glm::mat4 translation(1.0f); 
	//want to zoom in by moving everything negative z 
	//offset = - offset; 
	translation = glm::translate(translation, offset); //4x4 identity matrix 
	mView = translation * mView; 
	//get object coordinates of camera 
	cameraPosition = glm::vec3(glm::inverse(mView) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)); //3d camera position 
	//cout << cameraPosition.x << " "<< cameraPosition.y << " " << cameraPosition.z << endl; 
}

//camera will be rotated to look at current look at point 
void Camera::moveTo(glm::vec3& point) {
	cameraPosition = point; //move camera to a specific location / point 
	mView = glm::lookAt(cameraPosition, cameraAt, cameraUp); //point, vector, vector 
}

//only rotates position 
void Camera::rotate(glm::vec3& rotations) {
	glm::mat4 allAxes = glm::mat4(1.0f); 
	float one = 1.0f; 
	if (rotations.x != 0.0f) 
		allAxes = glm::rotate(allAxes, rotations.x, glm::vec3(one, 0.0f, 0.0f)); //4x4 matrix with sin/cos 
	if (rotations.y != 0.0f) 
		allAxes = glm::rotate(allAxes, rotations.y, glm::vec3(0.0f, one, 0.0f));
	if (rotations.z != 0.0f) 
		allAxes = glm::rotate(allAxes, rotations.z, glm::vec3(0.0f, 0.0f, one)); 

	cameraPosition = glm::vec3(allAxes * glm::vec4(cameraPosition, 1.0f)); //homogeneous coordinate 
	//mView = allAxes * mView; 
	//cameraPosition = glm::vec3(glm::inverse(mView) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)); 
}

//move to distance of object from center, rotate, move back 
void Camera::rotateAbout(glm::vec3 rotations, glm::vec3 point) {
	glm::mat4 mRotation(1.0f); //clear rotations 
	if (rotations.x != 0.0f)
		mRotation = glm::rotate(mRotation, rotations.x, glm::vec3(1.0f, 0.0f, 0.0f)); 
	if (rotations.y != 0.0f) 
		mRotation = glm::rotate(mRotation, rotations.y, glm::vec3(0.0f, 1.0f, 0.0f));
	if (rotations.z != 0.0f) 
		mRotation = glm::rotate(mRotation, rotations.z, glm::vec3(0.0f, 0.0f, 1.0f)); 
	glm::vec3 originalPos = cameraPosition; 
	point.y = 0.0f; 
	cameraPosition = glm::vec3(glm::translate(glm::mat4(1.0f), point) * mRotation * glm::translate(glm::mat4(1.0f), -point) * glm::vec4(cameraPosition, 1.0f)); 
	//move it to origin, do rotation, then take it back with its rotation 
}

//does not have same name semantics as glm::lookat 
//you only provide point to look at, the rest remains same 
void Camera::lookAt(glm::vec3& point) {
	cameraAt = point; 
	mView = glm::lookAt(cameraPosition, cameraAt, cameraUp); 
	//new model view matrix with new point to look at 
}

glm::vec3 Camera::windowToWorld(glm::vec3 winCoord, glm::vec4 viewPort) {
	return glm::unProject(winCoord, mView, mProjection, viewPort); 
}