#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getZChange();
	bool getLuces() { return luces; }
	bool getProyectar() { return proyector; }
	bool getCamara() { return camaraJ; }
	bool getCamaraExtra() { return camaraExtra; }
	bool getRegula() { return regulador; }
	bool getAngulo() { return angulo; }
	void setAngulo(bool valor) { angulo = valor; }
	GLfloat getmuevex() { return muevex; }
	GLfloat getmuevey() { return muevey; } //Método GET para mueve Y
	GLfloat getmuevez() { return muevez; }//Método GET para mueve Z
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool luces;
	bool proyector;
	bool camaraJ;  // Cámara vista Johnny y cámara libre
	bool camaraExtra;  //cámara personal y vista cine 
	bool regulador; //regula que par de cámara se habilita
	bool angulo;
	GLfloat muevex; //Variable para muevex
	GLfloat muevey; //Variable para mueveY
	GLfloat muevez; //Variable para mueveZ
	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

