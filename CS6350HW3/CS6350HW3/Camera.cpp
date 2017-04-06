//Krizia Houston Buck 
//from in class code 

#include "Camera.h" 
#include <GL/glew.h> 
#include <include/GLFW/glfw3.h> 
#include <stdio.h> 
#include <iostream> 
#include <fstream> 
#include <string> 
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
	
}