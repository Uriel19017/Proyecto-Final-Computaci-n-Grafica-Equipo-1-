/*
				 Proyecto Lab CGyIHC
				        Equipo 1
					   04/11/2024
*/

#include <iostream>
#include <cmath>
#include <fstream> // Para manejo de archivos

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"


// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void Animation();
void AnimationCar();
void AnimationBoy();
void saveAnimationToFile();
void loadAnimationFromFile();

void clearAnimationFile();
void resetAnimationAndFile();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,2.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
};

float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

glm::vec3 Light1 = glm::vec3(0);

//Animaciones

//Auto
float rotWheel = 0.0f;
float rotAuto = 0.0f;

bool AnimCar = false; // Variable para activar y desactivar mi animación
bool step = false;
float CarRotation = 0.0f;  // Ángulo de rotación del carro
glm::vec3 AutoPos(0.0f, 0.0f, 0.0f);

//Boy

float rightLegRotation = 0.0f;
float leftLegRotation = 0.0f;
float rightArmRotation = 0.0f;
float leftArmRotation = 0.0f;
float headRotation = 0.0f;

bool AnimBoy = false; // Variable to activate and deactivate my animation
bool step1 = false;

float BoyRotation = 0.0f;  // Dog rotation angle
glm::vec3 BoyPos(0.0f, 0.0f, 0.0f);

//Bird 
float rightWingR = 0.0f;
float leftWingR = 0.0f;
float tailBird = 0.0f;

// Variables para manejar los 4 estados del carro
bool estado1Activo = true;
bool estado2Activo = false;
bool estado3Activo = false;
bool estado4Activo = false;
bool estado5Activo = false;
bool estado6Activo = false;

// Variables para manejar los 5 estados del Boy
bool estado1ActivoB = true;
bool estado2ActivoB = false;
bool estado3ActivoB = false;
bool estado4ActivoB = false;


//KeyFrames
float BirdPosX, BirdPosY, BirdPosZ;

#define MAX_FRAMES 70

int i_max_steps = 190;
int i_curr_steps = 0;
typedef struct _frame {
	
	float BirdPosX;
	float BirdPosY;
	float BirdPosZ;
	float BirdPosXInc;
	float BirdPosYInc;
	float BirdPosZInc;

	float rightWingR;
	float leftWingR;
	float tailBird;
	float rightWingRInc;
	float leftWingRInc;
	float tailBirdInc;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{

	printf("frameindex %d\n", FrameIndex);
	
	KeyFrame[FrameIndex].BirdPosX = BirdPosX;
	KeyFrame[FrameIndex].BirdPosY = BirdPosY;
	KeyFrame[FrameIndex].BirdPosZ = BirdPosZ;

	KeyFrame[FrameIndex].rightWingR = rightWingR;
	KeyFrame[FrameIndex].leftWingR = leftWingR;
	KeyFrame[FrameIndex].tailBird = tailBird;

	FrameIndex++;
} 

void resetElements(void)
{
	
	BirdPosX = KeyFrame[0].BirdPosX;
	BirdPosY = KeyFrame[0].BirdPosY;
	BirdPosZ = KeyFrame[0].BirdPosZ;

	rightWingR = KeyFrame[0].rightWingR;
	leftWingR = KeyFrame[0].leftWingR;
	tailBird = KeyFrame[0].tailBird;

	
}
void interpolation(void)
{

	KeyFrame[playIndex].BirdPosXInc = (KeyFrame[playIndex + 1].BirdPosX - KeyFrame[playIndex].BirdPosX) / i_max_steps;
	KeyFrame[playIndex].BirdPosYInc = (KeyFrame[playIndex + 1].BirdPosY - KeyFrame[playIndex].BirdPosY) / i_max_steps;
	KeyFrame[playIndex].BirdPosZInc = (KeyFrame[playIndex + 1].BirdPosZ - KeyFrame[playIndex].BirdPosZ) / i_max_steps;

	KeyFrame[playIndex].rightWingRInc = (KeyFrame[playIndex + 1].rightWingR - KeyFrame[playIndex].rightWingR) / i_max_steps;
	KeyFrame[playIndex].leftWingRInc = (KeyFrame[playIndex + 1].leftWingR - KeyFrame[playIndex].leftWingR) / i_max_steps;
	KeyFrame[playIndex].tailBirdInc = (KeyFrame[playIndex + 1].tailBirdInc - KeyFrame[playIndex].tailBirdInc) / i_max_steps;

}



// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();

	loadAnimationFromFile();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Lab CGyHc Equipo 1", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
	
	
	//models
	Model Piso((char*)"Models/piso.obj");
	Model arbol((char*)"Models/arbol/tree_in_OBJ.obj");
	Model poste1((char*)"Models/POSTE1/poste1.obj");
	//Auto

	Model Carroceria((char*)"Models/Auto_Lab/Carroceria.obj");
	Model Wheel((char*)"Models/Auto_Lab/Wheel.obj");

	//Boy
	Model botaDer((char*)"Models/Personaje/bota.obj");
	Model piernaDer((char*)"Models/Personaje/piernader.obj");
	Model piernaIzq((char*)"Models/Personaje/piernader.obj");
	Model torso((char*)"Models/Personaje/torso.obj");
	Model brazoDer((char*)"Models/Personaje/brazoder.obj");
	Model brazoIzq((char*)"Models/Personaje/brazoizq.obj");
	Model cabeza((char*)"Models/Personaje/cabeza.obj");

	//Bird
	Model birdBody((char*)"Models/bird/body.obj");
	Model leftWing((char*)"Models/bird/left_wing.obj");
	Model rightWing((char*)"Models/bird/left_wingtrue.obj");
	Model tail((char*)"Models/bird/tail.obj");


	//KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].BirdPosX = 0;
		KeyFrame[i].BirdPosXInc = 0;
		KeyFrame[i].BirdPosY = 0;
		KeyFrame[i].BirdPosYInc = 0;
		KeyFrame[i].BirdPosZ = 0;
		KeyFrame[i].BirdPosZInc = 0;

		KeyFrame[i].rightWingR = 0;
		KeyFrame[i].rightWingRInc = 0;
		KeyFrame[i].leftWingR = 0;
		KeyFrame[i].leftWingRInc = 0;
		KeyFrame[i].tailBird = 0;
		KeyFrame[i].tailBirdInc = 0;
	
	}


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO,EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	
	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		Animation();
		AnimationCar();
		AnimationBoy();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	   
		// OpenGL options
		glEnable(GL_DEPTH_TEST);

		
		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp
		glm::mat4 modelTempA = glm::mat4(1.0f); //Temp A
		glm::mat4 modelTempB = glm::mat4(1.0f); //Temp B
		glm::mat4 modelTempC = glm::mat4(1.0f); //Temp B
	

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"),0.6f,0.6f,0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"),0.3f, 0.3f, 0.3f);


		// Point light 1
	    glm::vec3 lightColor;
		lightColor.x= abs(sin(glfwGetTime() *Light1.x));
		lightColor.y= abs(sin(glfwGetTime() *Light1.y));
		lightColor.z= sin(glfwGetTime() *Light1.z);

		
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x,lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x,lightColor.y,lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"),0.075f);


		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));
		

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);

	
		
		//Carga de modelo 
        view = camera.GetViewMatrix();	
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);

			
 //Se configura la escala
		float scaleFactor = 0.001f; 

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		// Auto
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		modelTempA = model = glm::translate(model, AutoPos);
		modelTempA = model = glm::rotate(model, glm::radians(rotAuto), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(scaleFactor, scaleFactor, scaleFactor)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Carroceria.Draw(lightingShader);

		// Llanta izquierda delantera
		model = modelTempA; 
		model = glm::translate(model, glm::vec3(8.5f * 0.01f, 2.5f * 0.01f, 12.9f * 0.01f)); // Nueva posición
		model = glm::scale(model, glm::vec3(scaleFactor, scaleFactor, scaleFactor)); 
		model = glm::rotate(model, glm::radians(rotWheel), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Wheel.Draw(lightingShader); 

		// Llanta izquierda delantera
		model = modelTempA; 
		model = glm::translate(model, glm::vec3(-8.5f * 0.01f, 2.5f * 0.01f, 12.9f * 0.01f)); // Nueva posición
		model = glm::scale(model, glm::vec3(scaleFactor, scaleFactor, scaleFactor)); 
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotWheel), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Wheel.Draw(lightingShader); 
		
		// Llanta izquierda trasera
		model = modelTempA; 
		model = glm::translate(model, glm::vec3(-8.5f * 0.01f, 2.5f * 0.01f, -14.5f * 0.01f)); // Nueva posición
		model = glm::scale(model, glm::vec3(scaleFactor, scaleFactor, scaleFactor)); 
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotWheel), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Wheel.Draw(lightingShader); 

		// Llanta izquierda trasera
		model = modelTempA; 
		model = glm::translate(model, glm::vec3(8.5f * 0.01f, 2.5f * 0.01f, -14.5f * 0.01f)); // Nueva posición
		model = glm::scale(model, glm::vec3(scaleFactor, scaleFactor, scaleFactor)); // Aplica la escala
		model = glm::rotate(model, glm::radians(rotWheel), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Wheel.Draw(lightingShader);

		
		//Muñequito
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.3f));
		model = glm::translate(model, BoyPos);
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		modelTempB = model = glm::rotate(model, glm::radians(BoyRotation), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		torso.Draw(lightingShader);

		//Pierna Der
		model = glm::translate(modelTempB, glm::vec3(-0.5f, 0.0f, -0.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(rightLegRotation), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		piernaDer.Draw(lightingShader);

		//Pie Der
		model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		botaDer.Draw(lightingShader);

		//Pierna Izq
		model = glm::translate(modelTempB, glm::vec3(0.5f, 0.0f, -0.1f));
		model = glm::rotate(model, glm::radians(leftLegRotation), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		piernaIzq.Draw(lightingShader);

		//Pie Iz
		model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		botaDer.Draw(lightingShader);	

		//Brazo derecho
		model = glm::translate(modelTempB, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(rightArmRotation), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		brazoDer.Draw(lightingShader);

		//Brazo izquierdo
		model = glm::translate(modelTempB, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(leftArmRotation), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		brazoIzq.Draw(lightingShader);

		//Cabeza
		model = glm::translate(modelTempB, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(headRotation), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		cabeza.Draw(lightingShader);

		//Bird

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.2f, 0.6f, 0.2f));
		model = glm::translate(model, glm::vec3(BirdPosX, BirdPosY, BirdPosZ));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		modelTempC = model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		birdBody.Draw(lightingShader);

		//Ala Derecha
		model = glm::translate(modelTempC, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rightWingR), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		leftWing.Draw(lightingShader);

		//Ala Izquierda
		model = glm::translate(modelTempC, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(leftWingR), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		rightWing.Draw(lightingShader);

		//Cola Bird
		model = glm::translate(modelTempC, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(tailBird), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		tail.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(0.07f, 0.07f, 0.07f));
		model = glm::translate(model, glm::vec3(8.0f, -3.4f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		arbol.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		model = glm::translate(model, glm::vec3(-200.0f, -35.0f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		poste1.Draw(lightingShader);
		
		
		glBindVertexArray(0);
	

		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		
		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[0]);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
	

		glBindVertexArray(0);

		
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();

	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	//Dog Controls

	if (keys[GLFW_KEY_6])
	{

		rightWingR += 0.1f;

	}

	if (keys[GLFW_KEY_7])
	{

		rightWingR -= 0.1f;

	}
	if (keys[GLFW_KEY_C])
	{

		leftWingR += 0.1f;

	}

	if (keys[GLFW_KEY_V])
	{

		leftWingR -= 0.1f;

	}
	if (keys[GLFW_KEY_B])
	{

		tailBird += 0.1f;

	}

	if (keys[GLFW_KEY_N])
	{

		tailBird -= 0.07f;

	}
	
	if (keys[GLFW_KEY_Z])
	{

		BirdPosX += 0.001f;

	}

	if (keys[GLFW_KEY_X])
	{

		BirdPosX -= 0.001f;

	}
	if (keys[GLFW_KEY_4])
	{

		BirdPosY += 0.001f;

	}

	if (keys[GLFW_KEY_5])
	{

		BirdPosY -= 0.001f;

	}
	if (keys[GLFW_KEY_2])
	{
		
			BirdPosZ += 0.001f;

	}

	if (keys[GLFW_KEY_3])
	{
		
		BirdPosZ -= 0.001f;

	}
			
	

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

	if (keys[GLFW_KEY_T])
	{
		pointLightPositions[0].x += 0.01f;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.01f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.01f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.01f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.1f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.01f;
	}
	
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// Reproducir animación
	if (keys[GLFW_KEY_L]) {
		if (!play && FrameIndex > 1) {
			resetElements();
			interpolation();
			loadAnimationFromFile();
			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else {
			play = false;
		}
	}

	// Guardar la animación
	if (keys[GLFW_KEY_K]) {
		if (FrameIndex < MAX_FRAMES) {
			saveFrame();
			saveAnimationToFile();
		}
	}

	 //Limpiar el archivo y reiniciar animación al presionar 0
	if (keys[GLFW_KEY_0]) {
		resetAnimationAndFile();
	}

	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(0.2f, 0.8f, 1.0f);

		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}
	if (keys[GLFW_KEY_B])
	{
		AnimBoy = !AnimBoy; // Activa o desactiva la animación
		AnimCar = !AnimCar;

	}
}

// Limpiar el archivo de animación
void clearAnimationFile() {
	std::ofstream outFile("animBird_data.bin", std::ios::binary | std::ios::trunc);
	if (outFile.is_open()) {
		outFile.close(); 
		std::cout << "Archivo de animacion limpiado exitosamente.\n";
	}
	else {
		std::cerr << "Error al abrir el archivo para limpiar.\n";
	}
}

// Reiniciar la animación y limpiar el archivo
void resetAnimationAndFile() {
	// Limpiar el archivo de animación
	clearAnimationFile();

	// Reiniciar las posiciones del modelo a las iniciales usando KeyFrame[0]
	resetElements();

	// Reiniciar el índice de los frames
	FrameIndex = 0;

	// Reiniciar el estado de la animación
	play = false;           // Detener la animación
	playIndex = 0;

	// Guardar el estado inicial en el archivo
	saveAnimationToFile(); // Llama a esta función para guardar las posiciones iniciales

	// Reiniciar el estado de la animación
	play = false; // Detener la animación
	playIndex = 0; // Reiniciar el índice
	i_curr_steps = 0; // Reiniciar el contador de pasos
	std::cout << "Animacion reiniciada.\n";
}

// Guardar los KeyFrames en mi archivo binario
void saveAnimationToFile() {
	std::ofstream outFile("animBird_data.bin", std::ios::binary);
	if (outFile.is_open()) {
		int framesToSave = std::min(FrameIndex, MAX_FRAMES);
		outFile.write(reinterpret_cast<char*>(&framesToSave), sizeof(framesToSave));
		outFile.write(reinterpret_cast<char*>(KeyFrame), sizeof(FRAME) * framesToSave); // Guarda solo los frames utilizados
		outFile.close();
		//std::cout << "Animacion guardada exitosamente.\n";
	}
	else {
		std::cerr << "Error al abrir el archivo para guardar.\n";
	}
}

// Cargar los frames desde mi archivo binario
void loadAnimationFromFile() {
	std::ifstream inFile("animBird_data.bin", std::ios::binary);
	if (inFile.is_open()) {
		inFile.read(reinterpret_cast<char*>(&FrameIndex), sizeof(FrameIndex));
		inFile.read(reinterpret_cast<char*>(KeyFrame), sizeof(FRAME) * FrameIndex); // Carga solo la cantidad de frames guardados
		inFile.close();
		std::cout << "Animacion cargada exitosamente.\n";
	}
	else {
		std::cerr << "No se encontro archivo de animacion guardada.\n";
	}
}



void Animation() {
	if (play) {
		// Verificar si hemos llegado al final de la interpolación entre frames
		if (i_curr_steps >= i_max_steps) {
			playIndex++;
			// Verificar si hemos llegado al final de la animación
			if (playIndex >= FrameIndex - 1) {
				printf("Termina animacion\n");
				playIndex = 0; // Reiniciar al principio
				play = false;  // Detener la animación
			}
			else {
				// Preparar la interpolación para el siguiente frame
				i_curr_steps = 0; // Reiniciar el contador
				interpolation();   // Calcular la interpolación del siguiente frame
			}
		}
		else {
			// Actualizar las posiciones y rotaciones para la animación
			
			BirdPosX += KeyFrame[playIndex].BirdPosXInc;
			BirdPosY += KeyFrame[playIndex].BirdPosYInc;
			BirdPosZ += KeyFrame[playIndex].BirdPosZInc;

			rightWingR += KeyFrame[playIndex].rightWingRInc;
			leftWingR += KeyFrame[playIndex].leftWingRInc;
			tailBird += KeyFrame[playIndex].tailBirdInc;

			// Incrementar el contador de pasos
			i_curr_steps++;
		}
	}
}

void AnimationCar() {
	
	if (AnimCar) { // Si la animación del carro está activa

		// Estado 1: caminar mientras AutoPos.x < 2 y girar 90°
		if (estado1Activo) {
			if (AutoPos.z < 2.0f) {
				// Movimiento hacia adelante
				AutoPos.z += 0.001f;

				// Movimiento de las ruedas
				if (!step) {
					rotWheel -= 0.1f;
				}
			}
			else {
				// Cambiar a girar 90 grados
				if (rotAuto < 90.0f) {
					rotAuto += 0.07f;

					// Movimiento de las ruedas mientras gira
					if (!step) {
						rotWheel -= 0.1f;
					}
				}
				else {
					// Cambiar al estado 2
					estado1Activo = false;
					estado2Activo = true;
				}
			}
		}

		// Estado 2: caminar hacia adelante mientras AutoPos.z < 2
		if (estado2Activo) {
			if (AutoPos.x < 2.0f) {
				// Movimiento hacia adelante
				AutoPos.x += 0.001f;

				// Movimiento de las ruedas
				if (!step) {
					rotWheel -= 0.1f;
				}
			}
			else {
				// Girar 90 grados
				if (rotAuto < 180.0f) {
					rotAuto += 0.07f;

					// Movimiento de las ruedas mientras gira
					if (!step) {
						rotWheel -= 0.1f;
					}
				}
				else {
					// Cambiar al estado 3
					estado2Activo = false;
					estado3Activo = true;
				}
			}
		}
		// Estado 3: caminar hacia adelante mientras AutoPos.z < 2
		if (estado3Activo) {
			if (AutoPos.z > -2.0f) {
				// Movimiento hacia adelante
				AutoPos.z -= 0.001f;

				// Movimiento de las ruedas
				if (!step) {
					rotWheel -= 0.1f;
				}
			}
			else {
				// Girar 90 grados
				if (rotAuto < 270.0f) {
					rotAuto += 0.07f;

					// Movimiento de las ruedas mientras gira
					if (!step) {
						rotWheel -= 0.1f;
					}
				}
				else {
					// Cambiar al estado 4
					estado3Activo = false;
					estado4Activo = true;
				}
			}
		}
		// Estado 4: caminar hacia adelante mientras AutoPos.z < 2
		if (estado4Activo) {
			if (AutoPos.x > -2.0f) {
				// Movimiento hacia adelante
				AutoPos.x -= 0.001f;

				// Movimiento de las ruedas
				if (!step) {
					rotWheel -= 0.1f;
				}
			}
			else {
				// Girar 90 grados
				if (rotAuto < 360.0f) {
					rotAuto += 0.07f;

					// Movimiento de las ruedas mientras gira
					if (!step) {
						rotWheel -= 0.1f;
					}
				}
				else {
					
					// Cambiar al estado 4
					estado4Activo = false;
					estado5Activo = true;

				}
			}
		}
		// Estado 5: caminar hacia adelante mientras AutoPos.z < 2
		if (estado5Activo) {
			if (AutoPos.z < 2.0f) {
				// Movimiento hacia adelante
				AutoPos.z += 0.001f;

				// Movimiento de las ruedas
				if (!step) {
					rotWheel -= 0.1f;
				}
			}
			else {
				// Girar 90 grados
				if (rotAuto < 450.0f) {
					rotAuto += 0.07f;

					// Movimiento de las ruedas mientras gira
					if (!step) {
						rotWheel -= 0.1f;
					}
				}
				else {
					// Cambiar al estado 2 directamente
					estado5Activo = false;
					estado6Activo = true; // Pasar al estado 2
				}
			}
		}
		// Estado 6: caminar hacia adelante mientras AutoPos.z < 2
		if (estado6Activo) {
			if (AutoPos.x < 2.0f) {
				// Movimiento hacia adelante
				AutoPos.x += 0.001f;

				// Movimiento de las ruedas
				if (!step) {
					rotWheel -= 0.1f;
				}
			}
			else {
				// Girar 90 grados
				if (rotAuto < 540.0f) {
					rotAuto += 0.07f;

					// Movimiento de las ruedas mientras gira
					if (!step) {
						rotWheel -= 0.1f;
					}
				}
				else {
					// Cambiar al estado 3 directamente
					rotAuto = 180.0f;
					estado6Activo = false;
					estado3Activo = true; // Pasar al estado 2

				}
			}
		}
	}
}

void AnimationBoy() {
	

	if (AnimBoy) { // Walk animation

		// Estado 1: caminar mientras dogPos.z < 2 y girar 90°
		if (estado1ActivoB) {
			if (BoyPos.z < 2.0f) {
				// Movimiento hacia adelante
				BoyPos.z += 0.0009;

				// Movimiento de cabeza, cola y patas.
				if (!step1) {
					rightLegRotation += 0.1f;
					leftLegRotation -= 0.1f;
					headRotation += 0.04f;
					

					if (rightLegRotation > 45.0f) {
						step1 = true;
					}
				}
				else {
					rightLegRotation -= 0.1f;
					leftLegRotation += 0.1f;
					headRotation -= 0.04f;

					if (rightLegRotation < -45.0f) {
						step1 = false;
					}
				}
			}
			else {
				// Girar 90 grados
				if (BoyRotation < 90.0f) {
					BoyRotation += 0.07;

					// Cuando el perro gira debe seguir moviendo sus patas, cabeza y cola
					if (!step1) {
						rightLegRotation += 0.1f;
						leftLegRotation -= 0.1f;
						headRotation += 0.04f;

						if (rightLegRotation > 45.0f) {
							step1 = true;
						}
					}
					else {
						rightLegRotation -= 0.1f;
						leftLegRotation += 0.1f;
						headRotation -= 0.04f;

						if (rightLegRotation < -45.0f) {
							step1 = false;
						}
					}

				}
				else {
					// Cambiar al estado 2

					estado1ActivoB = false;
					estado2ActivoB = true;
				}
			}
		}

		// Estado 2: caminar mientras dogPos.x < 2 y girar 90°
		if (estado2ActivoB) {
			if (BoyPos.x < 2.0f) {
				// Movimiento hacia adelante
				BoyPos.x += 0.0009;

				// Movimiento de cabeza, cola y patas.
				if (!step1) {
					rightLegRotation += 0.1f;
					leftLegRotation -= 0.1f;
					headRotation += 0.04f;

					if (rightLegRotation > 45.0f) {
						step1 = true;
					}
				}
				else {
					rightLegRotation -= 0.1f;
					leftLegRotation += 0.1f;
					headRotation -= 0.04f;

					if (rightLegRotation < -45.0f) {
						step1 = false;
					}
				}
			}
			else {
				// Girar 90 grados
				if (BoyRotation < 180.0f) {
					BoyRotation += 0.07;
					// Movimiento de cabeza, cola y patas.
					if (!step1) {
						rightLegRotation += 0.1f;
						leftLegRotation -= 0.1f;
						headRotation += 0.04f;

						if (rightLegRotation > 45.0f) {
							step1 = true;
						}
					}
					else {
						rightLegRotation -= 0.1f;
						leftLegRotation += 0.1f;
						headRotation -= 0.04f;

						if (rightLegRotation < -45.0f) {
							step1 = false;
						}
					}

				}
				else {
					// Cambiar al estado 3
					estado2ActivoB = false;
					estado3ActivoB = true;
				}
			}
		}

		// Estado 3: caminar mientras dogPos.z > - 2 y girar 135°
		if (estado3ActivoB) {
			if (BoyPos.z > -2.0f) {
				// Movimiento hacia z = -2
				BoyPos.z -= 0.0009;

				// Movimiento de cabeza, cola y patas.
				if (!step1) {
					rightLegRotation += 0.1f;
					leftLegRotation -= 0.1f;
					headRotation += 0.04f;

					if (rightLegRotation > 45.0f) {
						step1 = true;
					}
				}
				else {
					rightLegRotation -= 0.1f;
					leftLegRotation += 0.1f;
					headRotation -= 0.04f;

					if (rightLegRotation < -45.0f) {
						step1 = false;
					}
				}
			}
			else {
				// Girar 135 grados
				if (BoyRotation < 315.0f) {
					BoyRotation += 0.07;
					// Movimiento de cabeza, cola y patas.
					if (!step1) {
						rightLegRotation += 0.1f;
						leftLegRotation -= 0.1f;
						headRotation += 0.04f;

						if (rightLegRotation > 45.0f) {
							step1 = true;
						}
					}
					else {
						rightLegRotation -= 0.1f;
						leftLegRotation += 0.1f;
						headRotation -= 0.04f;

						if (rightLegRotation < -45.0f) {
							step1 = false;
						}
					}
				}
				else {
					// Cambiar al estado 4
					estado3ActivoB = false;
					estado4ActivoB = true;
				}
			}
		}

		// Estado 4: Regresar a la posición inicial (origen)
		if (estado4ActivoB) {
			// Regresar al origen
			if (BoyPos.x > 0.0f) {
				BoyPos.x -= 0.0006; // Regresar en x
			}

			if (BoyPos.z < 0.0f) {
				BoyPos.z += 0.0006; // Regresar en z
			}

			// Movimiento de cabeza, cola y patas.
			if (!step1) {
				rightLegRotation += 0.1f;
				leftLegRotation -= 0.1f;
				headRotation += 0.04f;

				if (rightLegRotation > 45.0f) {
					step1 = true;
				}
			}
			else {
				rightLegRotation -= 0.1f;
				leftLegRotation += 0.1f;
				headRotation -= 0.04f;

				if (rightLegRotation < -45.0f) {
					step1 = false;
				}
			}

			// Suavizar rotación hacia 0
			if (BoyPos.x <= 0.0f && BoyPos.z >= 0.0f) {
				BoyPos.x = 0.0f; // Me aseguro de estar en la posición exacta
				BoyPos.z = 0.0f;

				if (BoyRotation < 360.0f) {
					BoyRotation += 0.07f; // Suavizar la rotación hacia 360
				}
				if (BoyRotation >= 360.0f) {
					BoyRotation = 0.0f; // Reiniciar a 0
				}
			}
			// Reiniciar estados cuando la rotación es 0
			if (BoyRotation == 0.0f) {

				// Reiniciar estados
				estado1ActivoB = true; // Reiniciar estado 1
				estado4ActivoB = false; // Desactivar estado 4
				step1 = false; // Reiniciar el paso de las patas
				rightLegRotation = 0.0f; // Reiniciar posición de las patas
				leftLegRotation = 0.0f;
				headRotation = 0.0f; // Reiniciar cabeza
				
			}
		}
	}
}
				
void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}