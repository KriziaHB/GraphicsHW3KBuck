//Krizia Houston Buck 
//From in class code 
/*
#pragma once 
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

typedef enum { ORTHOGRAPHIC = 1, PERSPECTIVE } CameraType;
class Camera {

public:
	Camera();

	Camera(CameraType cameraType, glm::mat4 projectionMatrix);

	void lookAt(glm::vec3& point);
	void translate(glm::vec3& offset);
	void moveTo(glm::vec3& point);
	void scale(float zoomFactor);
	void rotate(glm::vec3& rotation); 
	void rotateAbout(glm::vec3 rotation, glm::vec3 point); 
	glm::mat4 getInverse() { return glm::inverse(mView); }; 
	glm::vec3 windowToWorld(glm::vec3 winCoord, glm::vec4 viewPort); 

private: 
	glm::mat4 mView; 
	glm::mat4 mProjection; 
	glm::mat4 mScaleProjection; 
	glm::vec3 cameraUp; 
	glm::vec3 cameraPosition; 
	glm::vec3 cameraAt; 
	CameraType cameraType; 
};

*/