#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	muevex = 2.0f; //Inicializando la variable para el desplazamiento en X
	muevey = 2.0f;//Inicializando la variable para el desplazamiento en Y
	muevez = 2.0f;//Inicializando la variable para el desplazamiento en Z
	cancion = true;
	luces = true;
	camaraJ = true;
	camaraExtra = true;
	regulador = true;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Primer ventana", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}




void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	/***************DEFINIENDO CAMBIO DE CAMARA*************************/
	if (key == GLFW_KEY_UP)
	{//Camara libre
		theWindow->regulador = true;
	}
	if (key == GLFW_KEY_DOWN)
	{//Camara libre
		theWindow->regulador = false;
	}
	if (key == GLFW_KEY_O)
	{//Camara libre
		theWindow->camaraJ = true;
	}
	if (key == GLFW_KEY_I)
	{//Camara Johnny
		theWindow->camaraJ = false;
		theWindow->angulo = true;
	}
	if (key == GLFW_KEY_6)
	{//Camara taquilla
		theWindow->camaraExtra = true;
	}
	if (key == GLFW_KEY_7)
	{//Camara cine
		theWindow->camaraExtra = false;
	}
	/***************Definiendo apagado y encendido de luces******************/
	if (key == GLFW_KEY_C)
	{//Cuando se presiona X decrementa en y
		theWindow->luces = true;
	}
	if (key == GLFW_KEY_V)
	{//Cuando se presiona X decrementa en y
		theWindow->luces = false;
	}

	/******** DEFINIENDO MOVIMIENTO EN EL EJE X ***********/
	if (key == GLFW_KEY_Y)
	{//Cuando se presiona Y incrementa en y
		theWindow-> muevex += 0.1;
	}
	if (key == GLFW_KEY_U)
	{//Cuando se presiona X decrementa en y
		theWindow-> muevex -= 0.1;
	}

	/******** DEFINIENDO MOVIMIENTO EN EL EJE Y ***********/
	if (key == GLFW_KEY_H)
	{//Cuando se presiona Y incrementa en z
		theWindow->muevey += 1.0;
	}
	if (key == GLFW_KEY_J)
	{//Cuando se presiona X decrementa en z
		theWindow->muevey -= 1.0;
	}
	/******** DEFINIENDO MOVIMIENTO EN EL EJE Z ***********/
	if (key == GLFW_KEY_N)
	{//Cuando se presiona Y incrementa en x
		theWindow->muevez += 1.0;
	}
	if (key == GLFW_KEY_M)
	{//Cuando se presiona X decrementa en x
		theWindow->muevez -= 1.0;
	}

	//Cancion:
	if (key == GLFW_KEY_R)
	{//Cuando se presiona X decrementa en x
		theWindow->muevez -= 1.0;
	}
	if (key == GLFW_KEY_T)
	{//Cuando se presiona X decrementa en x
		theWindow->cancion =true;
	}
	if (key == GLFW_KEY_R)
	{//Cuando se presiona X decrementa en x
		theWindow->cancion = false;
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
