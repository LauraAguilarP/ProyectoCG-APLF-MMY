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
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 12;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void) //tecla L
{

	printf("frameindex %d\n", FrameIndex);


	KeyFrame[FrameIndex].movBravo_x = movBravo_x;
	KeyFrame[FrameIndex].movBravo_y = movBravo_y;
	KeyFrame[FrameIndex].movBravo_z = movBravo_z;
	KeyFrame[FrameIndex].giroBravo;
	//no volatil, agregar una forma de escribir a un archivo para guardar los frames
	FrameIndex++;
}

void resetElements(void) //Tecla 0
{

	movBravo_x = KeyFrame[0].movBravo_x;
	movBravo_y = KeyFrame[0].movBravo_y;
	movBravo_z = KeyFrame[0].movBravo_z;
	giroBravo = KeyFrame[0].giroBravo;
}

void interpolation(void)
{
	KeyFrame[playIndex].movBravo_xInc = (KeyFrame[playIndex + 1].movBravo_x - KeyFrame[playIndex].movBravo_x) / i_max_steps;
	KeyFrame[playIndex].movBravo_yInc = (KeyFrame[playIndex + 1].movBravo_y - KeyFrame[playIndex].movBravo_y) / i_max_steps;
	KeyFrame[playIndex].movBravo_zInc = (KeyFrame[playIndex + 1].movBravo_z - KeyFrame[playIndex].movBravo_z) / i_max_steps;
	KeyFrame[playIndex].giroBravoInc = (KeyFrame[playIndex + 1].giroBravo - KeyFrame[playIndex].giroBravo) / i_max_steps;

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
			movBravo_x += KeyFrame[playIndex].movBravo_xInc;
			movBravo_y += KeyFrame[playIndex].movBravo_yInc;
			movBravo_z += KeyFrame[playIndex].movBravo_zInc;
			giroBravo += KeyFrame[playIndex].giroBravoInc;
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


	//====================================
	
	//Comida = Texture("Textures/comida.tga");
	//Comida.LoadTextureA();
	//Bravo = Texture("Textures/bravo.tga");
	//Bravo.LoadTextureA();
	Madera = Texture("Textures/madera.tga");
	Madera.LoadTextureA();
	//Metal = Texture("Textures/metal.tga");
	//Metal.LoadTextureA();
	//Metales = Texture("Textures/metales.tga");
	//Metales.LoadTextureA();
	//Mosaicos = Texture("Textures/mosaicos.tga");
	//Mosaicos.LoadTextureA();
	//ReflejoLuces = Texture("Textures/reflejoLuces.tga");
	//ReflejoLuces.LoadTextureA();
	//PielesTelas = Texture("Textures/TexturaPieles.tga");
	//PielesTelas.LoadTextureA();
	//UtileriaExtra = Texture("Textures/TexturasVarias.tga");
	//UtileriaExtra.LoadTextureA();
	//Refri = Texture("Textures/refri.tga");
	//Refri.LoadTextureA();
	//popcorn = Texture("Textures/popcorn.tga");
	//popcorn.LoadTextureA();

	Llanta_M = Model();
	Llanta_M.LoadModel("Models/k_rueda.3ds");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	
	// *********** *********************************** C I N E ***********************************
	//Personajes
	johnny = Model();
	johnny.LoadModel("Models/johnny.obj");
	val = Model();
	val.LoadModel("Models/val.obj");
	dexter = Model();
	dexter.LoadModel("Models/dexter.fbx");
	mandy = Model();
	mandy.LoadModel("Models/mandy.obj");
	eduardo = Model();
	eduardo.LoadModel("Models/eduardo.fbx");
	//Muebles 
	cine = Model();
	cine.LoadModel("Models/sala.obj");
	silla = Model();
	silla.LoadModel("Models/silla.fbx"); //Se actualizó por un objeto texturizado
	mesa = Model();
	mesa.LoadModel("Models/mesa.obj");
	mostrador = Model();
	mostrador.LoadModel("Models/mostrador.obj");
	puerta = Model();
	puerta.LoadModel("Models/puerta1.obj"); //Cuarto tiene un bug
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
	
	//otros elementos
	basurero = Model();
	basurero.LoadModel("Models/basura.obj");
	
	piso = Model();
	piso.LoadModel("Models/piso.obj");

	pared = Model();
	pared.LoadModel("Models/pared.obj");
	techo = Model();
	techo.LoadModel("Models/techo.obj");
	lamparaPared = Model();
	lamparaPared.LoadModel("Models/lamparaPared.obj");

	//bocina = Model();
	//bocina.LoadModel("Models/Speaker.obj");
	
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
	marcoPuerta.LoadModel("Models/marcoPuerta.obj");
	puertaI = Model();
	puertaI.LoadModel("Models/puertaI.obj");
	puertaD = Model();
	puertaD.LoadModel("Models/puertaD.obj");
	

	
	
	//botellaVidrio = Model();
	//botellaVidrio.LoadModel("Models/botella.fbx");
	//cuchara = Model();
	//cuchara.LoadModel("Models/cuchara.obj");
	//escalera = Model();
	//escalera.LoadModel("Models/scala.obj");

	cuchara = Model();
	cuchara.LoadModel("Models/cuchara.obj");
	extintor = Model();
	extintor.LoadModel("Models/existor.obj");


	//Personajes
	personal = Model();
	personal.LoadModel("Models/personaje.obj");
	//lamparaPared = Model();
	//lamparaPared.LoadModel("Models/lampara.obj");
	
	//planta = Model();
	//planta.LoadModel("Models/flower.fbx");
	
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

	glm::vec3 posBravo = glm::vec3(40.0f, -2.0f, 0.0f);
	glm::vec3 desplazamientoBravo = glm::vec3(0.0f, 0.0f, 0.0f);

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
	pointLights[0] = PointLight(1.0f, 1.0f, 0.0f, //Valores de color
		0.0f, 1.0f, //Coeficiente ambiental y difuso
		-15.2f, 11.5f, -22.0f, //Poisicion
		0.2f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación
	pointLightCount++;

	pointLights[1] = PointLight(1.0f, 1.0f, 0.0f, //Valores de color
		0.0f, 1.0f, //Coeficiente ambiental y difuso
		7.5f, 11.5f, -22.0f, //Poisicion
		0.6f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación
	pointLightCount++;

	pointLights[2] = PointLight(1.0f, 1.0f, 0.0f, //Valores de color
		0.0f, 1.0f, //Coeficiente ambiental y difuso
		30.2f, 11.5f, -22.0f, //Poisicion
		0.6f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación
	pointLightCount++;

	pointLights[3] = PointLight(1.0f, 1.0f, 0.0f, //Valores de color
		0.0f, 1.0f, //Coeficiente ambiental y difuso
		-15.2f, 11.5f, 31.0f, //Poisicion
		0.6f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación
	pointLightCount++;

	pointLights[4] = PointLight(1.0f, 1.0f, 0.0f, //Valores de color
		0.0f, 1.0f, //Coeficiente ambiental y difuso
		7.5f, 11.5f, 30.0f, //Poisicion
		0.6f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación
	pointLightCount++;

	pointLights[5] = PointLight(1.0f, 1.0f, 0.0f, //Valores de color
		0.0f, 1.0f, //Coeficiente ambiental y difuso
		30.2f, 11.5f, 30.0f, //Poisicion
		0.6f, 0.2f, 0.2f); //Valores de una ecuación de segundo grado que sirven para una atenuación
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
		30.2f, 11.5f, 30.0f,
		0.0f, -2.0f, -2.0f,
		0.3f, 0.3f, 0.3f,
		15.0f);
	spotLightCount++;

	spotLights[2] = SpotLight(1.0f, 1.0f, 1.0f, //Aqui va el color
		1.0f, 2.0f,
		-72.0f, 28.2f, 5.3f, 
		-1.0f, -0.3f, 0.0f, //Vector de dirección, 
		1.0f, 0.0f, 0.0f,
		18.0f); //Tamaño del diametro
	spotLightCount++;



	/*/luz de faro
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
		-74.0f, 3.0f, -20.0f, //Vector de posición, aproximadamente donde da origen la luz, este lo coloqué cerca del faro derecho
		0.0f, -1.0f, 0.0f, //Vector de dirección, en este caso un vector unitario que apunta a donde ve el auto
		1.0f, 0.0f, 0.0f,
		15.0f); //Tamaño del diametro
	spotLightCount++;

	spotLights[3] = SpotLight(0.0f, 1.0f, 0.0f, //Aqui va el color
		1.0f, 2.0f,
		-70.0f, 3.0f, -20.0f, //Vector de posición, aproximadamente donde da origen la luz, este lo coloqué cerca del faro derecho
		0.0f, -1.0f, 0.0f, //Vector de dirección, en este caso un vector unitario que apunta a donde ve el auto
		1.0f, 0.0f, 0.0f,
		15.0f); //Tamaño del diametro
	spotLightCount++; */

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 300.0f);
	
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
	KeyFrame[2].giroBravo = 0; //Bien


	KeyFrame[3].movBravo_x = -21.0f;
	KeyFrame[3].movBravo_y = 0.0f;
	KeyFrame[3].movBravo_z = -3.0f;
	KeyFrame[3].giroBravo = 0;

	/*	KeyFrame[4].movBravo_x = 3.0f;
		KeyFrame[4].movBravo_y = -2.0f;
		KeyFrame[4].giroBravo = 45.0f*/;

		KeyFrame[4].movBravo_x = -40.0f;
		KeyFrame[4].movBravo_y = 0.0f;
		KeyFrame[4].movBravo_z = -3.0f;
		KeyFrame[4].giroBravo = 0.0f;

		KeyFrame[5].movBravo_x = -40.0f;
		KeyFrame[5].movBravo_y = 0.0f;
		KeyFrame[5].movBravo_z = -7.0f;
		KeyFrame[5].giroBravo = 0.0f;

		KeyFrame[6].movBravo_x = -35.0f;
		KeyFrame[6].movBravo_y = 0.0f;
		KeyFrame[6].movBravo_z = -7.0f;
		KeyFrame[6].giroBravo = 0.0f;

		KeyFrame[7].movBravo_x = -75.0f;
		KeyFrame[7].movBravo_y = 0.0f;
		KeyFrame[7].movBravo_z = -7.0f;
		KeyFrame[7].giroBravo = 0.0f;

		KeyFrame[8].movBravo_x = -75.0f;
		KeyFrame[8].movBravo_y = 0.0f;
		KeyFrame[8].movBravo_z = 4.0f;
		KeyFrame[8].giroBravo = 0.0f;

		KeyFrame[9].movBravo_x = -83.0f;
		KeyFrame[9].movBravo_y = 0.0f;
		KeyFrame[9].movBravo_z = 4.0f;
		KeyFrame[9].giroBravo = 0.0f;

		KeyFrame[10].movBravo_x = -83.0f;
		KeyFrame[10].movBravo_y = 0.0f;
		KeyFrame[10].movBravo_z = 20.0f;
		KeyFrame[10].giroBravo = 0.0f;

		KeyFrame[11].movBravo_x = 0.0f;
		KeyFrame[11].movBravo_y = 5.0f;
		KeyFrame[11].movBravo_z = 0.0f;
		KeyFrame[11].giroBravo = 0;

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


		///******************************************************** CINE ********************************
		desplazamientoBravo = glm::vec3(movBravo_x, movBravo_y, movBravo_z);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(posBravo+desplazamientoBravo));
		model = glm::scale(model, glm::vec3(5.5f, 5.5f, 5.4f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		johnny.RenderModel();
		
		//	***********************			Sala de proyección 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-120.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cine.RenderModel();
		
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-71.0f, -1.4f, -17.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(0.001f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca de forma correcta la posición de la puerta
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		puerta.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.2f, 6.7f, 4.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(3.0f, 3.05f, 3.05f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca de forma correcta la posición de la puerta
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dexter.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-110.2f, 3.5f, 8.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(3.0f, 3.05f, 3.05f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca de forma correcta la posición de la puerta
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mandy.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-115.2f, 1.5f, 0.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(2.0f, 2.05f, 2.05f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca de forma correcta la posición de la puerta
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		eduardo.RenderModel();
		//Exitintor
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-140.0f, 3.0f, 30.5f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(3.5f, 3.8f, 3.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		extintor.RenderModel();

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

		model = glm::mat4(1.0);

		model = glm::translate(model, glm::vec3(-60.2f, -1.0f, 15.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.35f, 0.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca de frente al personal
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		personal.RenderModel();

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
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
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

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f, -1.0f, -17.3f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
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

		
		//Puerta
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(15.5f, 0.5f, 55.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca de forma correcta la posición de la puerta
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		puerta.RenderModel();

		
		
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
		model = glm::translate(model, glm::vec3(-15.0f, 15.1f, 33.2f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(7.3f, 4.95f, 0.5f));
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

		//Cuadros
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

		//Bocina
		/*model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(25.5f, 0.5f, 15.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bocina.RenderModel();*/
		//BotellaVidrio Omitida de momento por un bug
		/*model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-200.5f, 0.0f, -50.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		botellaVidrio.RenderModel();
		//Cuchara
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.5f, 0.0f, 0.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(2.0f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuchara.RenderModel();*/
		//Escaleras
		/*model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-150.5f, 0.0f, 50.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(1.0f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		escalera.RenderModel();
		//Extintor
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-120.5f, 0.0f, 0.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(1.0f, 1.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		extintor.RenderModel();
		

		//Tarja
		/*model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.5f, 0.0f, 20.0f)); //mainWindow.getMuevex permite mover el objeto en X y getMueveZ en el eje Z
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Sink.RenderModel();*/

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