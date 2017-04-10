//Krizia Houston Buck 
//Variation on Code presented in class by Mustafa Tunc 
//Project due: 4/14/2017

#include <iostream> 
#include <vector> 
#include <cstdlib> 
#include <ctime> 
#include <GL/glew.h> 
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 
#include <glm/ext.hpp> 

#include "Camera2.h" 
#include "Shader.h" 
//[KHB] nto using textures 
//#include "Texture.h" 

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void doMovement();

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
//[KHB] not using textures 
//int Texture::xthTexture = 0;
glm::vec3 rotationAlongAxis[3];

int main() {
	if (!glfwInit()) {
		std::cerr << "ERROR: could not start GLFW3 \n";
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Graphics Window", glfwGetPrimaryMonitor(), NULL);
	if (!window) {
		std::cerr << "ERROR: Could not open window with GLFW3 \n";
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, 0, 0);
	glfwSetCursorPos(window, lastX, lastY);
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
	Shader shader("shader.vert", "shader.frag");
	Shader lamp("lamp.vert", "lamp.frag");

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

	// [KHB} not using textures 
	/*
	std::vector<Texture> textures;
	textures.push_back(Texture("1.png"));
	textures.push_back(Texture("2.png"));
	textures.push_back(Texture("3.png")); */


	//printf("Cam Position vec3(%f, %f, %f)", cam.getPosition().x, cam.getPosition().y, cam.getPosition().z); 
	float lightAngle = 1.0f;
	GLfloat angle = 0.0f; //for rotating cubes 

	rotationAlongAxis[0] = rotationAlongAxis[1] = rotationAlongAxis[2] = glm::vec3(0.0f, 1.0f, 1.0f);
	glm::vec3 cubeColorsForEach[3];
	cubeColorsForEach[0] = glm::vec3(1, 0, 0);
	cubeColorsForEach[1] = glm::vec3(1, 1, 0);
	cubeColorsForEach[2] = glm::vec3(0, 1, 0);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		doMovement();

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//[KHB] 
		glLoadIdentity();

		//constant rotation 
		lightAngle += 0.001f;
		angle += 0.005f;

		//rotate things 
		//cam.moveTo(glm::vec3(7.0f*glm::cos(lightAngle), 0,7.0f*glm::sin(lightAngle))); 
		lightPos = glm::vec3(7.0f*glm::cos(lightAngle), 5 * glm::sin(lightAngle * 3), 7.0f*glm::sin(lightAngle)); //7 radius, 5 height for circular movement 

		shader.useShader();
		glm::mat4 view;
		glm::mat4 projection;
		view = cam.GetViewMatrix();
		projection = glm::perspective(1.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);

		GLint modelLoc = glGetUniformLocation(shader.Program, "model");
		GLint viewLoc = glGetUniformLocation(shader.Program, "view");
		GLint projLoc = glGetUniformLocation(shader.Program, "projection");
		GLint lightPosLoc = glGetUniformLocation(shader.Program, "lightPosition");
		GLint lightColorLoc = glGetUniformLocation(shader.Program, "lightColor");
		GLint camPosLoc = glGetUniformLocation(shader.Program, "camPos");
		GLint cubeColorLoc = glGetUniformLocation(shader.Program, "cubeColor");
		GLint revertLoc = glGetUniformLocation(shader.Program, "revertNormals"); //use for the biggest cube 

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
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
/*		{ //the biggest cube 
		  //[KHB] not using the biggest cube / room 
			glm::mat4 model;
			model = glm::translate(model, cubePositions[0]);
			model = glm::scale(model, glm::vec3(25.0f, 25.0f, 25.0f));
			glUniform1i(revertLoc, 1);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			textures[0].useTexture(shader, "theTexture");
			glUniform3f(cubeColorLoc, 0.5, 0.5f, 0.5f);
			glDrawArrays(GL_TRIANGLES, 0, 36);

		} */

		{ // light visualization 
			lamp.useShader();
			modelLoc = glGetUniformLocation(lamp.Program, "model");
			viewLoc = glGetUniformLocation(lamp.Program, "view");
			projLoc = glGetUniformLocation(lamp.Program, "projection");
			cubeColorLoc = glGetUniformLocation(lamp.Program, "colorChanged");
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
