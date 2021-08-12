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
#include "include/irrKlang.h"
#include <irrklang\irrKlang.h>

using namespace irrklang;
#pragma comment(lib, "irrKlang.lib")

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;
//Variable para keyFrames
float reproduciranimacion, habilitaranimacion, guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;
bool angulo = true;

Texture pisoTexture;
Texture Tagave;
Texture salaCine;
Texture ma_bebidas;
Texture mesaT;
Texture regis;
Texture vasoJ;
Texture mostradorT;
Texture proyectorT;

Texture Comida;
Texture Bravo;
Texture Madera;
Texture Metales;
Texture Mosaicos;
Texture ReflejoLuces;
Texture PielesTelas;
Texture UtileriaExtra;
Texture Metal;
Texture Refri;
Texture popcorn;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;

// ******* Cine 
Model johnny;
Model val;
Model dexter;
Model mandy;
Model eduardo;

//Muebles
Model cine;
Model mesa;
Model mostrador;
Model silla;
Model sofa;
Model desk;
Model puerta;

//Alimentos y bebidas
Model food;
Model vaso;
Model bebidas;
Model coca;
Model palomitas;
Model maqPalomitasBase;
Model maqPalomitasVidrio;
Model palomitaAni;

//electronicos
Model registradora;
Model maquina_bebida;
Model electronico;
Model lampara;
Model lamparaPared;
Model monitor;
Model monitor2;
Model refrigerador;
Model pantallaD;
Model combo1;
Model combo2;
Model combo3;
Model proyector;
Model pantallaPrincipal;

Model bocina;
Model botellaVidrio;
Model cuchara;
Model escalera;
Model extintor;
Model separadores;

Model planta;

Model Sink;
//otros elementos
Model basurero;
Model piso;
Model pared;
Model techo;
Model boleto;



//Cuadros
Model cuadro1;
Model cuadro2;
Model cuadro3;
Model cuadro4;
Model cuadro5;
Model cuadro6;
Model cuadro7;
Model cuadro8;
Model cuadro9;
Model cuadro10;
Model cuadro11;
Model cuadro12;
Model cuadro13;
Model cuadro14;
Model cuadro15;
Model cuadro16;

Model personal;

Model marcoPuerta;
Model puertaI;
Model puertaD;

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
bool apagador = true;
float focos = 0;

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
float posXbravo = 40.0, posYbravo = -2.0, posZbravo = 0;
float	movBravo_x = 0.0f, movBravo_y = 0.0f, movBravo_z = 0.0f;
float giroBravo = 0;

//Ticket
float posXTicket = 40.0, posYTicket = -2.0, posZTicket = 0;
float	movTicket_x = 0.0f, movTicket_y = 0.0f, movTicket_z = 0.0f;
float giroTicket = 0;

float giroPuerta = 0;
float giroPuertaEm = 0;

#define MAX_FRAMES 100
int i_max_steps = 90;
int i_curr_steps = 6;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float movBravo_x;		//Variable para PosicionX
	float movBravo_y;		//Variable para PosicionY
	float movBravo_z;		//Variable para PosicionZ
	float movBravo_xInc;		//Variable para IncrementoX
	float movBravo_yInc;		//Variable para IncrementoY
	float movBravo_zInc;		//Variable para IncrementoY
	float giroBravo;
	float giroBravoInc;

	//Variables para GUARDAR Key Frames
	float movTicket_x;		//Variable para PosicionX
	float movTicket_y;		//Variable para PosicionY
	float movTicket_z;		//Variable para PosicionZ
	float movTicket_xInc;		//Variable para IncrementoX
	float movTicket_yInc;		//Variable para IncrementoY
	float movTicket_zInc;		//Variable para IncrementoY
	float giroTicket;
	float giroTicketInc;

	float giroPuerta;
	float giroPuertaInc;

	float giroPuertaEm;
	float giroPuertaEmInc;
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 40;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void) //tecla L
{

	printf("frameindex %d\n", FrameIndex);


	KeyFrame[FrameIndex].movBravo_x = movBravo_x;
	KeyFrame[FrameIndex].movBravo_y = movBravo_y;
	KeyFrame[FrameIndex].movBravo_z = movBravo_z;
	KeyFrame[FrameIndex].giroBravo;

	KeyFrame[FrameIndex].movTicket_x = movTicket_x;
	KeyFrame[FrameIndex].movTicket_y = movTicket_y;
	KeyFrame[FrameIndex].movTicket_z = movTicket_z;
	KeyFrame[FrameIndex].giroTicket;

	KeyFrame[FrameIndex].giroPuerta;
	KeyFrame[FrameIndex].giroPuertaEm;
	//no volatil, agregar una forma de escribir a un archivo para guardar los frames
	FrameIndex++;
}

void resetElements(void) //Tecla 0
{

	movBravo_x = KeyFrame[0].movBravo_x;
	movBravo_y = KeyFrame[0].movBravo_y;
	movBravo_z = KeyFrame[0].movBravo_z;
	giroBravo = KeyFrame[0].giroBravo;

	movTicket_x = KeyFrame[0].movTicket_x;
	movTicket_y = KeyFrame[0].movTicket_y;
	movTicket_z = KeyFrame[0].movTicket_z;
	giroTicket = KeyFrame[0].giroTicket;

	giroPuerta = KeyFrame[0].giroPuerta;
	giroPuertaEm = KeyFrame[0].giroPuerta;
}

void interpolation(void)
{
	KeyFrame[playIndex].movBravo_xInc = (KeyFrame[playIndex + 1].movBravo_x - KeyFrame[playIndex].movBravo_x) / i_max_steps;
	KeyFrame[playIndex].movBravo_yInc = (KeyFrame[playIndex + 1].movBravo_y - KeyFrame[playIndex].movBravo_y) / i_max_steps;
	KeyFrame[playIndex].movBravo_zInc = (KeyFrame[playIndex + 1].movBravo_z - KeyFrame[playIndex].movBravo_z) / i_max_steps;
	KeyFrame[playIndex].giroBravoInc = (KeyFrame[playIndex + 1].giroBravo - KeyFrame[playIndex].giroBravo) / i_max_steps;

	KeyFrame[playIndex].movTicket_xInc = (KeyFrame[playIndex + 1].movTicket_x - KeyFrame[playIndex].movTicket_x) / i_max_steps;
	KeyFrame[playIndex].movTicket_yInc = (KeyFrame[playIndex + 1].movTicket_y - KeyFrame[playIndex].movTicket_y) / i_max_steps;
	KeyFrame[playIndex].movTicket_zInc = (KeyFrame[playIndex + 1].movTicket_z - KeyFrame[playIndex].movTicket_z) / i_max_steps;
	KeyFrame[playIndex].giroTicketInc = (KeyFrame[playIndex + 1].giroTicket - KeyFrame[playIndex].giroTicket) / i_max_steps;

	KeyFrame[playIndex].giroPuertaInc = (KeyFrame[playIndex + 1].giroPuerta - KeyFrame[playIndex].giroPuerta) / i_max_steps;
	KeyFrame[playIndex].giroPuertaEmInc = (KeyFrame[playIndex + 1].giroPuertaEm - KeyFrame[playIndex].giroPuertaEm) / i_max_steps;

}


int animate(void)
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
			movBravo_x += KeyFrame[playIndex].movBravo_xInc;
			movBravo_y += KeyFrame[playIndex].movBravo_yInc;
			movBravo_z += KeyFrame[playIndex].movBravo_zInc;
			giroBravo += KeyFrame[playIndex].giroBravoInc;

			movTicket_x += KeyFrame[playIndex].movTicket_xInc;
			movTicket_y += KeyFrame[playIndex].movTicket_yInc;
			movTicket_z += KeyFrame[playIndex].movTicket_zInc;
			giroTicket += KeyFrame[playIndex].giroTicketInc;

			giroPuerta += KeyFrame[playIndex].giroPuertaInc;
			giroPuertaEm += KeyFrame[playIndex].giroPuertaEmInc;
			i_curr_steps++;
		}

	}
	return playIndex;
}

/********************** F I N  K E Y F R A M E S *********/

int main()
{
	float angulo = 0.0f;
	irrklang::ISoundEngine *SoundEngine = createIrrKlangDevice();
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();
	
	CreateObjects();
	
	CreateShaders();
	//Se cambió el penultimo valor para que la camara tenga  una velocidad normal
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 1.0f, 0.5f);



	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	Tagave = Texture("Textures/Agave.tga");
	Tagave.LoadTextureA();

	//Texturas cine
	
	salaCine = Texture("Textures/salaCine.tga");
	salaCine.LoadTextureA();
	ma_bebidas = Texture("Textures/bebidas.tga");
	ma_bebidas.LoadTextureA();
	proyectorT = Texture("Textures/proyector.tga");
	proyectorT.LoadTextureA();
	regis = Texture("Textures/Registradora.tga");
	regis.LoadTextureA();
	vasoJ = Texture("Textures/vaso.tga");
	vasoJ.LoadTextureA();
	mostradorT = Texture("Textures/mostrador.tga");
	mostradorT.LoadTextureA();

	Llanta_M = Model();
	Llanta_M.LoadModel("Models/k_rueda.3ds");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	
	// *********** *********************************** C I N E ***********************************
	//Personajes
	johnny = Model();
	johnny.LoadModel("Models/johnny2.obj");
	val = Model();
	val.LoadModel("Models/val.obj");
	dexter = Model();
	dexter.LoadModel("Models/dexter.fbx");
	mandy = Model();
	mandy.LoadModel("Models/mandy.obj");
	eduardo = Model();
	eduardo.LoadModel("Models/eduardo.fbx");
	personal = Model();
	personal.LoadModel("Models/personaje.obj");

	//Muebles 
	cine = Model();
	cine.LoadModel("Models/sala.obj");
	silla = Model();
	silla.LoadModel("Models/silla.fbx"); 
	mesa = Model();
	mesa.LoadModel("Models/mesa.obj");
	mostrador = Model();
	mostrador.LoadModel("Models/mostrador.obj");
	puerta = Model();
	puerta.LoadModel("Models/puerta1.obj"); 
	sofa = Model();
	sofa.LoadModel("Models/sofa.obj");
	desk = Model();
	desk.LoadModel("Models/desk.obj");
	separadores = Model();
	separadores.LoadModel("Models/separadores.obj");

	//Alimentos y bebidas
	food = Model();
	food.LoadModel("Models/food.obj");
	coca = Model();
	coca.LoadModel("Models/coca.fbx");
	vaso = Model();
	vaso.LoadModel("Models/vasoJ.obj");
	palomitas = Model();
	palomitas.LoadModel("Models/palomitas.obj");
	bebidas = Model();
	bebidas.LoadModel("Models/Blank.obj");
	maqPalomitasBase = Model();
	maqPalomitasBase.LoadModel("Models/maqPalomitasBase.obj");
	maqPalomitasVidrio = Model();
	maqPalomitasVidrio.LoadModel("Models/maqPalomitasVidrio.obj");
	palomitaAni = Model();
	palomitaAni.LoadModel("Models/palomita.obj");

	//Electronicos
	electronico = Model();
	electronico.LoadModel("Models/ElectronicRack.obj");
	registradora = Model();
	registradora.LoadModel("Models/Register.obj");
	maquina_bebida = Model();
	maquina_bebida.LoadModel("Models/maquina_bebidas.obj");
	monitor = Model();
	monitor.LoadModel("Models/monitor1.obj");
	monitor2 = Model();
	monitor2.LoadModel("Models/monitor3.obj");
	refrigerador = Model();
	refrigerador.LoadModel("Models/refri.fbx");
	lampara = Model();
	lampara.LoadModel("Models/lampara.obj");
	pantallaD= Model();
	pantallaD.LoadModel("Models/pantallaDulceria.obj");
	combo1 = Model();
	combo1.LoadModel("Models/combo1.obj");
	combo2 = Model();
	combo2.LoadModel("Models/combo2.obj");
	combo3 = Model();
	combo3.LoadModel("Models/combo3.obj");
	proyector = Model();
	proyector.LoadModel("Models/proyector.obj");
	pantallaPrincipal = Model();
	pantallaPrincipal.LoadModel("Models/pantalla.obj");
	
	//otros elementos
	basurero = Model();
	basurero.LoadModel("Models/basura.obj");
	boleto = Model();
	boleto.LoadModel("Models/boleto.obj");
	piso = Model();
	piso.LoadModel("Models/piso.obj");

	pared = Model();
	pared.LoadModel("Models/pared.obj");
	techo = Model();
	techo.LoadModel("Models/techo.obj");
	lamparaPared = Model();
	lamparaPared.LoadModel("Models/lamparaPared.obj");
	
	//Cuadros:
	cuadro1 = Model();
	cuadro1.LoadModel("Models/cuadro1.obj");
	cuadro2 = Model();
	cuadro2.LoadModel("Models/cuadro2.obj");
	cuadro3 = Model();
	cuadro3.LoadModel("Models/cuadro3.obj");
	cuadro4 = Model();
	cuadro4.LoadModel("Models/cuadro4.obj");
	cuadro5 = Model();
	cuadro5.LoadModel("Models/cuadro5.obj");
	cuadro6 = Model();
	cuadro6.LoadModel("Models/cuadro6.obj");
	cuadro7 = Model();
	cuadro7.LoadModel("Models/cuadro7.obj");
	cuadro8 = Model();
	cuadro8.LoadModel("Models/cuadro8.obj");
	cuadro9 = Model();
	cuadro9.LoadModel("Models/cuadro9.obj");
	cuadro10 = Model();
	cuadro10.LoadModel("Models/cuadro10.obj");
	cuadro11 = Model();
	cuadro11.LoadModel("Models/cuadro11.obj");
	cuadro12 = Model();
	cuadro12.LoadModel("Models/cuadro12.obj");
	cuadro13 = Model();
	cuadro13.LoadModel("Models/cuadro13.obj");
	cuadro14 = Model();
	cuadro14.LoadModel("Models/cuadro14.obj");
	cuadro15 = Model();
	cuadro15.LoadModel("Models/cuadro15.obj");
	cuadro16 = Model();
	cuadro16.LoadModel("Models/cuadro16.obj");

	//Puerta para animación:
	marcoPuerta = Model();
	marcoPuerta.LoadModel("Models/marco2.obj");
	puertaI = Model();
	puertaI.LoadModel("Models/puertaIzq.obj");
	puertaD = Model();
	puertaD.LoadModel("Models/puertaDer.obj");

	cuchara = Model();
	cuchara.LoadModel("Models/cuchara.obj");
	extintor = Model();
	extintor.LoadModel("Models/existor.obj");
	
	
	std::vector<std::string> skyboxFaces;
	//Nuevo SKybox
	skyboxFaces.push_back("Textures/Skybox/sp2_rt.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_lf.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_dn.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_up.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_bk.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_ft.png");

	skybox = Skybox(skyboxFaces);
	glEnable(GL_LIGHTING);
	Material_brillante = Material(4.0f, 256); //Variable especular y brillo son los argumentos. Brillo muy grande
	Material_opaco = Material(0.3f, 4); //Radio pequeño y brillo pequeño

	//Variables para la animación
	float offset = 0.0f;
	int indiceFrame = 0;

	bool bandera = true;
	//Var para palomita
	float posXpalomita = 0.0f;
	float posYpalomita = 0.0f;
	float posZpalomita = 0.0f;
	float anguloAvion = 0.0f;
	bool banderaPalomitaY = true;
	bool banderaPalomitaZ = true;
	glm::vec3 posPalomita = glm::vec3(-55.2f, 4.1f, 0.5f);
	glm::vec3 desplazamientoPalomita = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 desplazamientoPalomita2 = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 desplazamientoPalomita3 = glm::vec3(0.0f, 0.0f, 0.0f);

	//Variables Jhonny animación
	glm::vec3 posBravo = glm::vec3(40.0f, -2.0f, 0.0f);
	glm::vec3 desplazamientoBravo = glm::vec3(0.0f, 0.0f, 0.0f);

	//Variables Ticket
	glm::vec3 posTicket = glm::vec3(4.73f, 2.95f, -14.7f);
	glm::vec3 desplazamientoTicket = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec3 luces = glm::vec3(5.0f, 5.0f, 5.0f);
	
	//luz direccional, sólo 1 y siempre debe de existir
	/*mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, //Valores de color
		0.3f, 0.3f, //coeficiente ambiental, que tan intensa es la luz del la luz ambiental y coeficiente difuso es que tan intenso es el tono.
		0.0f, 0.0f, -1.0f); //Vector de direccion*/
	
	//contador de luces puntuales 

	
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
		30.2f, 11.5f, 30.0f,
		0.0f, -2.0f, -2.0f,
		0.3f, 0.3f, 0.3f,
		15.0f);
	spotLightCount++;

	



	
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 300.0f);


	/********************** F I N  K E Y F R A M E S *********/

		//KEYFRAMES DECLARADOS INICIALES

	KeyFrame[0].movBravo_x = 0.0f;
	KeyFrame[0].movBravo_y = 0.0f;
	KeyFrame[0].movBravo_z = 0.0f;
	KeyFrame[0].giroBravo = 0;

	KeyFrame[1].movBravo_x = 0.0f;
	KeyFrame[1].movBravo_y = 0.0f;
	KeyFrame[1].movBravo_z = 6.0f;
	KeyFrame[1].giroBravo = 0;


	KeyFrame[2].movBravo_x = -21.0f;
	KeyFrame[2].movBravo_y = 0.0f;
	KeyFrame[2].movBravo_z = 6.0f;
	KeyFrame[2].giroBravo = -90;

	//Llega a los separadores de la taquilla
	KeyFrame[3].movBravo_x = -21.0f;
	KeyFrame[3].movBravo_y = 0.0f;
	KeyFrame[3].movBravo_z = -3.0f;
	KeyFrame[3].giroBravo = -180;

	KeyFrame[4].movBravo_x = -40.0f;
	KeyFrame[4].movBravo_y = 0.0f;
	KeyFrame[4].movBravo_z = -3.0f;
	KeyFrame[4].giroBravo = -90.0f;

	KeyFrame[5].movBravo_x = -40.0f;
	KeyFrame[5].movBravo_y = 0.0f;
	KeyFrame[5].movBravo_z = -7.0f;
	KeyFrame[5].giroBravo = -180.0f;

	//Está de frente al empleado: Y el ticket comienza a moverse DINERO.MP3
	KeyFrame[6].movBravo_x = -35.0f;
	KeyFrame[6].movBravo_y = 0.0f;
	KeyFrame[6].movBravo_z = -7.0f;
	KeyFrame[6].giroBravo = -180.0f;
	KeyFrame[6].movTicket_x = 0.0f;
	KeyFrame[6].movTicket_y = 2.0f;
	KeyFrame[6].movTicket_z = 3.0f;
	KeyFrame[6].giroTicket = 90;

	KeyFrame[7].movBravo_x = -35.0f;
	KeyFrame[7].movBravo_y = 0.0f;
	KeyFrame[7].movBravo_z = -7.0f;
	KeyFrame[7].giroBravo = -180.0f;
	KeyFrame[7].movTicket_x = 0.0f;
	KeyFrame[7].movTicket_y = 2.0f;
	KeyFrame[7].movTicket_z = 5.0f;
	KeyFrame[7].giroTicket = 90;

	//Se retira de la taquilla:
	KeyFrame[8].movBravo_x = -75.0f;
	KeyFrame[8].movBravo_y = 0.0f;
	KeyFrame[8].movBravo_z = -7.0f;
	KeyFrame[8].giroBravo = -90.0f;

	KeyFrame[9].movBravo_x = -75.0f;
	KeyFrame[9].movBravo_y = 0.0f;
	KeyFrame[9].movBravo_z = 4.0f;
	KeyFrame[9].giroBravo = 0.0f;

	KeyFrame[10].movBravo_x = -83.0f;
	KeyFrame[10].movBravo_y = 0.0f;
	KeyFrame[10].movBravo_z = 4.0f;
	KeyFrame[10].giroBravo = -90.0f; //

	//PALOMITAS INICIO
	KeyFrame[11].movBravo_x = -83.0f;
	KeyFrame[11].movBravo_y = 0.0f;
	KeyFrame[11].movBravo_z = 24.0f;
	KeyFrame[11].giroBravo = 0.0f;

	//Está frente la dulcería:  FIN PALOMITAS.MP3
	KeyFrame[12].movBravo_x = -91.0f;
	KeyFrame[12].movBravo_y = 0.0f;
	KeyFrame[12].movBravo_z = 24.0f;
	KeyFrame[12].giroBravo = -90;

	KeyFrame[13].movBravo_x = -91.0f;
	KeyFrame[13].movBravo_y = 0.0f;
	KeyFrame[13].movBravo_z = 24.0f;
	KeyFrame[13].giroBravo = -90;

	//Se dirige a la puerta, PUERTA.MP3
	KeyFrame[14].movBravo_x = -91.0f;
	KeyFrame[14].movBravo_y = 0.0f;
	KeyFrame[14].movBravo_z = 14.0f;
	KeyFrame[14].giroBravo = -180;
	KeyFrame[14].giroPuerta = 90;

	KeyFrame[15].movBravo_x = -91.0f;
	KeyFrame[15].movBravo_y = 0.0f;
	KeyFrame[15].movBravo_z = -18.0f;
	KeyFrame[15].giroBravo = -90;
	KeyFrame[15].giroPuerta = 90;

	KeyFrame[16].movBravo_x = -175.0f;
	KeyFrame[16].movBravo_y = 0.0f;
	KeyFrame[16].movBravo_z = -18.0f;
	KeyFrame[16].giroBravo = -90;
	//Entra al cine
	KeyFrame[17].movBravo_x = -175.0f;
	KeyFrame[17].movBravo_y = 0.0f;
	KeyFrame[17].movBravo_z = -8.0f;
	KeyFrame[17].giroBravo = 0;

	KeyFrame[18].movBravo_x = -160.0f;
	KeyFrame[18].movBravo_y = 0.0f;
	KeyFrame[18].movBravo_z = -8.0f;
	KeyFrame[18].giroBravo = 90; //TOdo cool

	//Empieza a subir las escaleras 
	KeyFrame[19].movBravo_x = -133.0f;
	KeyFrame[19].movBravo_y = 10.0f;
	KeyFrame[19].movBravo_z = -8.0f;
	KeyFrame[19].giroBravo = 90;
	//Gira para sentarse INTO JHONNY --> 26 
	KeyFrame[20].movBravo_x = -133.0f;
	KeyFrame[20].movBravo_y = 10.0f;
	KeyFrame[20].movBravo_z = 12.0f;
	KeyFrame[20].giroBravo = 0;
	//Se sienta
	KeyFrame[21].movBravo_x = -133.0f;
	KeyFrame[21].movBravo_y = 7.0f;
	KeyFrame[21].movBravo_z = 12.0f;
	KeyFrame[21].giroBravo = -90;

	KeyFrame[22].movBravo_x = -133.0f;
	KeyFrame[22].movBravo_y = 7.0f;
	KeyFrame[22].movBravo_z = 12.0f;
	KeyFrame[22].giroBravo = -90;

	KeyFrame[23].movBravo_x = -133.0f;
	KeyFrame[23].movBravo_y = 7.0f;
	KeyFrame[23].movBravo_z = 12.0f;
	KeyFrame[23].giroBravo = -90;

	KeyFrame[24].movBravo_x = -133.0f;
	KeyFrame[24].movBravo_y = 7.0f;
	KeyFrame[24].movBravo_z = 12.0f;
	KeyFrame[24].giroBravo = -90;

	KeyFrame[25].movBravo_x = -133.0f;
	KeyFrame[25].movBravo_y = 7.0f;
	KeyFrame[25].movBravo_z = 12.0f;
	KeyFrame[25].giroBravo = -90;

	KeyFrame[26].movBravo_x = -133.0f;
	KeyFrame[26].movBravo_y = 7.0f;
	KeyFrame[26].movBravo_z = 12.0f;
	KeyFrame[26].giroBravo = -90;
	//Se levanta
	KeyFrame[27].movBravo_x = -135.0f;
	KeyFrame[27].movBravo_y = 10.0f;
	KeyFrame[27].movBravo_z = 12.0f;
	KeyFrame[27].giroBravo = -90;
	//Va a las escaleras APLAUSOS.MP3
	KeyFrame[28].movBravo_x = -135.0f;
	KeyFrame[28].movBravo_y = 10.0f;
	KeyFrame[28].movBravo_z = -8.0f;
	KeyFrame[28].giroBravo = -180;

	KeyFrame[29].movBravo_x = -160.0f;
	KeyFrame[29].movBravo_y = 0.0f;
	KeyFrame[29].movBravo_z = -8.0f;
	KeyFrame[29].giroBravo = -90;
	//Aquí baja de las escaleras

	// Sale de la sala PUERTA.MP3
	KeyFrame[30].movBravo_x = -175.0f;
	KeyFrame[30].movBravo_y = 0.0f;
	KeyFrame[30].movBravo_z = -8.0f;
	KeyFrame[30].giroBravo = -90;

	//Gira para salir del cine
	KeyFrame[31].movBravo_x = -175.0f;
	KeyFrame[31].movBravo_y = 0.0f;
	KeyFrame[31].movBravo_z = 38.0f;
	KeyFrame[31].giroBravo = 0;
	KeyFrame[31].giroPuertaEm = 90;

	//Llega al final del pasillo de emergencia
	KeyFrame[32].movBravo_x = -91.0f;
	KeyFrame[32].movBravo_y = 0.0f;
	KeyFrame[32].movBravo_z = 38.0f;
	KeyFrame[32].giroBravo = 90;
	KeyFrame[32].giroPuertaEm = 90;

	KeyFrame[33].movBravo_x = -91.0f;
	KeyFrame[33].movBravo_y = 0.0f;
	KeyFrame[33].movBravo_z = 30.0f;
	KeyFrame[33].giroBravo = 180;

	//Llega antes del sofá
	KeyFrame[34].movBravo_x = -65.0f;
	KeyFrame[34].movBravo_y = 0.0f;
	KeyFrame[34].movBravo_z = 30.0f;
	KeyFrame[34].giroBravo = 90;

	//
	KeyFrame[35].movBravo_x = -65.0f;
	KeyFrame[35].movBravo_y = 0.0f;
	KeyFrame[35].movBravo_z = 20.0f;
	KeyFrame[35].giroBravo = 180;

	KeyFrame[36].movBravo_x = -55.0f;
	KeyFrame[36].movBravo_y = 0.0f;
	KeyFrame[36].movBravo_z = 20.0f;
	KeyFrame[36].giroBravo = 90;

	//Izquierda del sofá
	KeyFrame[37].movBravo_x = -55.0f;
	KeyFrame[37].movBravo_y = 0.0f;
	KeyFrame[37].movBravo_z = 10.0f;
	KeyFrame[37].giroBravo = 180;

	KeyFrame[38].movBravo_x = -5.0f;
	KeyFrame[38].movBravo_y = 0.0f;
	KeyFrame[38].movBravo_z = 10.0f;
	KeyFrame[38].giroBravo = 90;

	KeyFrame[39].movBravo_x = 0.0f;
	KeyFrame[39].movBravo_y = 5.0f;
	KeyFrame[39].movBravo_z = 0.0f;
	KeyFrame[39].giroBravo = 0;
	
	
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{	
		if (mainWindow.getRegula() == true) {
			if (mainWindow.getCamara() == true)
			{
				camera.setPosDir(glm::vec3(40.0f, 8.0f, 1.0f) + desplazamientoBravo, glm::vec3(1.0f, 0.0f, 0.0f));
			}
			if (mainWindow.getCamara() == false)
			{
				if (mainWindow.getAngulo() == true) {
					camera.setPosDir(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
					mainWindow.setAngulo(false);
				}

			}
		}
		else {
			if (mainWindow.getCamaraExtra() == true)
			{
				camera.setPosDir(glm::vec3(4.0f, 6.5f, -17.3f), glm::vec3(0.0f, 0.0f, -1.0f));
			}
			if (mainWindow.getCamaraExtra() == false)
			{

				camera.setPosDir(glm::vec3(-75.0f, 26.82f, 5.5f), glm::vec3(-1.0f, .0f, 0.0f));
			}
		}
		
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		
		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		
		inputKeyframes(mainWindow.getsKeys());
		indiceFrame = animate();

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
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x , camera.getCameraPosition().y, camera.getCameraPosition().z );

		//luz ligada a la cámara de tipo flash 
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection()); //Le manda la direccion a donde apunta nuestra camara
		//spotLights[0].setPos(); Solo recibe un vector para modoficar la posición sin moddificar el cono
		//información al shader de fuentes de iluminación
		if (mainWindow.getProyectar() == true) {
			mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, //Valores de color
				0.3f, 0.3f, //coeficiente ambiental, que tan intensa es la luz del la luz ambiental y coeficiente difuso es que tan intenso es el tono.
				0.0f, 0.0f, -1.0f); //Vector de direccion
			spotLights[2] = SpotLight(1.0f, 1.0f, 1.0f, //Aqui va el color
				1.0f, 2.0f,
				-72.0f, 28.2f, 5.3f,
				-1.0f, -0.3f, 0.0f, //Vector de dirección, 
				1.0f, 0.0f, 0.0f,
				18.0f); //Tamaño del diametro
			spotLightCount++;
		}
		else {
			mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, //Valores de color
				0.3f, 0.3f, //coeficiente ambiental, que tan intensa es la luz del la luz ambiental y coeficiente difuso es que tan intenso es el tono.
				0.0f, 0.0f, -1.0f); //Vector de direccion
			spotLights[2] = SpotLight(0.0f, 0.0f, 0.0f, //Aqui va el color
				1.0f, 2.0f,
				-72.0f, 28.2f, 5.3f,
				-1.0f, -0.3f, 0.0f, //Vector de dirección, 
				1.0f, 0.0f, 0.0f,
				18.0f); //Tamaño del diametro
			
		}
		
		unsigned int pointLightCount = 0;
		if (mainWindow.getLuces() == true) {
			mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, //Valores de color
				0.3f, 0.3f, //coeficiente ambiental, que tan intensa es la luz del la luz ambiental y coeficiente difuso es que tan intenso es el tono.
				0.0f, 0.0f, -1.0f); //Vector de direccion
			//Declaración de primer luz puntual
			pointLights[0] = PointLight(1.0f, 1.0f, 0.0f, //Valores de color
				0.5f, 1.0f, //Coeficiente ambiental y difuso
				-15.2f, 11.5f, -22.0f, //Poisicion
				0.6f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación
			pointLightCount++;

			pointLights[1] = PointLight(1.0f, 1.0f, 0.0f, //Valores de color
				0.5f, 1.0f, //Coeficiente ambiental y difuso
				7.5f, 11.5f, -22.0f, //Poisicion
				0.6f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación
			pointLightCount++;

			pointLights[2] = PointLight(1.0f, 1.0f, 0.0f, //Valores de color
				0.5f, 1.0f, //Coeficiente ambiental y difuso
				30.2f, 11.5f, -22.0f, //Poisicion
				0.6f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación
			pointLightCount++;

			pointLights[3] = PointLight(1.0f, 1.0f, 0.0f, //Valores de color
				0.5f, 1.0f, //Coeficiente ambiental y difuso
				-15.2f, 11.5f, 29.0f, //Poisicion
				0.6f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación
			pointLightCount++;

			pointLights[4] = PointLight(1.0f, 1.0f, 0.0f, //Valores de color
				0.5f, 1.0f, //Coeficiente ambiental y difuso
				7.5f, 11.5f, 29.0f, //Poisicion
				0.6f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación
			pointLightCount++;

			pointLights[5] = PointLight(1.0f, 1.0f, 0.0f, //Valores de color
				0.5f, 1.0f, //Coeficiente ambiental y difuso
				30.2f, 11.5f, 29.0f, //Poisicion
				0.6f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación
			pointLightCount++;
			//luz dulceria
			pointLights[6] = PointLight(1.0f, 1.0f, 0.0f, //Valores de color
				0.5f, 1.0f, //Coeficiente ambiental y difuso
				-69.1f, 9.0f, -1.0f, //Poisicion
				0.6f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación
			pointLightCount++;
			//sala cine
			pointLights[7] = PointLight(1.0f, 1.0f, 0.0f, //Valores de color
				0.5f, 1.0f, //Coeficiente ambiental y difuso
				-100.2f, 17.0f, -23.0f, //Poisicion
				0.6f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación
			pointLightCount++;
			pointLights[8] = PointLight(1.0f, 1.0f, 0.0f, //Valores de color
				0.5f, 1.0f, //Coeficiente ambiental y difuso
				-130.2f, 17.0f, -23.0f, //Poisicion
				0.6f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación
			pointLightCount++;

			pointLights[9] = PointLight(1.0f, 1.0f, 0.0f, //Valores de color
				0.5f, 1.0f, //Coeficiente ambiental y difuso
				-100.2f, 17.0f, 30.0f, //Poisicion
				0.6f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación
			pointLightCount++;
			pointLights[10] = PointLight(1.0f, 1.0f, 0.0f, //Valores de color
				0.5f, 1.0f, //Coeficiente ambiental y difuso
				-130.2f, 17.0f, 30.0f, //Poisicion
				0.6f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación
			pointLightCount++;

			pointLights[11] = PointLight(1.0f, 1.0f, 0.0f, //Valores de color
				0.5f, 1.0f, //Coeficiente ambiental y difuso
				-65.8f, 15.1f, 35.0f, //Poisicion
				0.6f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación
			pointLightCount++;
			pointLights[12] = PointLight(1.0f, 1.0f, 0.0f, //Valores de color
				0.5f, 1.0f, //Coeficiente ambiental y difuso
				-100.8f, 15.1f, 35.0f, //Poisicion
				0.6f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación
			pointLightCount++;

			pointLights[13] = PointLight(1.0f, 1.0f, 0.0f, //Valores de color
				0.5f, 1.0f, //Coeficiente ambiental y difuso
				-65.8f, 15.1f, 55.0f, //Poisicion
				0.6f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación
			pointLightCount++;
			pointLights[14] = PointLight(1.0f, 1.0f, 0.0f, //Valores de color
				0.5f, 1.0f, //Coeficiente ambiental y difuso
				-100.8f, 15.1f, 55.0f, //Poisicion
				0.6f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación
			pointLightCount++;

		}
		if (mainWindow.getLuces() == false) {
			mainLight = DirectionalLight(0.15f, 0.15f, 0.2f, //Valores de color
				0.3f, 0.3f, //coeficiente ambiental, que tan intensa es la luz del la luz ambiental y coeficiente difuso es que tan intenso es el tono.
				0.0f, 0.0f, -1.0f); //Vector de direccion
			//Declaración de primer luz puntual
			pointLights[0] = PointLight(0.0f, 0.0f, 0.0f, //Valores de color
				0.5f, 1.0f, //Coeficiente ambiental y difuso
				-15.2f, 11.5f, -22.0f, //Poisicion
				0.6f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación

			pointLights[1] = PointLight(0.0f, 0.0f, 0.0f, //Valores de color
				0.5f, 1.0f, //Coeficiente ambiental y difuso
				7.5f, 11.5f, -22.0f, //Poisicion
				0.6f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación

			pointLights[2] = PointLight(0.0f, 0.0f, 0.0f, //Valores de color
				0.5f, 1.0f, //Coeficiente ambiental y difuso
				30.2f, 11.5f, -22.0f, //Poisicion
				0.6f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación


			pointLights[3] = PointLight(0.0f, 0.0f, 0.0f, //Valores de color
				0.5f, 1.0f, //Coeficiente ambiental y difuso
				-15.2f, 11.5f, 29.0f, //Poisicion
				0.6f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación

			pointLights[4] = PointLight(0.0f, 0.0f, 0.0f, //Valores de color
				0.5f, 1.0f, //Coeficiente ambiental y difuso
				7.5f, 11.5f, 29.0f, //Poisicion
				0.6f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación

			pointLights[5] = PointLight(0.0f, 0.0f, 0.0f, //Valores de color
				0.5f, 1.0f, //Coeficiente ambiental y difuso
				30.2f, 11.5f, 29.0f, //Poisicion
				0.6f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación
			//luz dulceria
			pointLights[6] = PointLight(0.0f, 0.0f, 0.0f, //Valores de color
				0.5f, 1.0f, //Coeficiente ambiental y difuso
				-69.1f, 9.0f, -1.0f, //Poisicion
				0.6f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación

			//sala cine
			pointLights[7] = PointLight(0.0f, 0.0f, 0.0f, //Valores de color
				0.5f, 1.0f, //Coeficiente ambiental y difuso
				-100.2f, 17.0f, -23.0f, //Poisicion
				0.6f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación

			pointLights[8] = PointLight(0.0f, 0.0f, 0.0f, //Valores de color
				0.5f, 1.0f, //Coeficiente ambiental y difuso
				-130.2f, 17.0f, -23.0f, //Poisicion
				0.6f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación

			pointLights[9] = PointLight(0.0f, 0.0f, 0.0f, //Valores de color
				0.5f, 1.0f, //Coeficiente ambiental y difuso
				-100.2f, 17.0f, 30.0f, //Poisicion
				0.6f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación

			pointLights[10] = PointLight(0.0f, 0.0f, 0.0f, //Valores de color
				0.5f, 1.0f, //Coeficiente ambiental y difuso
				-130.2f, 17.0f, 30.0f, //Poisicion
				0.6f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación

			pointLights[11] = PointLight(0.0f, 0.0f, 0.0f, //Valores de color
				0.5f, 1.0f, //Coeficiente ambiental y difuso
				-65.8f, 15.1f, 35.0f, //Poisicion
				0.6f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación

			pointLights[12] = PointLight(0.0f, 0.0f, 0.0f, //Valores de color
				0.5f, 1.0f, //Coeficiente ambiental y difuso
				-100.8f, 15.1f, 35.0f, //Poisicion
				0.6f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación

			pointLights[13] = PointLight(0.0f, 0.0f, 0.0f, //Valores de color
				0.5f, 1.0f, //Coeficiente ambiental y difuso
				-65.8f, 15.1f, 55.0f, //Poisicion
				0.6f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación

			pointLights[14] = PointLight(0.0f, 0.0f, 0.0f, //Valores de color
				0.5f, 1.0f, //Coeficiente ambiental y difuso
				-100.8f, 15.1f, 55.0f, //Poisicion
				0.6f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación


		}
		shaderList[0].SetDirectionalLight(&mainLight);
		//shaderList[0].SetPointLights(pointLights, pointLightCount);
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

		// ********* Condicionales de sonido:
		//Dinero
		if (indiceFrame == 6) {
			if (bandera == true) {
				SoundEngine->play2D("audio/dinero.mp3");
				printf("Estoy dentro");
				bandera = false;
			}

		}
		else if (11 <= indiceFrame && indiceFrame <= 12) {
			if (bandera == true) {
				SoundEngine->play2D("audio/palomitas.mp3");
				printf("Estoy dentro");
				bandera = false;
			}
		}
		else if (indiceFrame == 14) {
			if (bandera == true) {
				SoundEngine->play2D("audio/puerta.mp3");
				printf("Estoy dentro");
				bandera = false;
			}
		}
		else if (20 <= indiceFrame && indiceFrame <= 26) {
			if (bandera == true) {
				SoundEngine->play2D("audio/opening.mp3");
				printf("Estoy dentro");
				bandera = false;
			}
		}
		else if (indiceFrame == 28) {
			if (bandera == true) {
				SoundEngine->play2D("audio/aplauso.mp3");
				printf("Estoy dentro");
				bandera = false;
			}
		}
		else if (indiceFrame == 30) {
			if (bandera == true) {
				SoundEngine->play2D("audio/puerta.mp3");
				printf("Estoy dentro");
				bandera = false;
			}
		}
		else {
			bandera = true;
		}

		//
		//SoundEngine->play2D("audio/breakout.mp3", true);


		///******************************************************** CINE ********************************
		desplazamientoBravo = glm::vec3(movBravo_x, movBravo_y, movBravo_z);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(posBravo + desplazamientoBravo));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.4f));
		model = glm::rotate(model, 0 + giroBravo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		johnny.RenderModel();

		//	***********************			Sala de proyección 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-120.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cine.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-142.5f, 12.3f, 3.85f));
		model = glm::scale(model, glm::vec3(5.0f, 9.5f, 14.3f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pantallaPrincipal.RenderModel();

		//Puerta de entrada CINE
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-71.0f, 1.8f, -18.0f)); 
		model = glm::scale(model, glm::vec3(1.8f, 2.5f, 1.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		marcoPuerta.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-70.5f, 1.8f, -22.0f)); 
		model = glm::scale(model, glm::vec3(1.5f, 1.8f, 1.5f));
		model = glm::rotate(model, 0 + giroPuerta * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		puertaD.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-70.5f, 1.8f, -14.0f)); 
		model = glm::scale(model, glm::vec3(1.5f, 1.8f, 1.5f));
		model = glm::rotate(model, 0 - giroPuerta * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		puertaI.RenderModel();
		//fin puerta

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.2f, 6.7f, 4.0f)); 
		model = glm::scale(model, glm::vec3(3.0f, 3.05f, 3.05f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dexter.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-110.2f, 3.5f, 8.0f)); 
		model = glm::scale(model, glm::vec3(3.0f, 3.05f, 3.05f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mandy.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-115.2f, 1.5f, 0.0f)); 
		model = glm::scale(model, glm::vec3(2.0f, 2.05f, 2.05f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		eduardo.RenderModel();
		//Exitintor
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-140.0f, 3.0f, 30.5f)); 
		model = glm::scale(model, glm::vec3(3.5f, 3.8f, 3.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		extintor.RenderModel();

		// ---------------- Puerta de EMERGENCIA
		/*model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-130.0f, 1.8f, 30.5f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(1.5f, 2.5f, 1.6f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca de forma correcta la posición de la puerta
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		marcoPuerta.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-134.0f, 1.8f, 30.5f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(1.5f, 1.8f, 1.5f));
		model = glm::rotate(model, 90 +giroPuertaEm  * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca de forma correcta la posición de la puerta
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		puertaD.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-126.0f, 1.8f, 30.5f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(1.5f, 1.8f, 1.5f));
		model = glm::rotate(model, 90 + giroPuertaEm  * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca de forma correcta la posición de la puerta
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		puertaI.RenderModel();
		//fin puerta emergencia */

		//Exitintor
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-140.0f, 3.0f, -23.5f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(3.5f,3.8f, 3.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		extintor.RenderModel();

		//lamparas derecha
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.2f, 17.0f, -24.5f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(4.8f, 4.8f, 4.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamparaPared.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-130.2f, 17.0f, -24.5f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(4.8f, 4.8f, 4.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamparaPared.RenderModel();

		//lamparas izquierda
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.2f, 17.0f, 31.5f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(4.8f, 4.8f, 4.8f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamparaPared.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-130.2f, 17.0f, 31.5f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(4.8f, 4.8f, 4.8f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamparaPared.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-72.0f, 26.82f, 5.5f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		proyector.RenderModel();

		// ***********************			Dulceria 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-69.2f, 0.3f, 15.0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.3f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mostrador.RenderModel();

		angulo += 0.2;
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-60.2f, -1.0f, 15.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.35f, 0.3f));
		model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca de frente al personal
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		personal.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-70.7f, 7.3f, 8.0f));
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		combo1.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-70.7f, 7.3f, 14.0f));
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		combo3.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-70.7f, 7.3f, 21.0f));
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		combo2.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-69.1f, -1.9f, 0.5f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(3.2f, 3.2f, 3.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		refrigerador.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-69.2f, 2.0f, 15.0f));
		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maquina_bebida.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-69.2f, 1.8f, 18.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vaso.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-69.2f, 1.8f, 20.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vaso.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-55.2f, 0.3f, 14.0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.3f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mostrador.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-55.2f, 1.8f, 10.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		registradora.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-55.2f, 2.8f, 5.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pantallaD.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-55.2f, 1.8f, 22.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		palomitas.RenderModel();
		
		//Maquina Palomitas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-55.2f, 1.8f, 0.5f));
		model = glm::scale(model, glm::vec3(1.15f, 1.15f, 1.15f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maqPalomitasBase.RenderModel();



		// ***************** Palomita animada ***********
		offset += 0.15;

		posXpalomita = 1.1 * cos( 3 * offset * toRadians); //Para 1ra palomita
		posZpalomita = 1.1 * cos( 3 * offset * toRadians ); //Para 2da palomita
		posYpalomita = 1.1 * sin( 3 * offset * toRadians );

		desplazamientoPalomita = glm::vec3( 0 , posYpalomita, posZpalomita);
		desplazamientoPalomita2 = glm::vec3(posXpalomita, posYpalomita, 0);
		desplazamientoPalomita3 = glm::vec3(posXpalomita, posYpalomita, posZpalomita);

		model = glm::mat4(1.0);
		model = glm::translate(model, posPalomita + desplazamientoPalomita);
		model = glm::scale(model, glm::vec3(0.1f, 0.10f, 0.10f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		palomitaAni.RenderModel();

		//Palomita1 espejo
		model = glm::mat4(1.0);
		model = glm::translate(model, posPalomita - desplazamientoPalomita);
		model = glm::scale(model, glm::vec3(0.1f, 0.10f, 0.10f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		palomitaAni.RenderModel();

		//Segunda palomita
		model = glm::mat4(1.0);
		model = glm::translate(model, posPalomita + desplazamientoPalomita2);
		model = glm::scale(model, glm::vec3(0.1f, 0.10f, 0.10f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		palomitaAni.RenderModel();

		//Segunda palomita espejeo
		model = glm::mat4(1.0);
		model = glm::translate(model, posPalomita - desplazamientoPalomita2);
		model = glm::scale(model, glm::vec3(0.1f, 0.10f, 0.10f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		palomitaAni.RenderModel();

		//Tercera palomita
		model = glm::mat4(1.0);
		model = glm::translate(model, posPalomita + desplazamientoPalomita3);
		model = glm::scale(model, glm::vec3(0.1f, 0.10f, 0.10f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		palomitaAni.RenderModel();

		//Tercera palomita espejo
		model = glm::mat4(1.0);
		model = glm::translate(model, posPalomita - desplazamientoPalomita3);
		model = glm::scale(model, glm::vec3(0.1f, 0.10f, 0.10f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		palomitaAni.RenderModel();

		// ******************* Fin palomita animada ****************

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-54.5f, 1.8f,22.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(0.04f, 0.04f, 0.04f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		coca.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-45.0f, -2.0f, 11.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(2.5f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		separadores.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-55.2f, 1.8f, 25.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		registradora.RenderModel();
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-55.2f, 2.8f, 20.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pantallaD.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-55.2f, 1.8f, 14.0f)); 
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		palomitas.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-54.4f, 1.8f, 14.2f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vaso.RenderModel();

		// *********************		zona de comida

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-31.2f, -1.8f, 19.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.06f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesa.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-33.5f, -1.8f, 21.5f));
		model = glm::scale(model, glm::vec3(0.012f, 0.013f, 0.012f));
		model = glm::rotate(model, 40 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		silla.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-28.4f, -1.8f, 17.0f));
		model = glm::scale(model, glm::vec3(0.012f, 0.013f, 0.012f));
		model = glm::rotate(model, -140 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		silla.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-26.5f, -1.8f, 4.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.06f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesa.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-28.2f, -2.0f, 7.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.06f, 4.05f));
		model = glm::rotate(model, -180 + angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		val.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-28.8f, -1.8f, 1.5f));
		model = glm::scale(model, glm::vec3(0.012f, 0.013f, 0.012f));
		model = glm::rotate(model, -40 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		silla.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-23.2f, -1.8f, 3.9f));
		model = glm::scale(model, glm::vec3(0.012f, 0.013f, 0.012f));
		model = glm::rotate(model, -190 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		silla.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-26.5f, 1.38f, 4.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, 40 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		food.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.2f, -1.8f, 13.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.06f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesa.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-22.5f, -1.8f, 15.0f));
		model = glm::scale(model, glm::vec3(0.012f, 0.013f, 0.012f));
		model = glm::rotate(model, 50 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		silla.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-17.8f, -1.8f, 10.65f));
		model = glm::scale(model, glm::vec3(0.012f, 0.013f, 0.012f));
		model = glm::rotate(model, -120 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		silla.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-19.2f, 1.35f, 12.4f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(0.04f, 0.04f, 0.04f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		coca.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-21.0f, 1.35f, 13.6f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vaso.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.2f, -2.2f, 19.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(3.5f, 4.1f, 2.5f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sofa.RenderModel();

		//Basurero
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-19.2f, -1.9f, -23.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(1.9f, 1.9f, 1.9f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		basurero.RenderModel();

		//Exitintor
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-60.0f, 5.0f, -25.4f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(3.5f, 3.8f, 3.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		extintor.RenderModel();

		// **********************			Taquilla 

		//Desk
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.8f, -2.0f, -13.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(0.17f, 0.11f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		desk.RenderModel();

		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.2f, 2.2f, -14.5f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		registradora.RenderModel();

		//Ticket
		desplazamientoTicket = glm::vec3(movTicket_x, movTicket_y, movTicket_z);

		model = glm::mat4(1.0);
		model = glm::translate(model, posTicket + desplazamientoTicket);
		model = glm::scale(model, glm::vec3(0.5f, 0.4f, 0.9f));
		model = glm::rotate(model, -80 + giroTicket * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, giroTicket * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		boleto.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f, -1.0f, -17.3f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		personal.RenderModel();
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(11.0f, 3.8f, -11.4f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		model = glm::rotate(model, 130 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		monitor.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(6.0f, 2.8f, -10.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		monitor2.RenderModel();
		
			
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.2f, 2.2f, -14.5f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		registradora.RenderModel();
		
				
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.5f, 3.7f, -11.3f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lampara.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(11.5f, -2.0f, -5.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 2.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		separadores.RenderModel();
			   	
		// lamparas izquierda		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.2f, 10.0f, 32.9f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(4.8f, 4.8f, 4.8f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamparaPared.RenderModel(); 

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(7.5f, 10.0f, 32.9f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(4.8f, 4.8f, 4.8f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamparaPared.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.2f, 10.0f, 32.9f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(4.8f, 4.8f, 4.8f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamparaPared.RenderModel();
		
		//lamparas derecha
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.2f, 10.0f, -25.9f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(4.8f, 4.8f, 4.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamparaPared.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(7.5f, 10.0f, -25.9f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(4.8f, 4.8f, 4.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamparaPared.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.2f, 10.0f, -25.9f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(4.8f, 4.8f, 4.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamparaPared.RenderModel();

		//fondo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-71.1f, 8.0f, -1.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(4.8f, 4.8f, 4.8f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca de forma correcta la posición de la puerta
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamparaPared.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.5f, 0.4f, 50.5f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bebidas.RenderModel();

		//Maquina Palomitas Vidrio
		/*model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-55.2f, 5.5f, 0.5f));
		model = glm::scale(model, glm::vec3(1.15f, 1.15f, 1.15f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);									//Si no habilitamos este blending cuando no tenemos fondo esto se verá negro
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  //Si no habilitamos este blending cuando no tenemos fondo esto se verá negro
		maqPalomitasVidrio.RenderModel();
		glDisable(GL_BLEND);*/

		
		//			Puertas, paredes y lamparas

		//Piso
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-200.5f, 0.0f, 20.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		piso.RenderModel();
		
		//Pared
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, 15.1f, -26.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(7.3f, 4.95f, 0.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca de forma correcta la posición de la pared
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pared.RenderModel();

		//Pared2 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.12f, 15.1f, 33.2f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(4.9f, 4.95f, 0.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca de forma correcta la posición de la pared
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pared.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-64.8f, 15.1f, 33.1f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(0.7f, 4.9f, 1.9f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca de forma correcta la posición de la pared
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pared.RenderModel();

		//Pared Entrada
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(59.0f, 16.5f, 3.6f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(0.55f, 4.6f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pared.RenderModel();

		//Puerta Principal
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(58.7f, -2.0f, 5.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(0.08f, 0.1f, 0.1f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca de forma correcta la posición de la puerta
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		puerta.RenderModel();

		//Techo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-41.0f, 35.4f, 2.f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(10.2f, 0.1f, 7.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca de forma correcta la posición de la pared
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		techo.RenderModel();

		//Pasillo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-144.6f, 16.5f, 45.6f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(0.55f, 4.6f, 1.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pared.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, 16.5f, 46.6f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(0.55f, 4.6f, 1.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pared.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-90.8f, 15.1f, 59.1f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(5.3f, 4.95f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca de forma correcta la posición de la pared
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pared.RenderModel();

		//Techo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-90.1f, 35.4f, 45.1f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(5.3f, 2.95f, 4.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca de forma correcta la posición de la pared
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		techo.RenderModel();

		//lamparas izquierda
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-65.8f, 15.1f, 58.86f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(4.8f, 4.8f, 4.8f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamparaPared.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.8f, 15.1f, 58.86f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(4.8f, 4.8f, 4.8f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamparaPared.RenderModel();

		//lamparas derecha
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-65.8f, 15.1f, 33.4f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(4.8f, 4.8f, 4.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamparaPared.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.8f, 15.1f, 33.4f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(4.8f, 4.8f, 4.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamparaPared.RenderModel();

		// CUADROS
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, 5.5f, -25.8f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(3.0f, 3.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca de forma correcta la posición de la pared
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuadro1.RenderModel();
		//2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, 5.5f, -25.8f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(3.0f, 3.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca de forma correcta la posición de la pared
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuadro2.RenderModel();
		//3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, 5.5f, -25.8f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(3.0f, 3.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca de forma correcta la posición de la pared
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuadro3.RenderModel();
		//4
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-25.0f, 5.5f, -25.8f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(3.0f, 3.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca de forma correcta la posición de la pared
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuadro4.RenderModel();
		//5
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-35.0f, 5.5f, -25.8f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(3.0f, 3.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca de forma correcta la posición de la pared
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuadro5.RenderModel();
		//6
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(25.0f, 5.5f, -25.8f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(3.0f, 3.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca de forma correcta la posición de la pared
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuadro6.RenderModel();
		//7
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(35.0f, 5.5f, -25.8f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(3.0f, 3.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca de forma correcta la posición de la pared
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuadro7.RenderModel();
		//8
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(45.0f, 5.5f, -25.8f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(3.0f, 3.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca de forma correcta la posición de la pared
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuadro8.RenderModel();
		//9
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(55.0f, 5.5f, -25.8f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(3.0f, 3.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca de forma correcta la posición de la pared
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuadro9.RenderModel();
		//10
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, 5.5f, 33.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(3.0f, 3.5f, 0.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca de forma correcta la posición de la pared
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuadro10.RenderModel();
		//11
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, 5.5f, 33.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(3.0f, 3.5f, 0.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca de forma correcta la posición de la pared
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuadro11.RenderModel();
		//12
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.0f, 5.5f, 33.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(3.0f, 3.5f, 0.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca de forma correcta la posición de la pared
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuadro12.RenderModel();
		//13
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(40.0f, 5.5f, 33.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(3.0f, 3.5f, 0.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca de forma correcta la posición de la pared
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuadro13.RenderModel();
		//14
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 5.5f, 33.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(3.0f, 3.5f, 0.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca de forma correcta la posición de la pared
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuadro14.RenderModel();
		//15
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 5.5f, 33.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(3.0f, 3.5f, 0.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca de forma correcta la posición de la pared
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuadro15.RenderModel();
		//16
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, 5.5f, 33.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(3.0f, 3.5f, 0.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca de forma correcta la posición de la pared
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuadro16.RenderModel();



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
			//printf("movBravo_x es: %f\n", movBravo_x);
			//printf("movBravo_y es: %f\n", movBravo_y);
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
			//printf("movBravo_x es: %f\n", movBravo_x);
			movBravo_x += 1.0f;
			printf("movBravo_x es: %f\n", movBravo_x);
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

