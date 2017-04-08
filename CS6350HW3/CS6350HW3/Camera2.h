#pragma once

//GL Includes 
#include <GL/glew.h> 
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/quaternion.hpp> 
//Std. includes 
#include <vector> 


//Defines several possible options for camera movement. Used as abstraction to stay away from window-stystem specific input methods 
enum Camera_Movement {
	FORWARD, 
	BACKWARD, 
	LEFT, 
	RIGHT, 
	UP, 
	DOWN 
};

//Default camera values 
const GLfloat YAW = -90.0f; 
const GLfloat PITCH = 0.0f; 
const GLfloat SPEED = 3.0f; 
const GLfloat SENSITIVITY = 0.25f; 
const GLfloat ZOOM = 45.0f; 
const GLfloat JUMPHEIGHT = 2.0f; 

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
	//Camera options 
	GLfloat MovementSpeed; 
	GLfloat MouseSensitivity; 
	bool jumpInProgress; 

	//Constructor with vectors 
	Camera2(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY) {
		this->Position = position; 
		this->WorldUp = up; 
		this->Yaw = yaw; 
		this->Pitch = pitch; 
		this->jumpInProgress = false; 
		this->firstJump = true; 
		this->updateCameraVectors(); 
	}

	//constructor with scalar values 
	Camera2(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY) {
		this->Position = glm::vec3(posX, posY, posZ); 
		this->WorldUp = glm::vec3(upX, upY, upZ); 
		this->Yaw = yaw; 
		this->Pitch = pitch; 
		this->firstJump = true; 
		this->jumpInProgress = false; 
		this->updateCameraVectors(); 
	}

	//returns the view matrix calculated using Eular Angles and the LookAt Matrix 
	glm::mat4 GetViewMatrix() {
		return glm::lookAt(this->Position, this->Position + this->Front, this->Up); 
	}

	//Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems) 
	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime) {
		GLfloat velocity = this->MovementSpeed * deltaTime; 
		if (direction == FORWARD)
			this->Position += this->Front * velocity; 
		if (direction == BACKWARD)
			this->Position -= this->Front * velocity; 
		if (direction == LEFT)
			this->Position -= this->Right * velocity; 
		if (direction == RIGHT)
			this->Position += this->Right * velocity; 
		if (direction == UP)
			this->Position += this->Up * velocity; 
		if (direction == DOWN)
			this->Position += -this->Up * velocity; 
	}

	//processes input received from a mouse input system. Expects the offset value in both the x and y direction.. 
	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true) {
		xoffset *= this->MouseSensitivity; 
		yoffset *= this->MouseSensitivity; 

		this->Yaw += xoffset; 
		this->Pitch += yoffset; 

		//make sure that when pitch is out of bounds, screen doesn't get flipped 
		if (constrainPitch)
		{
			if (this->Pitch > 89.0f)
				this->Pitch = 89.0f; 
			if (this->Pitch < -89.0f)
				this->Pitch = -89.0f; 
		}

		//update Front, Right, and Up Vectors using the updated Eular angles 
		this->updateCameraVectors(); 
	}

	void jump(GLfloat deltaTime) {
		if (firstJump) {
			firstJump = false; 
			jumpAngle = 90.0f; 
			tempUpVec = this->Up; 
		}
		if (jumpAngle > 270.0f) {
			jumpAngle = 90.0f; 
			jumpInProgress = false; 
			tempUpVec = glm::vec3(0); 
			firstJump = true; 
		}
		jumpAngle += 1; 
		this->Position += this->tempUpVec * glm::sin(glm::radians(jumpAngle)) * this->MovementSpeed * deltaTime * JUMPHEIGHT; 
	}

private: 
	glm::vec3 tempUpVec; //to store the upVector when jumped 
	bool firstJump; 
	float jumpAngle; 
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