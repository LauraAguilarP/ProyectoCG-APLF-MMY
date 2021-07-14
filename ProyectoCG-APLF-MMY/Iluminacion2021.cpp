/*
Semestre 2021-2
Pr�ctica 6: Iluminaci�n
Mart�nez Mart�nez Yanni
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


//para iluminaci�n
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

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture dadoTexture;
Texture pisoTexture;
Texture Tagave;

Model Kitt_M;
Model Llanta_M;
Model Camino_M;
Model Blackhawk_M;
Model Dado_M;
Model cine;

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
PointLight pointLights[MAX_POINT_LIGHTS]; //Luz puntual, es una luz que ilumina s�lo en su radio como esfera
SpotLight spotLights[MAX_SPOT_LIGHTS]; //Luz spotlight, s�lo ilumina en una sola direccion.


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


//c�lculo del promedio de las normales para sombreado de Phong
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

void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};


	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.27f,  0.35f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.48f,	0.35f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.48f,	0.64f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.27f,	0.64f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.52f,  0.35f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.73f,	0.35f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.73f,	0.64f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.52f,	0.64f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.77f,	0.35f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.98f,	0.35f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.98f,	0.64f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.77f,	0.64f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.0f,	0.35f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.23f,  0.35f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.23f,	0.64f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.0f,	0.64f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.27f,	0.02f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.48f,  0.02f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.48f,	0.31f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.27f,	0.31f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.27f,	0.68f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.48f,  0.68f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.48f,	0.98f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.27f,	0.98f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh *cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(cubo);

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
	CrearCubo();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	dadoTexture = Texture("Textures/dado.tga");
	dadoTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	Tagave = Texture("Textures/Agave.tga");
	Tagave.LoadTextureA();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/carritoCenter2.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/k_rueda.3ds");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	Camino_M = Model();
	Camino_M.LoadModel("Models/railroad track2.obj");
	cine = Model();
	cine.LoadModel("Models/sala.obj");

	Dado_M = Model();
	Dado_M.LoadModel("Models/dadoanimales.obj");

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
	Material_opaco = Material(0.3f, 4); //Radio peque�o y brillo peque�o

	//Variables para la animaci�n
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

	//posici�n inicial del helic�ptero
	glm::vec3 posblackhawk = glm::vec3(-20.0f, 6.0f, -1.0);
	glm::vec3 desplazamiento = glm:: vec3(0.0f, 0.0f, 0.0f);
	//Posicion inicial Carro
	glm::vec3 posKitt = glm::vec3(0.0f, 0.5f, -1.5f);
	glm::vec3 desplazamientoKitt = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 luces = glm::vec3(5.0f, 5.0f, 5.0f);
	//luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, //Valores de color
		0.3f, 0.3f, //coeficiente ambiental, que tan intensa es la luz del la luz ambiental y coeficiente difuso es que tan intenso es el tono.
		0.0f, 0.0f, -1.0f); //Vector de direccion
	//contador de luces puntuales 
	unsigned int pointLightCount = 0;
	//Declaraci�n de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f, //Valores de color
		0.0f, 1.0f, //Coeficiente ambiental y difuso
		2.0f, 1.5f, 1.5f, //Poisicion
		0.3f, 0.2f, 0.1f); //Valores de una ecuaci�n de segundo grado que sirven para una atenuaci�n
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f, //Luz ligada a la camara, en este caso tiene color blanco
		0.0f, 2.0f, //Coef ambiental y difusa
		0.0f, 0.0f, 0.0f, //Vecto de posici�n
		0.0f, -1.0f, 0.0f, //Vector de direccion
		1.0f, 0.0f, 0.0f,
		5.0f); //Tama�o de la circunferencia del cono, mientras m�s grande m�s grande ser� la circunferencia nuestra luz
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
		-1.5f, -1.5f, -3.7f, //Vector de posici�n, aproximadamente donde da origen la luz, este lo coloqu� cerca del faro derecho
		-1.0f, -0.0f, 0.0f, //Vector de direcci�n, en este caso un vector unitario que apunta a donde ve el auto
		1.0f, 0.0f, 0.0f, 
		15.0f); //Tama�o del diametro
	spotLightCount++; 

	//luz de helic�ptero
	spotLights[2] = SpotLight(1.0f, 0.0f, 0.0f, //Aqui va el color
		1.0f, 2.0f,
		-30.0f, 14.0f, -1.0f, //Vector de posici�n, aproximadamente donde da origen la luz, este lo coloqu� cerca del faro derecho
		0.0f, -1.0f, 0.0f, //Vector de direcci�n, en este caso un vector unitario que apunta a donde ve el auto
		1.0f, 0.0f, 0.0f,
		15.0f); //Tama�o del diametro
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

		//informaci�n en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		//luz ligada a la c�mara de tipo flash 
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection()); //Le manda la direccion a donde apunta nuestra camara
		//spotLights[0].setPos(); Solo recibe un vector para modoficar la posici�n sin moddificar el cono
		//informaci�n al shader de fuentes de iluminaci�n
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
		model = glm::translate(model, glm::vec3(-3.5f, -0.5f, 2.7f));  //Ajustando la posici�n de la llanta
		model = glm::scale(model, glm::vec3(0.017f, 0.017f, 0.017f)); //Ajustando el tama�o de la llanta
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //Movimineto de llantas
		model = glm::rotate(model, giroAvion * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Permite ver que giran la llanta
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();
		 
		
		//Llanta derecha adelante:
		//model = glm::mat4(1.0);
		model = modelAux; //Envez de reiniciar la matriz le pasamos la info de model aux
		model = glm::translate(model, glm::vec3(-3.5f, -0.5f, -3.0f));  //Ajustando la posici�n de la llanta
		model = glm::scale(model, glm::vec3(0.017f, 0.017f, 0.017f)); //Ajustando el tama�o de la llanta
		model = glm::rotate(model, 360 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, -360 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, giroAvion * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));  //Permite ver que giran la llanta
		//Falta girar la llanta para que se vea al rev�s
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();
		
		//Llanta izquierda atras:
		//model = glm::mat4(1.0);
		model = modelAux; //Envez de reiniciar la matriz le pasamos la info de model aux
		model = glm::translate(model, glm::vec3(5.1f, -0.5f, 2.7f));  //Ajustando la posici�n de la llanta
		model = glm::scale(model, glm::vec3(0.017f, 0.017f, 0.017f)); //Ajustando el tama�o de la llanta
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, giroAvion * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));  //Permite ver que giran la llanta
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta derecha atras:
		//model = glm::mat4(1.0);
		model = modelAux; //Envez de reiniciar la matriz le pasamos la info de model aux
		model = glm::translate(model, glm::vec3(5.1f, -0.5f, -3.0f));  //Ajustando la posici�n de la llanta
		model = glm::scale(model, glm::vec3(0.017f, 0.017f, 0.017f)); //Ajustando el tama�o de la llanta
		model = glm::rotate(model, 360 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, giroAvion * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));  //Permite ver que giran la llanta
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		
		
		
		/*Ejercicio 1:
		El helic�ptero se desplaza en forma senoidal, llegue a un punto al avanzar, de la vuelta sobre su centro y regrese.esto de forma c�clica infinita.
			Ejercicio 2 :

			El coche avance y retrocede sin girar sobre su eje, el spotlight ilumine hacia la direcci�n donde el coche se est� desplazand.o*/

		/***************** H E L I C O P T E R O ************/
		offset += 0.1; //Controlar� la velocidad en la que sube y baja el avion.
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

		if (posXavion >= 25.0f) { //Es el punto en donde comenzar� a dar la curva el helicoptero
			banderaCurva = true; //Toma el giro derecho
		}
		if (posXavion <= -25.0f) { //Es el punto en donde comenzar� a dar la curva el helicoptero
			banderaCurva = false; //Toma el giro izquierdo
		}
		if (posXavion >= -25.0f && posXavion < 25.0f) { //Reinciando el angulo del helicoptero
			anguloAvion = 0.0f; //Reinicia el �ngulo cuando el trayecto es recto
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
		if (bandera == false) { //Ir� en negativo  <---
			posXavion -= 0.1 * deltaTime;
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //270
			/*No modifica la rotaci�n del helicoptero cuando est� dando la curva, si no se incluye
			hace un movimiento muy raro, es decir, le suma m�s grados al giro*/
			if (posXavion <= 25 && banderaCurva != false) { 
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			}
			
		}
		if (bandera == true) {
			posXavion += 0.1 * deltaTime;
			//posZavion += 0.1 * deltaTime;
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); 
			/*No modifica la rotaci�n del helicoptero cuando est� dando la curva, si no se incluye
			hace un movimiento muy raro, es decir, le suma m�s grados al giro*/
			if (posXavion >= -25 && banderaCurva != true) {
				model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			}
			
		}
		
		//model = glm::rotate(model, rotarAvion * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//agregar material al helic�ptero
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Blackhawk_M.RenderModel();
		//�C�mo ligas la luz al helic�ptero?

		//********	LUZ DEL HELICOPTERO	***************
		//Defino un vector con posiciones del helicoptero
		//A la misma velocidad que el Vehiculo
		
		//glm::vec3 helicopter(posblackhawk.x + mainWindow.getmuevex(), 
		//				posblackhawk.y + mainWindow.getmuevey(), 
		//			posblackhawk.z);

		//A diferentes velocidades horizontales
		glm::vec3 unitaryY(0.0f, -1.0f, 0.0f); //Un unitario que tenga direcci�n hacia el suelo.
		spotLights[2].SetFlash(posblackhawk + desplazamiento, unitaryY);


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.53f, 0.0f));
		model = glm::scale(model, glm::vec3(25.0f, 1.9f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dado_M.RenderModel();

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
		glEnable(GL_BLEND);									//Si no habilitamos este blending cuando no tenemos fondo esto se ver� negro
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  //Si no habilitamos este blending cuando no tenemos fondo esto se ver� negro
		Tagave.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);								//Si no habilitamos este blending cuando no tenemos fondo esto se ver� negro

		//La animaci�n nos sirve para mostrarle al usuario que no est� viendo una foto, se pueden ejecutar mediante Triggers por medio de banderas.
		/*			ANIMACI�N:
		COND1: Debe tener 2 tipos de transformaciones.
		COND2: Si estoy hablando de 1 bandera o 2 banderas, que solo sea ciclica que no esten condicioonadas por banderas estoy hablando de animaci�n b�sica.
		
		COMPLEJA:
		COND1: Animaci�n basada en funciones, es decir, tomar una funci�n de algun movimiento fisico real, ejemplo ecuaciones reales como caida libre o tiro parab�lico
		COND2: Debemos considerar almenos 5 casos que se presenten
		*/
		//Para que se considere animaci�n b�sica no debe tener s�lo traslaci�n rotacion o escalaci�n, tiene que ser por lo menos una trasformacion de 2 cosas
		

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