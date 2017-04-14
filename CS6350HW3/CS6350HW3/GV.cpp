//German Valenzuela
//CSCI 6350
/*
#include <stdio.h>
#include <stdlib.h> 
#include <C:\Users\buckkr\Documents\glfw-3.2.1\LIB files\headers\GL\glew.h> 
#include <GLFW/glfw3.h> 
#include <glm/glm/glm.hpp>
#include <glm/glm/gtx/transform.hpp>>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <vector> 
#include <fstream> 
#include <sstream> 
#include <iostream> 
#include <glm/glm/gtx/euler_angles.hpp> 
#include <glm/glm/gtx/norm.hpp> 
#include <glm/glm/gtc/type_ptr.hpp> 
using namespace glm; 

//Function prototypes
GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

//Global variablees
vec3 rotationAlongAxis[3];
bool rotateLight = false;
bool rotateCube = false;
bool rotateCamera = false;

int main() {

	//View and projection matrices
	mat4 view;
	mat4 projection = perspective(1.0f, 1080.0f / 720.0f, 0.1f, 100.0f);

	//Camera and light vectors
	vec3 lightPosition(0.0f, 0.0f, 10.0f);
	vec3 cameraPosition = vec3(0.0f, 0.0f, 8.0f);
	vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
	vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
	
	//Light, camera and cube's angles used in their rotations
	float lightAngle = 1.0f;
	GLfloat angle = 0.0f;
	GLfloat theta = 0.001f;

	//Initiallize GLFW/GL context and O/S window using the GLFW helper library
	glfwInit();

	//Display error if GLFW does not initialize correctly 
	if (!glfwInit())
	{
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	//Create a GLFW window we can use to draw
	GLFWwindow* window = glfwCreateWindow(1080, 720, "CSCI 6350 Computer Graphics", NULL, NULL);

	//Display error if window does not open
	if (!window)
	{
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}

	//Switch context to the GLFW window (from the CMD window)
	glfwMakeContextCurrent(window);

	//Set the key callback function, this will handle key input
	glfwSetKeyCallback(window, key_callback);

	//Move the cursor to the corner of the window out of the way
	glfwSetCursorPos(window, 0, 0);

	//Start GLEW extension handler 
	glewExperimental = GL_TRUE;
	glewInit();

	//Get and display the GL information in the CMD window (background)
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	//Set GL's special functions
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Compile, link and load the shaders
	GLuint programID = LoadShaders("C:/Users/buckkr/Source/Repos/GraphicsHW3KBuck/CS6350HW3/ShaderVertex.hlsl", "C:/Users/buckkr/Source/Repos/GraphicsHW3KBuck/CS6350HW3/ShaderFragment.hlsl");
	GLuint lampProgramID = LoadShaders("C:/Users/buckkr/Source/Repos/GraphicsHW3KBuck/CS6350HW3/LampVertex.hlsl", "C:/Users/buckkr/Source/Repos/GraphicsHW3KBuck/CS6350HW3/LampFragment.hlsl");

	//Save the vertex data in the vertices float array
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
	vec3 cubePositions[] = {
		vec3(0.0f, 0.0f, 0.0f),
		vec3(3.0f, 0.0f, 0.0f),
		vec3(-3.0f, 0.0f, 0.0f)
	};

	//Define Vertex Buffer/Array Objects
	GLuint VBO, VAO;

	//Generate Vertex Object names
	glGenVertexArrays(1, &VAO);

	//Generate Buffer Object names
	glGenBuffers(1, &VBO);

	//Bind the Vertex Array Object
	glBindVertexArray(VAO);

	//Bind the Vertex Buffer Object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Creates and initializes a Buffer Object's data
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	//Specify the location and data format of the array of generic Vertex Attributes (location) to use when rendering
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);

	//Enable the VAA
	glEnableVertexAttribArray(0);

	//Specify the location and data format of the array of generic Vertex Attributes (normals) to use when rendering
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));

	//Enable the VAA
	glEnableVertexAttribArray(2);

	//Binds the Vertex Array Object, when the argument is zero this function breaks the existing binding (VAO)
	glBindVertexArray(0);

	//Axis of rotation for each of the cubes
	rotationAlongAxis[0] = rotationAlongAxis[1] = rotationAlongAxis[2] = vec3(0.0f, 1.0f, 0.0f);

	//Array of color vectors to be used for each of the cubes
	vec3 cubeColorsForEach[] = { vec3(1, 0, 0) ,vec3(0, 1, 0),vec3(0, 0, 1) };

	//Visualization loop that ends when the escape key is pressed or the window is closed
	while (!glfwWindowShouldClose(window)) {

		//Function that processes the GLFW events
		glfwPollEvents();

		//Set the window color to black
		glClearColor(0.0f, 0.0f, 0.0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Start/stop sinusoidal movement of the camera when the user selects L 
		if (rotateLight)
			lightPosition = vec3(7.0f*cos(lightAngle), 5.0f*sin(lightAngle * 4), 7.0f*sin(lightAngle));
		lightAngle += 0.001f;
		
		//Use the custom shader program
		glUseProgram(programID);

		//Update the cameras position for it to rotate around the cubes if the user selects R
		if (rotateCamera)
		{
			theta += 0.001f;
			cameraPosition = vec3(-9.0f*cos(theta), 0, -9.0f*sin(theta));
			cameraFront = vec3(cos(theta), 0, sin(theta));
		}	

		//Get and set the current view matrix
		view = lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
		
		//Get the all of the parameters that get modified inside the custom shaders for use in the main loop 
		GLint modelLoc = glGetUniformLocation(programID, "model");
		GLint viewLoc = glGetUniformLocation(programID, "view");
		GLint projLoc = glGetUniformLocation(programID, "projection");
		GLint lightPosLoc = glGetUniformLocation(programID, "lightPosition");
		GLint lightColorLoc = glGetUniformLocation(programID, "lightColor");
		GLint camPosLoc = glGetUniformLocation(programID, "camPos");
		GLint cubeColorLoc = glGetUniformLocation(programID, "cubeColor");
		GLint revertLoc = glGetUniformLocation(programID, "revertNormals");

		//Use the parameters gathered from the shaders to set GL										
		glUniform3f(lightPosLoc, lightPosition.x, lightPosition.y, lightPosition.z);
		glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
		glUniform3f(camPosLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);
		glUniform1i(revertLoc, 0);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));

		//Binds the Vertex Array Object
		glBindVertexArray(VAO);

		//Render the cubes
		for (int i = 0; i < 3; i++) { 
			mat4 model;
			model = translate(model, cubePositions[i]);

			//Rotate cubes when the user selects C
			if (rotateCube)
				model = rotate(model, angle, rotationAlongAxis[i]);
			angle += 0.001f;

			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
			glUniform3f(cubeColorLoc, cubeColorsForEach[i].x, cubeColorsForEach[i].y, cubeColorsForEach[i].z);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


#pragma region Light

		//Use the custom light vertex and fragment shaders
		glUseProgram(lampProgramID);

		//Get the all of the parameters that get modified inside the custom shaders for use in the main loop 
		modelLoc = glGetUniformLocation(lampProgramID, "model");
		viewLoc = glGetUniformLocation(lampProgramID, "view");
		projLoc = glGetUniformLocation(lampProgramID, "projection");
		cubeColorLoc = glGetUniformLocation(lampProgramID, "colorChanged");

		//Use the parameters gathered from the shaders to set GL
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));
		glUniform3f(cubeColorLoc, 1.0f, 1.0f, 1.0f);
		mat4 model;
		model = translate(model, lightPosition);
		model = scale(model, vec3(0.3));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
#pragma endregion
		
		//Swap the screen buffers 
		glfwSwapBuffers(window);
	}

	//De-allocate resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	//Terminate GLFW
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

//Function that processes keyboard input
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	else if (key == GLFW_KEY_R && action == GLFW_PRESS)
		rotateCamera = !rotateCamera;
	else if (key == GLFW_KEY_L && action == GLFW_PRESS)
		rotateLight = !rotateLight;
	else if (key == GLFW_KEY_C && action == GLFW_PRESS)
		rotateCube = !rotateCube;
	else if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		rotationAlongAxis[0] = vec3(rand() % 20, rand() % 20, rand() % 20);
	else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
		rotationAlongAxis[1] = vec3(rand() % 20, rand() % 20, rand() % 20);
	else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
		rotationAlongAxis[2] = vec3(rand() % 20, rand() % 20, rand() % 20);
}

//Function to load, compile, and link custom shader
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
*/