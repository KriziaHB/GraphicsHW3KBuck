//Krizia Houston Buck (x86)
//Variation on Code presented in class by Mustafa Tunc 
//Project due: 4/14/2017

#include <iostream> 
#include <fstream> 
#include <sstream> 
#include <vector> 
#include <cstdlib> 
#include <stdio.h>
#include <stdlib.h>
#include <ctime> 
#include <C:\Users\buckkr\Documents\glfw-3.2.1\LIB files\headers\GL\glew.h> 
#include <GLFW/glfw3.h> 
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp> 
#include <glm/glm/ext.hpp> 
//#include "C:\Users\buckkr\Source\Repos\GraphicsHW3KBuck\CS6350HW3\CS6350HW3\Camera2.h" 
using namespace std; 


//Prototype and global variables 
GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path); 
//Camera2 cam(glm::vec3(0.0f, 0.0f, 8.0f));
glm::vec3 lightPos(0.0f, 0.0f, 4.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);



int main() {
	//[KHB] User input choices for rotational axes 
	char axis1, axis2, axis3;

	//[KHB] Take user input for rotational axis information on each cube 
	cout << "Please enter x, y, or z for Cube #1 axis of rotation: ";
	cin >> axis1;
	cout << "Please enter x, y, or z for Cube #2 axis of rotation: ";
	cin >> axis2;
	cout << "Please enter x, y, or z for Cube #3 axis of rotation: ";
	cin >> axis3;

	//[KHB] all rotating on different axes 
	glm::vec3 rotationAlongAxis[3];{
		GLfloat x, y, z;
		if (axis1 == 'x') {
			x = 1.0f; y = 0.0f; z = 0.0f;
		}
		else if (axis1 == 'y') {
			x = 0.0f; y = 1.0f; z = 0.0f;
		}
		else if (axis1 == 'z') {
			x = 0.0f; y = 0.0f; z = 1.0f;
		}
		else {
			x = -1.0f; y = 0.0f; z = 0.0f;
		} rotationAlongAxis[0] = glm::vec3(x, y, z);
		if (axis2 == 'x') {
			x = 1.0f; y = 0.0f; z = 0.0f;
		}
		else if (axis2 == 'y') {
			x = 0.0f; y = 1.0f; z = 0.0f;
		}
		else if (axis2 == 'z') {
			x = 0.0f; y = 0.0f; z = 1.0f;
		}
		else {
			x = 0.0f; y = -1.0f; z = 0.0f;
		} rotationAlongAxis[1] = glm::vec3(x, y, z);
		if (axis3 == 'x') {
			x = 1.0f; y = 0.0f; z = 0.0f;
		}
		else if (axis3 == 'y') {
			x = 0.0f; y = 1.0f; z = 0.0f;
		}
		else if (axis3 == 'z') {
			x = 0.0f; y = 0.0f; z = 1.0f;
		}
		else {
			x = 0.0f; y = 0.0f; z = -1.0f;
		} rotationAlongAxis[2] = glm::vec3(x, y, z);
	}

	//[KHB] Custom color for each cube 
	glm::vec3 cubeColorsForEach[3];
	cubeColorsForEach[0] = glm::vec3(1, 0, 1);
	cubeColorsForEach[1] = glm::vec3(1, 1, 0);
	cubeColorsForEach[2] = glm::vec3(0, 1, 1);

	//[KHB] Cube vertices 
	GLfloat vertices[] = { // U V is for texture 
		//		X Y Z			U V				Normal 
		//bottom 
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

		//top 
		-0.5f, 0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,

		//front 
		-0.5f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 1.0f,

		//back 
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,		0.0f, 0.0f, -1.0f,
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,		1.0f, 0.0f,		0.0f, 0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,		1.0f, 0.0f,		0.0f, 0.0f, -1.0f,
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f,		0.0f, 0.0f, -1.0f,

		//left 
		-0.5f, -0.5f, 0.5f,		0.0f, 1.0f,		-1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f,		1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,		-1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f,		0.0f, 1.0f,		-1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,		1.0f, 1.0f,		-1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f,		1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,

		//right 
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f,		1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		1.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f,		1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f,		0.0f, 1.0f,		1.0f, 0.0f, 0.0f
	};

	//world space positions of cubes 
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(3.0f, 0.0f, 0.0f),
		glm::vec3(-3.0f, 0.0f, 0.0f)
	};

	//[KHB] Camera and Light angles, coordinates, and direction for camera
	glm::mat4 projection = glm::perspective(1.0f, 1280.0f / 720.0f, 0.1f, 1000.0f); //720p
	GLfloat lightAngle = 1.0f;
	GLfloat camAngle = 1.0f;
	GLfloat cubeAngle = 0.0f; //[KHB] for rotating cubes 
	GLfloat camX = 0.0f;
	GLfloat camY = 0.0f;
	GLfloat camZ = 8.0f;
	float direction = 1.0f; //[KHB] for euclidean distance for camera from origin 



	//[KHB] Setup complete 
	//Initialize glfw and window (catch errors) 
	if (!glfwInit()) {
		std::cerr << "ERROR: could not start GLFW3 \n";
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Graphics Window", NULL, NULL);
	if (!window) {
		std::cerr << "ERROR: Could not open window with GLFW3 \n";
		glfwTerminate();
		return 1;
	}

	//[KHB] switch to GLFW and initialize GLEW 
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();

	//[KHB] get and display version info then setup 
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	printf("Renderer: %s\n", renderer);
	printf("Version: %s\n", version);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//[KHB] use the added function to get the programID and LampID
	GLuint pid = LoadShaders("C:/Users/buckkr/Source/Repos/GraphicsHW3KBuck/CS6350HW3/ShaderVertex.hlsl", "C:/Users/buckkr/Source/Repos/GraphicsHW3KBuck/CS6350HW3/ShaderFragment.hlsl");
	GLuint lampid = LoadShaders("C:/Users/buckkr/Source/Repos/GraphicsHW3KBuck/CS6350HW3/LampVertex.hlsl", "C:/Users/buckkr/Source/Repos/GraphicsHW3KBuck/CS6350HW3/LampFragment.hlsl");



	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(VAO);
	//position attribute 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//[KHB] texture coordinate attribute - WILL NOT USE 
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	//normal attribute 
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);






	//[KHB] animation loop 
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//[KHB] constant rotation for light, camera, and cubes 
		lightAngle += 0.001f;
		camAngle += 0.002f; 
		cubeAngle += 0.005f; 

		// [KHB] 6 radius, 3 height for circular movement 
		lightPos = glm::vec3(6.0f*glm::cos(lightAngle), 3.0 * glm::sin(lightAngle * 3), 6.0f*glm::sin(lightAngle));

		//[KHB] use shader 
		glUseProgram(pid);

		//rotate things 
		//cam.moveTo(glm::vec3(7.0f*glm::cos(lightAngle), 0,7.0f*glm::sin(lightAngle))); 
		// [KHB] 12 radius, opposite direction as light 
/*		camX += (direction * 0.01f);					//* (-camAngle); 
		camY = 0.0f;
//		camZ = sqrt(144 - (camX*camX));					//= 8.0f;  //* (-camAngle); 
		//Changes for X and Z to keep steady path around the origin
		if (camX >= 12.0f) 
			direction = -1.0f;
		else if (camX <= -12.0f)
			direction = 1.0f;
		if ((camX < 12.0f) && (camX > 0.0f) && (direction == -1.0f)) //quadrant x-, z+
			camZ = sqrt(144 - (camX * camX));
		else if ((camX > -12.0f) && (camX < 0.0f) && (direction == -1.0f)) //quadrant x-, z-
			camZ = sqrt(144 - (camX * camX));
		else if ((camX > -12.0f) && (camX < 0.0f) && (direction == 1.0f)) //quadrant x+, z-
			camZ = -(sqrt(144 - (camX * camX)));
		else if ((camX < 12.0f) && (camX > 0.0f) && (direction == 1.0f)) //quadrant x+, z+ 
			camZ = -(sqrt(144 - (camX * camX)));
		else
			camZ += 0.01f; 
			
//		cam.Position = glm::vec3(0.0f, 0.0f, 0.0f);
//		camX = 0.0f; 
//		camZ = 10.0f; 
*/




		//[KHB] camera vectors 
		glm::vec3 camPosition = glm::vec3(camX, camY, camZ); 
		glm::vec3 camFront = glm::vec3(0.0f, 0.0f, -1.0f); 
		glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f); 
		//position, position + look at origin, up vector 
		glm::mat4 view = glm::lookAt(camPosition, camPosition + camFront, camUp);


		//[KHB] all shader info from pid and lampid 
		GLint modelLoc = glGetUniformLocation(pid, "model");
		GLint viewLoc = glGetUniformLocation(pid, "view");
		GLint projLoc = glGetUniformLocation(pid, "projection");
		GLint lightPosLoc = glGetUniformLocation(pid, "lightPosition");
		GLint lightColorLoc = glGetUniformLocation(pid, "lightColor");
		GLint camPosLoc = glGetUniformLocation(pid, "camPos");
		GLint cubeColorLoc = glGetUniformLocation(pid, "cubeColor");
		GLint revertLoc = glGetUniformLocation(pid, "revertNormals"); //use for the biggest cube 

		//set to GPU 
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(lightColorLoc, lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(camPosLoc, camX, camY, camZ);
		glUniform1i(revertLoc, 0);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);

		for (int i = 0; i < 3; i++) {
			//draw cubes 
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, cubeAngle * (i + 1), rotationAlongAxis[i]);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3f(cubeColorLoc, cubeColorsForEach[i].x, cubeColorsForEach[i].y, cubeColorsForEach[i].z);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		{ //the biggest cube 
		  //[KHB] not using the biggest cube / room 
			glm::mat4 model;
			model = glm::translate(model, cubePositions[0]);
			model = glm::scale(model, glm::vec3(15.0f, 15.0f, 15.0f));
			glUniform1i(revertLoc, 1);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3f(cubeColorLoc, 0.5, 0.5f, 0.5f);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		} 

		{ // light visualization 
			//use the added shader retrieval function to get the LampID  
			glUseProgram(lampid); 
			modelLoc = glGetUniformLocation(lampid, "model");
			viewLoc = glGetUniformLocation(lampid, "view");
			projLoc = glGetUniformLocation(lampid, "projection");
			cubeColorLoc = glGetUniformLocation(lampid, "colorChanged");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
			glUniform3f(cubeColorLoc, lightColor.x, lightColor.y, lightColor.z);
			glm::mat4 model;
			model = glm::translate(model, lightPos);
			model = glm::scale(model, glm::vec3(0.3));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(0);
		//swap the screen buffers 
		glfwSwapBuffers(window);
	}

	//clean up 
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwDestroyWindow(window);
	glfwTerminate(); 
	return 0;
}



//From provided code 
GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar(); 
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}



