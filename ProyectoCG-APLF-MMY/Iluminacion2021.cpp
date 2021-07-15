/*
Semestre 2021-2
Proyecto Final ---- CGeIHC-FI-UNAM

Integrantes:
Aguilar Plascencia Laura Fabiola
Martínez Martínez Yanni
*/

//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"


//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;
//Variable para keyFrames
float reproduciranimacion, habilitaranimacion, guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;



Texture pisoTexture;
Texture Tagave;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;

// ******* Cine 
Model registradora;
Model silla;
Model bebidas;
Model maquina_bebida;
Model food;
Model electronico;
Model cine;
//************Objetos propuestos
Model cocacola;
Model bocina;
Model sofa;
Model lampara;
Model piso;
Model desk;
Model basurero;


Skybox skybox;

//materiales
/*Estos materiales nos permiten que el objeto interacture con la luz, porque por si
solo no lo hacen y con solo la textura tampoco*/
Material Material_brillante;
Material Material_opaco;

//luz direccional
DirectionalLight mainLight; //Luz direccional, es una luz que ilumina todos los objetos, son como el sol
//la luz direccional siempre debe ponerse, sino puede verse mal
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS]; //Luz puntual, es una luz que ilumina sólo en su radio como esfera
SpotLight spotLights[MAX_SPOT_LIGHTS]; //Luz spotlight, sólo ilumina en una sola direccion.


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

//void my_input(GLFWwindow *window); Para cuando se presione una tecla
void inputKeyframes(bool* keys);

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}




void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

	Mesh *obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

}




void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

/*******************************  K E Y F R A M E S *************************/
bool animacion = false;



//NEW// Keyframes
float posXavion = 2.0, posYavion = 2.0, posZavion = 0;
float	movAvion_x = 0.0f, movAvion_y = 0.0f, movAvion_z=0.0f;
float giroAvion = 0;

#define MAX_FRAMES 100
int i_max_steps = 90;
int i_curr_steps = 6;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float movAvion_x;		//Variable para PosicionX
	float movAvion_y;		//Variable para PosicionY
	float movAvion_z;		//Variable para PosicionZ
	float movAvion_xInc;		//Variable para IncrementoX
	float movAvion_yInc;		//Variable para IncrementoY
	float movAvion_zInc;		//Variable para IncrementoY
	float giroAvion;
	float giroAvionInc;
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 12;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void) //tecla L
{

	printf("frameindex %d\n", FrameIndex);


	KeyFrame[FrameIndex].movAvion_x = movAvion_x;
	KeyFrame[FrameIndex].movAvion_y = movAvion_y;
	KeyFrame[FrameIndex].movAvion_z = movAvion_z;
	KeyFrame[FrameIndex].giroAvion;
	//no volatil, agregar una forma de escribir a un archivo para guardar los frames
	FrameIndex++;
}

void resetElements(void) //Tecla 0
{

	movAvion_x = KeyFrame[0].movAvion_x;
	movAvion_y = KeyFrame[0].movAvion_y;
	movAvion_z = KeyFrame[0].movAvion_z;
	giroAvion = KeyFrame[0].giroAvion;
}

void interpolation(void)
{
	KeyFrame[playIndex].movAvion_xInc = (KeyFrame[playIndex + 1].movAvion_x - KeyFrame[playIndex].movAvion_x) / i_max_steps;
	KeyFrame[playIndex].movAvion_yInc = (KeyFrame[playIndex + 1].movAvion_y - KeyFrame[playIndex].movAvion_y) / i_max_steps;
	KeyFrame[playIndex].movAvion_zInc = (KeyFrame[playIndex + 1].movAvion_z - KeyFrame[playIndex].movAvion_z) / i_max_steps;
	KeyFrame[playIndex].giroAvionInc = (KeyFrame[playIndex + 1].giroAvion - KeyFrame[playIndex].giroAvion) / i_max_steps;

}


void animate(void)
{
	//Movimiento del objeto // barra espaciadora
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			printf("playindex : %d\n", playIndex);
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("Frame index= %d\n", FrameIndex);
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				//printf("entro aqu\n");
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//printf("se qued aqui\n");
			//printf("max steps: %f", i_max_steps);
			//Draw animation
			movAvion_x += KeyFrame[playIndex].movAvion_xInc;
			movAvion_y += KeyFrame[playIndex].movAvion_yInc;
			movAvion_z += KeyFrame[playIndex].movAvion_zInc;
			giroAvion += KeyFrame[playIndex].giroAvionInc;
			i_curr_steps++;
		}

	}
}

/********************** F I N  K E Y F R A M E S *********/

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);



	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	Tagave = Texture("Textures/Agave.tga");
	Tagave.LoadTextureA();


	Llanta_M = Model();
	Llanta_M.LoadModel("Models/k_rueda.3ds");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	
	// *********** *********************************** C I N E ***********************************
	registradora = Model();
	registradora.LoadModel("Models/Register.obj");
	silla = Model();
	silla.LoadModel("Models/Armchair.obj");
	bebidas = Model();
	bebidas.LoadModel("Models/Blank.obj");
	maquina_bebida = Model();
	maquina_bebida.LoadModel("Models/maquina_bebida.obj");
	food = Model();
	food.LoadModel("Models/food.obj");
	electronico = Model();
	electronico.LoadModel("Models/ElectronicRack.obj");
	cine = Model();
	cine.LoadModel("Models/sala.obj");

	cocacola = Model();
	cocacola.LoadModel("Models/door.obj"); //Cuarto tiene un bug
	bocina = Model();
	bocina.LoadModel("Models/Speaker.obj");
	sofa = Model();
	sofa.LoadModel("Models/sofa.obj");
	lampara = Model();
	lampara.LoadModel("Models/Poly.obj");
	piso = Model();
	piso.LoadModel("Models/Floor.fbx");
	desk = Model();
	desk.LoadModel("Models/desk.obj");
	basurero = Model();
	basurero.LoadModel("Models/basurero.obj");



	std::vector<std::string> skyboxFaces;
	//Nuevo SKybox
	skyboxFaces.push_back("Textures/Skybox/sp2_rt.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_lf.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_dn.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_up.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_bk.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_ft.png");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256); //Variable especular y brillo son los argumentos. Brillo muy grande
	Material_opaco = Material(0.3f, 4); //Radio pequeño y brillo pequeño

	//Variables para la animación
	float offset = 0.0f;
	float posYavion = 0.0f;
	float posXavion = 0.0f;
	float posZavion = 0.0f;
	float anguloAvion = 0.0f;

	float posXcarro = 0.0f;
	float posYcarro = 0.0f;
	float posZcarro = 0.0f;
	bool bandera = false;
	bool banderaCurva = true;
	bool banderaCarro = false;

	//posición inicial del helicóptero
	glm::vec3 posblackhawk = glm::vec3(-20.0f, 6.0f, -1.0);
	glm::vec3 desplazamiento = glm:: vec3(0.0f, 0.0f, 0.0f);
	//Posicion inicial Carro
	glm::vec3 posKitt = glm::vec3(0.0f, 0.5f, -1.5f);
	glm::vec3 desplazamientoKitt = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 luces = glm::vec3(5.0f, 5.0f, 5.0f);
	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, //Valores de color
		0.3f, 0.3f, //coeficiente ambiental, que tan intensa es la luz del la luz ambiental y coeficiente difuso es que tan intenso es el tono.
		0.0f, 0.0f, -1.0f); //Vector de direccion
	//contador de luces puntuales 
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f, //Valores de color
		0.0f, 1.0f, //Coeficiente ambiental y difuso
		2.0f, 1.5f, 1.5f, //Poisicion
		0.3f, 0.2f, 0.1f); //Valores de una ecuación de segundo grado que sirven para una atenuación
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f, //Luz ligada a la camara, en este caso tiene color blanco
		0.0f, 2.0f, //Coef ambiental y difusa
		0.0f, 0.0f, 0.0f, //Vecto de posición
		0.0f, -1.0f, 0.0f, //Vector de direccion
		1.0f, 0.0f, 0.0f,
		5.0f); //Tamaño de la circunferencia del cono, mientras más grande más grande será la circunferencia nuestra luz
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	//luz de faro
	spotLights[1] = SpotLight(0.8f, 0.0f, 0.5f, //Aqui va el color
		1.0f, 2.0f, 
		-1.5f, -1.5f, -3.7f, //Vector de posición, aproximadamente donde da origen la luz, este lo coloqué cerca del faro derecho
		-1.0f, -0.0f, 0.0f, //Vector de dirección, en este caso un vector unitario que apunta a donde ve el auto
		1.0f, 0.0f, 0.0f, 
		15.0f); //Tamaño del diametro
	spotLightCount++; 

	//luz de helicóptero
	spotLights[2] = SpotLight(1.0f, 0.0f, 0.0f, //Aqui va el color
		1.0f, 2.0f,
		-30.0f, 14.0f, -1.0f, //Vector de posición, aproximadamente donde da origen la luz, este lo coloqué cerca del faro derecho
		0.0f, -1.0f, 0.0f, //Vector de dirección, en este caso un vector unitario que apunta a donde ve el auto
		1.0f, 0.0f, 0.0f,
		15.0f); //Tamaño del diametro
	spotLightCount++;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 300.0f);
	
	//KEYFRAMES DECLARADOS INICIALES

	KeyFrame[0].movAvion_x = 0.0f;
	KeyFrame[0].movAvion_y = 0.0f;
	KeyFrame[0].movAvion_z = 0.0f;
	KeyFrame[0].giroAvion = 0;


	KeyFrame[1].movAvion_x = -10.0f;
	KeyFrame[1].movAvion_y = 0.0f;
	KeyFrame[1].movAvion_z = 0.0f;
	KeyFrame[1].giroAvion = 0;


	KeyFrame[2].movAvion_x = -12.0f;
	KeyFrame[2].movAvion_y = 5.0f;
	KeyFrame[2].movAvion_z = 0.0f;
	KeyFrame[2].giroAvion = 45;


	KeyFrame[3].movAvion_x = -17.0f;
	KeyFrame[3].movAvion_y = 0.0f;
	KeyFrame[3].movAvion_z = 0.0f;
	KeyFrame[3].giroAvion = 0;

	/*	KeyFrame[4].movAvion_x = 3.0f;
		KeyFrame[4].movAvion_y = -2.0f;
		KeyFrame[4].giroAvion = 45.0f*/;

		KeyFrame[4].movAvion_x = -25.0f;
		KeyFrame[4].movAvion_y = 0.0f;
		KeyFrame[4].movAvion_z = 0.0f;
		KeyFrame[4].giroAvion = 0.0f;

		KeyFrame[5].movAvion_x = -25.0f;
		KeyFrame[5].movAvion_y = 0.0f;
		KeyFrame[5].movAvion_z = 5.0f;
		KeyFrame[5].giroAvion = 45.0f;

		KeyFrame[6].movAvion_x = -20.0f;
		KeyFrame[6].movAvion_y = 0.0f;
		KeyFrame[6].movAvion_z = 10.0f;
		KeyFrame[6].giroAvion = 45.0f;

		KeyFrame[7].movAvion_x = -20.0f;
		KeyFrame[7].movAvion_y = 0.0f;
		KeyFrame[7].movAvion_z = 20.0f;
		KeyFrame[7].giroAvion = 45.0f;

		KeyFrame[8].movAvion_x = -30.0f;
		KeyFrame[8].movAvion_y = 0.0f;
		KeyFrame[8].movAvion_z = 25.0f;
		KeyFrame[8].giroAvion = 45.0f;

		KeyFrame[9].movAvion_x = -20.0f;
		KeyFrame[9].movAvion_y = 0.0f;
		KeyFrame[9].movAvion_z = 35.0f;
		KeyFrame[9].giroAvion = 45.0f;

		KeyFrame[10].movAvion_x = -20.0f;
		KeyFrame[10].movAvion_y = 0.0f;
		KeyFrame[10].movAvion_z = 15.0f;
		KeyFrame[10].giroAvion = 45.0f;

		KeyFrame[11].movAvion_x = 0.0f;
		KeyFrame[11].movAvion_y = 0.0f;
		KeyFrame[11].movAvion_z = 0.0f;
		KeyFrame[11].giroAvion = 0;

	float giro = 90.0f;
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		
		inputKeyframes(mainWindow.getsKeys());
		animate();

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		//luz ligada a la cámara de tipo flash 
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection()); //Le manda la direccion a donde apunta nuestra camara
		//spotLights[0].setPos(); Solo recibe un vector para modoficar la posición sin moddificar el cono
		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glm::mat4 model(1.0);
		glm::mat4 modelAux(1.0); //Sirve para poder darle jerarquia

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pisoTexture.UseTexture();
		//agregar material al plano de piso
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		/*****************************C A R R O  *************************/
		
		desplazamientoKitt = glm::vec3(movAvion_x, movAvion_y,  movAvion_z);
		//agregar su coche y ponerle material
		model = glm::mat4(1.0);
		model = glm::translate(model, posKitt + desplazamientoKitt); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		modelAux = model; //Con esto ya estamos dandole jerarquia a la llanta
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, 80 +giroAvion * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Kitt_M.RenderModel();
		giro += 30;
		//Llanta izquierda adelante:
		//model = glm::mat4(1.0);
		model = modelAux; //Envez de reiniciar la matriz le pasamos la info de model aux
		model = glm::translate(model, glm::vec3(-3.5f, -0.5f, 2.7f));  //Ajustando la posición de la llanta
		model = glm::scale(model, glm::vec3(0.017f, 0.017f, 0.017f)); //Ajustando el tamaño de la llanta
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //Movimineto de llantas
		model = glm::rotate(model, giroAvion * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Permite ver que giran la llanta
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();
		 
		
		//Llanta derecha adelante:
		//model = glm::mat4(1.0);
		model = modelAux; //Envez de reiniciar la matriz le pasamos la info de model aux
		model = glm::translate(model, glm::vec3(-3.5f, -0.5f, -3.0f));  //Ajustando la posición de la llanta
		model = glm::scale(model, glm::vec3(0.017f, 0.017f, 0.017f)); //Ajustando el tamaño de la llanta
		model = glm::rotate(model, 360 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, -360 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, giroAvion * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));  //Permite ver que giran la llanta
		//Falta girar la llanta para que se vea al revés
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();
		
		//Llanta izquierda atras:
		//model = glm::mat4(1.0);
		model = modelAux; //Envez de reiniciar la matriz le pasamos la info de model aux
		model = glm::translate(model, glm::vec3(5.1f, -0.5f, 2.7f));  //Ajustando la posición de la llanta
		model = glm::scale(model, glm::vec3(0.017f, 0.017f, 0.017f)); //Ajustando el tamaño de la llanta
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, giroAvion * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));  //Permite ver que giran la llanta
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta derecha atras:
		//model = glm::mat4(1.0);
		model = modelAux; //Envez de reiniciar la matriz le pasamos la info de model aux
		model = glm::translate(model, glm::vec3(5.1f, -0.5f, -3.0f));  //Ajustando la posición de la llanta
		model = glm::scale(model, glm::vec3(0.017f, 0.017f, 0.017f)); //Ajustando el tamaño de la llanta
		model = glm::rotate(model, 360 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, giroAvion * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));  //Permite ver que giran la llanta
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		
		
		
		/*Ejercicio 1:
		El helicóptero se desplaza en forma senoidal, llegue a un punto al avanzar, de la vuelta sobre su centro y regrese.esto de forma cíclica infinita.
			Ejercicio 2 :

			El coche avance y retrocede sin girar sobre su eje, el spotlight ilumine hacia la dirección donde el coche se está desplazand.o*/

		/***************** H E L I C O P T E R O ************/
		offset += 0.1; //Controlará la velocidad en la que sube y baja el avion.
		/*if (posXavion > -20.0f) {
			
			posXavion -= 0.01 * deltaTime; //Permite desplazar hacia enfrente de forma constante. Se recomienda multiplicar por deltaTime
			//printf("%f",&posXavion);
		}*/
		posYavion = sin(10*offset *toRadians);
		

		/************************
		************************MOVIMIENTO DE GIRO DEL HELICOPTERO
		************************
		*/

		if (posXavion >= 50.0f) { //Limite derecho
			bandera = false;
		}

		if (posXavion <= -50.0f) { //Limite izquierdo
			bandera = true;
		}

		if (posXavion >= 25.0f) { //Es el punto en donde comenzará a dar la curva el helicoptero
			banderaCurva = true; //Toma el giro derecho
		}
		if (posXavion <= -25.0f) { //Es el punto en donde comenzará a dar la curva el helicoptero
			banderaCurva = false; //Toma el giro izquierdo
		}
		if (posXavion >= -25.0f && posXavion < 25.0f) { //Reinciando el angulo del helicoptero
			anguloAvion = 0.0f; //Reinicia el ángulo cuando el trayecto es recto
		}

		desplazamiento = glm:: vec3 (posXavion , posYavion, posZavion);		//agregar incremento en X con teclado
		//desplazamiento = glm::vec3(mainWindow.getmuevex(), posYavion, 0.0f);		//agregar incremento en X con teclado
		//desplazamiento = glm::vec3(posXavion, posYavion, 0.0f);		//Se deplaza en forma diagonal

		model = glm::mat4(1.0);
		model = glm:: translate(model, posblackhawk + desplazamiento);
		//model = glm::translate(model, glm::vec3(-20.0f + mainWindow.getmuevex(), 8.0 + mainWindow.getmuevey(), -1.0)); //Moviendo el helicoptero en los X,Y
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		if (banderaCurva == false && posXavion <= -25) {
			posZavion -= 0.1 * deltaTime;
			model = glm::rotate(model, -200 + anguloAvion * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			anguloAvion -= 0.1f;
		}
		if (banderaCurva == true && posXavion >= 25) {
			posZavion += 0.1 * deltaTime;
			model = glm::rotate(model, -90 +anguloAvion * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			anguloAvion -= 0.1f;
		}
		if (bandera == false) { //Irá en negativo  <---
			posXavion -= 0.1 * deltaTime;
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //270
			/*No modifica la rotación del helicoptero cuando está dando la curva, si no se incluye
			hace un movimiento muy raro, es decir, le suma más grados al giro*/
			if (posXavion <= 25 && banderaCurva != false) { 
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			}
			
		}
		if (bandera == true) {
			posXavion += 0.1 * deltaTime;
			//posZavion += 0.1 * deltaTime;
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); 
			/*No modifica la rotación del helicoptero cuando está dando la curva, si no se incluye
			hace un movimiento muy raro, es decir, le suma más grados al giro*/
			if (posXavion >= -25 && banderaCurva != true) {
				model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			}
			
		}
		
		//model = glm::rotate(model, rotarAvion * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//agregar material al helicóptero
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Blackhawk_M.RenderModel();
		//¿Cómo ligas la luz al helicóptero?

		//********	LUZ DEL HELICOPTERO	***************
		//Defino un vector con posiciones del helicoptero
		//A la misma velocidad que el Vehiculo
		
		//glm::vec3 helicopter(posblackhawk.x + mainWindow.getmuevex(), 
		//				posblackhawk.y + mainWindow.getmuevey(), 
		//			posblackhawk.z);

		//A diferentes velocidades horizontales
		glm::vec3 unitaryY(0.0f, -1.0f, 0.0f); //Un unitario que tenga dirección hacia el suelo.
		spotLights[2].SetFlash(posblackhawk + desplazamiento, unitaryY);

		///******************************************************** CINE ********************************

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.5f, 0.5f, 0.5f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		modelAux = model; //Con esto ya estamos dandole jerarquia a la llanta
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		//model = glm::rotate(model, 0* toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		registradora.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.5f, 0.5f, 0.5f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		modelAux = model; //Con esto ya estamos dandole jerarquia a la llanta
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		//model = glm::rotate(model, 0* toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		silla.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.5f, 0.5f, 50.5f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		modelAux = model; //Con esto ya estamos dandole jerarquia a la llanta
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		//model = glm::rotate(model, 0* toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bebidas.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.5f, 0.5f, 25.5f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		modelAux = model; //Con esto ya estamos dandole jerarquia a la llanta
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//model = glm::rotate(model, 0* toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maquina_bebida.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.5f, 0.5f, 10.5f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		modelAux = model; //Con esto ya estamos dandole jerarquia a la llanta
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//model = glm::rotate(model, 0* toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		food.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.5f, 0.5f, 50.5f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		modelAux = model; //Con esto ya estamos dandole jerarquia a la llanta
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		//model = glm::rotate(model, 0* toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		electronico.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, 10.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(25.0f, 1.9f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cine.RenderModel();



		/*Agave*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 10.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Blending trasnparencia o traslucidez
		glEnable(GL_BLEND);									//Si no habilitamos este blending cuando no tenemos fondo esto se verá negro
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  //Si no habilitamos este blending cuando no tenemos fondo esto se verá negro
		Tagave.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);								//Si no habilitamos este blending cuando no tenemos fondo esto se verá negro

		//La animación nos sirve para mostrarle al usuario que no está viendo una foto, se pueden ejecutar mediante Triggers por medio de banderas.
		/*			ANIMACIÓN:
		COND1: Debe tener 2 tipos de transformaciones.
		COND2: Si estoy hablando de 1 bandera o 2 banderas, que solo sea ciclica que no esten condicioonadas por banderas estoy hablando de animación básica.
		
		COMPLEJA:
		COND1: Animación basada en funciones, es decir, tomar una función de algun movimiento fisico real, ejemplo ecuaciones reales como caida libre o tiro parabólico
		COND2: Debemos considerar almenos 5 casos que se presenten
		*/ 
		//pPara que se considere animación básica no debe tener sólo traslación rotacion o escalación, tiene que ser por lo menos una trasformacion de 2 cosas
		
		

		/************************OBJETOS************************/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(15.5f, 0.5f, 15.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cocacola.RenderModel();
		//Bocina
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(25.5f, 0.5f, 15.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bocina.RenderModel();
		//Sofa
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(15.5f, 0.5f, -15.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sofa.RenderModel();
		//Lampara
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(25.5f, 0.5f, -15.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lampara.RenderModel();
		//Piso
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-200.5f, 0.0f, 20.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		piso.RenderModel();
		//Desk
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 50.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		desk.RenderModel();
		//Basurero
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, -50.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(20.0f, 20.5f, 20.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		basurero.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}


void inputKeyframes(bool* keys)
{
	if (keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion < 1)
		{
			if (play == false && (FrameIndex > 1))
			{
				resetElements();
				//First Interpolation				
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
				printf("presiona 0 para habilitar reproducir de nuevo la animacin'\n");
				habilitaranimacion = 0;

			}
			else
			{
				play = false;

			}
		}
	}
	if (keys[GLFW_KEY_0])
	{
		if (habilitaranimacion < 1)
		{
			reproduciranimacion = 0;
			printf("Ya puedes reproducir de nuevo la animacin con la tecla de barra espaciadora'\n");
		}
	}

	if (keys[GLFW_KEY_L])
	{
		if (guardoFrame < 1)
		{
			saveFrame();
			//printf("movAvion_x es: %f\n", movAvion_x);
			//printf("movAvion_y es: %f\n", movAvion_y);
			printf("presiona P para habilitar guardar otro frame'\n");
			guardoFrame++;
			reinicioFrame = 0;
		}
	}
	if (keys[GLFW_KEY_P])
	{
		if (reinicioFrame < 1)
		{
			guardoFrame = 0;
			printf("Ya puedes guardar otro frame presionando la tecla L'\n");
		}
	}


	if (keys[GLFW_KEY_1])
	{
		if (ciclo < 1)
		{
			//printf("movAvion_x es: %f\n", movAvion_x);
			movAvion_x += 1.0f;
			printf("movAvion_x es: %f\n", movAvion_x);
			ciclo++;
			ciclo2 = 0;
			printf("Presiona la tecla 2 para poder habilitar la variable\n");
		}

	}
	if (keys[GLFW_KEY_2])
	{
		if (ciclo2 < 1)
		{
			ciclo = 0;
			printf("Ya puedes modificar tu variable presionando la tecla 1\n");
		}
	}


}