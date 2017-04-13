//Krizia Houston Buck (x86)
//Variation on Code presented in class by Mustafa Tunc 
//Project due: 4/14/2017

#include <iostream> 
#include <fstream> 
#include <sstream> 
#include <vector> 
#include <cstdlib> 
#include <ctime> 
#include <C:\Users\buckkr\Documents\glfw-3.2.1\LIB files\headers\GL\glew.h> 
#include <GLFW/glfw3.h> 
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp> 
#include <glm/glm/ext.hpp> 

#include "C:\Users\buckkr\Source\Repos\GraphicsHW3KBuck\CS6350HW3\CS6350HW3\Camera2.h" 
using namespace std; 

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void doMovement();
GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path); 

const GLuint WIDTH = 1280;
const GLuint HEIGHT = 720; //720p 
Camera2 cam(glm::vec3(0.0f, 2.0f, 6.0f));
glm::vec3 lightPos(0.0f, 0.0f, 4.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
bool keys[1024];
GLfloat lastX = WIDTH / 2;
GLfloat lastY = HEIGHT / 2;
bool firstMouse = true;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
glm::vec3 rotationAlongAxis[3];

//[KHB] User input choices for rotational axes 
GLfloat x_1, y_1, z_1, x_2, y_2, z_2, x_3, y_3, z_3;

//[KHB] test all user input to make sure the floats are between -1.0 and 1.0
GLfloat inputCheck(GLfloat input) {
	if ((input > 1.0f) || (input < -1.0f))
		return 0.0f;
	else
		return input; 
}

int main() {

	//[KHB] Take user input for rotational axis information on each cube 
	GLfloat in = 0.0f; 
	cout << "Please enter float values between -1.0 and 1.0 for x, y, and z separated by a space for Cube #1: "; 
	cin >> in; 
	x_1 = inputCheck(in); 
	cin >> in; 
	y_1 = inputCheck(in);
	cin >> in; 
	z_1 = inputCheck(in);
	cout << "Please enter float values between 0.0 and 1.0 for x, y, and z separated by a space for Cube #2: ";
	cin >> in;
	x_2 = inputCheck(in);
	cin >> in;
	y_2 = inputCheck(in);
	cin >> in;
	z_2 = inputCheck(in);
	cout << "Please enter float values between 0.0 and 1.0 for x, y, and z separated by a space for Cube #3: ";
	cin >> in;
	x_3 = inputCheck(in);
	cin >> in;
	y_3 = inputCheck(in);
	cin >> in;
	z_3 = inputCheck(in);

	//[KHB] all rotating on different axes 
	rotationAlongAxis[0] = glm::vec3(x_1, y_1, z_1);
	rotationAlongAxis[1] = glm::vec3(x_2, y_2, z_2);
	rotationAlongAxis[2] = glm::vec3(x_3, y_3, z_3);

	//Initialize glfw and window (catch errors) 
	if (!glfwInit()) {
		std::cerr << "ERROR: could not start GLFW3 \n";
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Graphics Window", NULL, NULL);
	if (!window) {
		std::cerr << "ERROR: Could not open window with GLFW3 \n";
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);
	//[KHB] not using Mustafa's custom cursor 
//	glfwSetCursorPosCallback(window, mouse_callback);
//	glfwSetKeyCallback(window, key_callback);
//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//	glfwSetCursorPos(window, 0, 0);
//	glfwSetCursorPos(window, lastX, lastY);
	glewExperimental = GL_TRUE;
	glewInit();
	//get version info 
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	printf("Renderer: %s\n", renderer);
	printf("Version: %s\n", version);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	Shader shader("shader.vert", "shader.frag");
//	Shader lamp("lamp.vert", "lamp.frag");
	//use the added function to get the programID  
	GLuint pid = LoadShaders("C:/Users/buckkr/Source/Repos/GraphicsHW3KBuck/CS6350HW3/ShaderVertex.hlsl", "C:/Users/buckkr/Source/Repos/GraphicsHW3KBuck/CS6350HW3/ShaderFragment.hlsl");
	GLuint lampid = LoadShaders("C:/Users/buckkr/Source/Repos/GraphicsHW3KBuck/CS6350HW3/LampVertex.hlsl", "C:/Users/buckkr/Source/Repos/GraphicsHW3KBuck/CS6350HW3/LampFragment.hlsl");

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


	// [KHB] color for cubes 
	static const GLfloat cube1_color[36] = { 0.5 }; 
	static const GLfloat cube2_color[36] = { 0.2 }; 
	static const GLfloat cube3_color[36] = { 0.8 }; 
	static const GLfloat cube4_color[36] = { 0.1 }; 



	//world space positions of cubes 
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(3.0f, 0.0f, 0.0f),
		glm::vec3(-3.0f, 0.0f, 0.0f)
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(VAO);
	//position attribute 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//texture coordinate attribute 
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	//normal attribute 
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);



	//printf("Cam Position vec3(%f, %f, %f)", cam.getPosition().x, cam.getPosition().y, cam.getPosition().z); 
	float lightAngle = 1.0f;
	float camAngle = 1.0f; 
	GLfloat camX = 0.0f; 
	GLfloat camY = 8.0f; 
	GLfloat camZ = 0.0f; 
	float direction = 1.0f; //for euclidean distance for camera from origin 
	GLfloat angle = 0.0f; //for rotating cubes 



	glm::vec3 cubeColorsForEach[3];
	cubeColorsForEach[0] = glm::vec3(1, 0, 1);
	cubeColorsForEach[1] = glm::vec3(1, 1, 0);
	cubeColorsForEach[2] = glm::vec3(0, 1, 1);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		doMovement();

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//[KHB] 
	//	glLoadIdentity();

		//constant rotation 
		lightAngle += 0.001f;
		camAngle += 0.002f; 
		angle += 0.005f; 
		

		//rotate things 
		//cam.moveTo(glm::vec3(7.0f*glm::cos(lightAngle), 0,7.0f*glm::sin(lightAngle))); 
		// [KHB] 12 radius, opposite direction as light 
		camX += (direction * 0.01f);					//* (-camAngle); 
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
		cam.Position = glm::vec3(camX, camY, camZ); 




		//position, look at origin, up vector 
		glm::mat4 view = glm::lookAt(cam.Position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));


//		glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
		// [KHB] 4 radius, 4 height for circular movement 
		lightPos = glm::vec3(4.0f*glm::cos(lightAngle), 4.0 * glm::sin(lightAngle * 3), 4.0f*glm::sin(lightAngle)); 

		//shader.useShader();
//		glm::mat4 view;
		glm::mat4 projection;
//		view = cam.GetViewMatrix();
		projection = glm::perspective(1.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);

		//Change all shader.Program to pid COME BACK HERE 
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
		glUniform3f(camPosLoc, cam.Position.x, cam.Position.y, cam.Position.z);
		glUniform1i(revertLoc, 0);

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);
		for (int i = 0; i < 3; i++) {
			//draw cubes 
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, angle*(i + 1), rotationAlongAxis[i]);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			//[KHB] not using textures 
			//textures[i].useTexture(shader, "theTexture"); 
			glUniform3f(cubeColorLoc, cubeColorsForEach[i].x, cubeColorsForEach[i].y, cubeColorsForEach[i].z);
			//[KHB] 
	//		glColor3f(0, 1, 0); 

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		{ //the biggest cube 
		  //[KHB] not using the biggest cube / room 
			glm::mat4 model;
			model = glm::translate(model, cubePositions[0]);
			model = glm::scale(model, glm::vec3(15.0f, 15.0f, 15.0f));
			glUniform1i(revertLoc, 1);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	//		textures[0].useTexture(shader, "theTexture");
	//		glUniform3f(cubeColorLoc, 0.5, 0.5f, 0.5f);
			//[KHB] 
	//		glColor3f(0.2, 0.2, 0);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		} 

		{ // light visualization 
	//		lamp.useShader();

	//	Shader lamp("lamp.vert", "lamp.frag");
	//use the added function to get the programID  
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

void doMovement() { 
	//[KHB] not using, this is Mustafa's custom 
	//Camera controls 
	/*
	if (keys[GLFW_KEY_W])
		cam.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		cam.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		cam.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		cam.ProcessKeyboard(RIGHT, deltaTime);
	if (keys[GLFW_KEY_E])
		cam.ProcessKeyboard(UP, deltaTime);
	if (keys[GLFW_KEY_Q])
		cam.ProcessKeyboard(DOWN, deltaTime);
	if (keys[GLFW_KEY_O])
		lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	if (cam.jumpInProgress)
		cam.jump(deltaTime); */
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) { 
	//[KHB] not using, this is Mustafa's custom 
	/*
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos; //reversed since y-coordinates go from bottom to left 
	lastX = xpos;
	lastY = ypos;
	cam.ProcessMouseMovement(xoffset, yoffset); */
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) { 
	//[KHB] not using, Mustafa's custom 
	/*
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	else if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		rotationAlongAxis[0] = glm::vec3(rand() % 20, rand() % 20, rand() % 20);
	else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
		rotationAlongAxis[1] = glm::vec3(rand() % 20, rand() % 20, rand() % 20);
	else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
		rotationAlongAxis[2] = glm::vec3(rand() % 20, rand() % 20, rand() % 20);
	else if (key == GLFW_KEY_C && action == GLFW_PRESS)
		lightColor = glm::normalize(glm::vec3(rand() % 20, rand() % 20, rand() % 20));
	else if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS)
		cam.MovementSpeed += 0.7f;
	else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		cam.jumpInProgress = true;
	else if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS)
		if (cam.MovementSpeed > 0.8f) //keep the minimum speed, don't go to subzero 
			cam.MovementSpeed -= 0.7f;

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
	*/
}


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



