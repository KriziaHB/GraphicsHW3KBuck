//[KHB] simplified Mustafa's Camera class 

#pragma once

#include <gl\glew.h> 
#include <glm/glm/glm.hpp> 
#include <glm/glm/gtc/matrix_transform.hpp> 
#include <glm/glm/gtx/quaternion.hpp> 
#include <vector> 



//Default camera values 
const GLfloat YAW = -90.0f; 
const GLfloat PITCH = 0.0f; 

//an abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL 
class Camera2
{
public: 
	//camera attributes 
	glm::vec3 Position; 
	glm::vec3 Front; 
	glm::vec3 Up; 
	glm::vec3 Right; 
	glm::vec3 WorldUp; 
	//Eular Angles 
	GLfloat Yaw; 
	GLfloat Pitch; 

	//Constructor with vectors 
	Camera2(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)) {
		this->Position = position; 
		this->WorldUp = up; 
		this->Yaw = yaw; 
		this->Pitch = pitch; 
		this->updateCameraVectors(); 
	}



	//returns the view matrix calculated using Eular Angles and the LookAt Matrix 
	glm::mat4 GetViewMatrix() {
		return glm::lookAt(this->Position, this->Position + this->Front, this->Up); 
	}





	

private: 
	glm::vec3 tempUpVec; //to store the upVector when jumped 

	//calculates the front vector from the Camera's (updated) Eular Angles 
	void updateCameraVectors() {
		//calculate the new Front vector 
		glm::vec3 front; 
		front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch)); 
		front.y = sin(glm::radians(this->Pitch)); 
		front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch)); 
		this->Front = glm::normalize(front); 
		//also re-calculate the Right and Up vector 
		this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp)); 
		//normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement 
		this->Up = glm::normalize(glm::cross(this->Right, this->Front)); 
	}
};