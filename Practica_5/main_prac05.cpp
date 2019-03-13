/*---------------------------------------------------------*/
/* ----------------   Práctica 5 --------------------------*/
/*-----------------    2019-2   ---------------------------*/
/*------------- Computación gráfica e interacción humano computadora ---------------*/

/*-------------Juan Carlos Nolasco     Grupo4 -------------- */
/*---------------------Version visual 2017------------------------*/
/*---------Instrucciones-------------------------------------------*/
/*--------Use las teclas para mover el modelo----------------------*/
/*--------W acercar   S  alejar------------------------------------*/
/*--------Z Bajar   E  subir------------------------------------*/
/*--------A izquierda   D derecha------------------------------------*/
/*--------flechas para rotar-----arriba abajo y a los lados--------*/
/*---------Para mover las articulaciones:----------------------------------------------*/
/*----------X rota hombro------F antebrazo--------U muñeca----------------------------*/
/*----------I + shift pulgar --- O + shift indice --- P+ shift medio --- J + shift anular---- K + shift meñique------------*/
/*---------al mover las articulaciones las teclas + shift tambien las mueven en sentido contrario o de regreso-------------*/
/*---------en el caso de los dedos se hace de forma contraria, debes apretar la tecla + shift para moverlos hacia adentro -----------*/
/*---------y la tecla sola para regresarlos a su posicion----------------------------------------*/

#include <glew.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_m.h>

#include <iostream>

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;

void myData(void);
void display(void);
void getResolution(void);

//For Keyboard
float	movX = 0.0f,
movY = 0.0f,
movZ = -5.0f,
rotX = 0.0f,
rotY = 0.0f;

//brazo
float angHom = 0.0f,
angCo = 0.0f,
angMu = 0.0f,
ang1 = 0.0f,
ang2 = 0.0f,
ang3 = 0.0f,
ang4 = 0.0f,
ang5 = 0.0f;

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData()
{
	GLfloat vertices[] = {
		//Position				//Color
		-0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V0 - Frontal
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V1
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V5
		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V4

		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V2 - Trasera
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 0.0f,	//V3
		-0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V7
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V6

		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//V4 - Izq
		-0.5f, 0.5f, -0.5f,		0.0f, 0.0f, 1.0f,	//V7
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	//V3
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//V0

		0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//V5 - Der
		0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//V1
		0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	//V2
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	//V6

		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	//V4 - Sup
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	//V5
		0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//V6
		-0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//V7

		-0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	//V0 - Inf
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	//V3
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	//V2
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	//V1
	};

	unsigned int indices[] =	//I am not using index for this session
	{
		0
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//Para trabajar con indices (Element Buffer Object)
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

void display(void)
{
	//Shader myShader("shaders/shader.vs", "shaders/shader.fs");
	Shader projectionShader("shaders/shader_projection.vs", "shaders/shader_projection.fs");

	projectionShader.use();

	// create transformations and Projection
	glm::mat4 modelTemp = glm::mat4(1.0f); //Temp
	glm::mat4 modelTemp2 = glm::mat4(1.0f); //Temp
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection


	//Use "projection" in order to change how we see the information
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	//Use "view" in order to affect all models
	view = glm::translate(view, glm::vec3(movX, movY, movZ));
	view = glm::rotate(view, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::rotate(view, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
	// pass them to the shaders
	projectionShader.setMat4("model", model);
	projectionShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	projectionShader.setMat4("projection", projection);

	glBindVertexArray(VAO);
	model = glm::translate(model, glm::vec3(0, 0, 0));
	model = glm::scale(model, glm::vec3(1.5, 2.5, 1));//escalas
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); //cabeza

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(0, -1.5, 0));
	model = glm::scale(model, glm::vec3(0.5, 0.5, 1));//escalas
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 0.7f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); //cuello

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(0, -4.25, 0));
	model = glm::scale(model, glm::vec3(4, 5, 1));//escalas
	projectionShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);//torso

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(0, -7.75, 0));
	model = glm::scale(model, glm::vec3(4, 2, 1));//escalas
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);//cinturon

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-1.5, -10.25, 0));
	model = glm::scale(model, glm::vec3(1, 3, 1));//escalas
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);//pierna1

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(1.5, -10.25, 0));
	model = glm::scale(model, glm::vec3(1, 3, 1));//escalas
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);//pierna2

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-3, -12.25, 0));
	model = glm::scale(model, glm::vec3(4, 1, 1));//escalas
	projectionShader.setVec3("aColor", glm::vec3(0.5f, 0.26f, 0.0f));
	projectionShader.setMat4("model", model);//pie1
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(3, -12.25, 0));
	model = glm::scale(model, glm::vec3(4, 1, 1));//escalas
	projectionShader.setVec3("aColor", glm::vec3(0.5f, 0.26f, 0.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); //pie2

	//hombro
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(2.5, -2.25, 0));
	projectionShader.setVec3("aColor", glm::vec3(1, 1, 0));
	projectionShader.setMat4("model", model);

	glDrawArrays(GL_QUADS, 0, 24);
	model = glm::rotate(model, glm::radians(angHom), glm::vec3(0, 0, 1));

	/*	projectionShader.setVec3("aColor", glm::vec3(1, 0, 0));
model = glm::translate(model = glm::mat4(1.0f), glm::vec3(8, 0, 0));
modelTemp = model;//TEMPORAL
model = glm::scale(model, glm::vec3(5, 5, 5));//escalas
projectionShader.setMat4("model", model);
glDrawArrays(GL_QUADS, 0, 24);//ROJO
//USANDOLA MATRIZ TEMPORAL PARA TRASLADAR EL CUBO AZUL A ´PARTIR DEL ROJO
model = modelTemp; //A PARTIR DE AQUI se restaura la inf. para que no afecte la escala
model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-17, 0, 0));
model = glm::scale(model, glm::vec3(8, 8, 8));//escalas
projectionShader.setVec3("aColor", glm::vec3(0, 1, 1));
projectionShader.setMat4("model", model);
glDrawArrays(GL_QUADS, 0, 24);//DIBUJANDO AZUL	*/

//bicep
	model = glm::translate(model, glm::vec3(1.5, 0, 0));
	modelTemp = model;//TEMPORAL
	model = glm::scale(model, glm::vec3(2, 1, 1));//escalas
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0, 0, 1));
	glDrawArrays(GL_QUADS, 0, 24);//ROJO
	model = modelTemp; //A PARTIR DE AQUI se restaura la inf. para que no afecte la escala

  //antebrazo
							//creando pivote
	model = glm::translate(model, glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(angCo), glm::vec3(0, 1, 0));

	model = glm::translate(model, glm::vec3(1.5, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(3, 1, 1));//escalas
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0, 0));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;

	//palma
	model = glm::translate(model, glm::vec3(1.5, 0, 0));
	model = glm::rotate(model, glm::radians(angMu), glm::vec3(0, 1, 0));

	model = glm::translate(model, glm::vec3(0.5, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(1, 1, .5));//escalas
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0, 1, 0));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;
	modelTemp2 = model;

	//pulgar
	model = glm::translate(model, glm::vec3(-0.35, 0.5, 0));
	model = glm::rotate(model, glm::radians(ang1), glm::vec3(1, 0, 0));

	model = glm::translate(model, glm::vec3(0, 0.15, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.3, .5));//escalas
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.8f, 1.0f, 9.0f));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;

	//pulg2
	model = glm::translate(model, glm::vec3(0.0, 0.15, 0));
	model = glm::rotate(model, glm::radians(ang1*0.7f), glm::vec3(1, 0, 0));
	model = glm::translate(model, glm::vec3(0, 0.15, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.3, 0.5));//escalas
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0, 0));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp2;

	//indice

	model = glm::translate(model, glm::vec3(0.5, 0.38, 0));
	model = glm::rotate(model, glm::radians(ang2), glm::vec3(0, -1, 0));
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, 0.3));//escalas
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0.4, 0));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;
	//inice2
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	model = glm::rotate(model, glm::radians(ang2*0.7f), glm::vec3(0, -1, 0));
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, 0.3));//escalas
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0.7, .2));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;
	//indic3
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	model = glm::rotate(model, glm::radians(ang2*0.7f), glm::vec3(0, -1, 0));
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, 0.3));//escalas
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0.2, 0));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp2;

	//medio
	model = glm::translate(model, glm::vec3(0.5, 0.125, 0));
	model = glm::rotate(model, glm::radians(ang3), glm::vec3(0, -1, 0));
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.4, 0.25, 0.3));//escalas
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, .5, 1));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;
	//med2
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	model = glm::rotate(model, glm::radians(ang3*0.7f), glm::vec3(0, -1, 0));
	model = glm::translate(model, glm::vec3(0.25, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.4, 0.25, 0.3));//escalas
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0, 0.9, 1));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;
	//med3
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	model = glm::rotate(model, glm::radians(ang3*0.7f), glm::vec3(0, -1, 0));
	model = glm::translate(model, glm::vec3(0.25, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.4, 0.25, 0.3));//escalas
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0.7, 0.3));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp2;

	//anular
	model = glm::translate(model, glm::vec3(0.5, -0.125, 0));
	model = glm::rotate(model, glm::radians(ang4), glm::vec3(0, -1, 0));
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, 0.3));//escalas
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0, .5, 1));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;
	//an2
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	model = glm::rotate(model, glm::radians(ang4*0.7f), glm::vec3(0, -1, 0));
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, 0.3));//escalas
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0.9, .5));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;
	//anu3
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	model = glm::rotate(model, glm::radians(ang4*0.7f), glm::vec3(0, -1, 0));
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, 0.3));//escalas
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(.8, 0.7, 1));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp2;

	//meñique
	model = glm::translate(model, glm::vec3(0.5, -0.375, 0));
	model = glm::rotate(model, glm::radians(ang5), glm::vec3(0, -1, 0));
	model = glm::translate(model, glm::vec3(0.1, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.2, 0.25, 0.3));//escalas
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(.4, .5, .7));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;
	//meñ2
	model = glm::translate(model, glm::vec3(0.1, 0, 0));
	model = glm::rotate(model, glm::radians(ang5*0.7f), glm::vec3(0, -1, 0));
	model = glm::translate(model, glm::vec3(0.1, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.2, 0.25, 0.3));//escalas
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(2, 0.9, .4));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;
	//meñ3
	model = glm::translate(model, glm::vec3(0.1, 0, 0));
	model = glm::rotate(model, glm::radians(ang5*0.7f), glm::vec3(0, -1, 0));
	model = glm::translate(model, glm::vec3(0.1, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.2, 0.25, 0.3));//escalas
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.8, 0.2, .6));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;
	//---------------------------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------------------
	//hombro 2
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-2.5, -2.25, 0));
	projectionShader.setVec3("aColor", glm::vec3(1, 1, 0));
	projectionShader.setMat4("model", model);

	glDrawArrays(GL_QUADS, 0, 24);
	model = glm::rotate(model, glm::radians(-angHom), glm::vec3(0, 0, 1));

	//bicep2
	model = glm::translate(model, glm::vec3(-1.5, 0, 0));
	modelTemp = model;//TEMPORAL
	model = glm::scale(model, glm::vec3(2, 1, 1));//escalas
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0, 0, 1));
	glDrawArrays(GL_QUADS, 0, 24);//ROJO
	model = modelTemp; //A PARTIR DE AQUI se restaura la inf. para que no afecte la escala

  //antebrazo2
							//creando pivote
	model = glm::translate(model, glm::vec3(-1, 0, 0));
	model = glm::rotate(model, glm::radians(-angCo), glm::vec3(0, 1, 0));

	model = glm::translate(model, glm::vec3(-1.5, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(3, 1, 1));//escalas
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0, 0));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;

	//palma
	model = glm::translate(model, glm::vec3(-1.5, 0, 0));
	model = glm::rotate(model, glm::radians(-angMu), glm::vec3(0, 1, 0));

	model = glm::translate(model, glm::vec3(-0.5, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(1, 1, .5));//escalas
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0, 1, 0));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;
	modelTemp2 = model;

	//Dedo1
	model = glm::translate(model, glm::vec3(-0.35, 0.5, 0));
	model = glm::rotate(model, glm::radians(ang1), glm::vec3(1, 0, 0));

	model = glm::translate(model, glm::vec3(0, 0.15, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.3, .5));//escalas
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 1.0f));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;

	//dedo2
	model = glm::translate(model, glm::vec3(0.0, 0.15, 0));
	model = glm::rotate(model, glm::radians(ang1*0.7f), glm::vec3(1, 0, 0));
	model = glm::translate(model, glm::vec3(0, 0.15, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.3, 0.5));//escalas
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, .4, .9));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp2;

	//dedo2.2

	model = glm::translate(model, glm::vec3(-0.5, 0.38, 0));
	model = glm::rotate(model, glm::radians(-ang2), glm::vec3(0, -1, 0));
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, 0.3));//escalas
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0, 1));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;
	//dedo2.3
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	model = glm::rotate(model, glm::radians(-ang2 * 0.7f), glm::vec3(0, -1, 0));
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, 0.3));//escalas
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0, .2));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;
	//dedo3
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	model = glm::rotate(model, glm::radians(-ang2 * 0.7f), glm::vec3(0, -1, 0));
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, 0.3));//escalas
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0, 0));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp2;

	//dedo4
	model = glm::translate(model, glm::vec3(-0.5, 0.125, 0));
	model = glm::rotate(model, glm::radians(-ang3), glm::vec3(0, -1, 0));
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.4, 0.25, 0.3));//escalas
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0, 1));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;
	//dedo3.2
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	model = glm::rotate(model, glm::radians(-ang3 * 0.7f), glm::vec3(0, -1, 0));
	model = glm::translate(model, glm::vec3(-0.25, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.4, 0.25, 0.3));//escalas
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0, 1));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;
	//dedo3.3
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	model = glm::rotate(model, glm::radians(-ang3 * 0.7f), glm::vec3(0, -1, 0));
	model = glm::translate(model, glm::vec3(-0.25, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.4, 0.25, 0.3));//escalas
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0.7, 0.3));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp2;

	//dedo4
	model = glm::translate(model, glm::vec3(-0.5, -0.125, 0));
	model = glm::rotate(model, glm::radians(-ang4), glm::vec3(0, -1, 0));
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, 0.3));//escalas
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0, .5, 1));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;
	//dedo4.2
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	model = glm::rotate(model, glm::radians(-ang4 * 0.7f), glm::vec3(0, -1, 0));
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, 0.3));//escalas
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0.9, .5));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;
	//dedo4.3
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	model = glm::rotate(model, glm::radians(-ang4 * 0.7f), glm::vec3(0, -1, 0));
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, 0.3));//escalas
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(.8, 1, 1));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp2;

	//dedo5
	model = glm::translate(model, glm::vec3(-0.5, -0.375, 0));
	model = glm::rotate(model, glm::radians(-ang5), glm::vec3(0, -1, 0));
	model = glm::translate(model, glm::vec3(-0.1, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.2, 0.25, 0.3));//escalas
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(.4, .5, .7));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;
	//dedo5.2
	model = glm::translate(model, glm::vec3(-0.1, 0, 0));
	model = glm::rotate(model, glm::radians(-ang5 * 0.7f), glm::vec3(0, -1, 0));
	model = glm::translate(model, glm::vec3(-0.1, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.2, 0.25, 0.3));//escalas
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(2, 0.9, .4));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;
	//dedo5.3
	model = glm::translate(model, glm::vec3(-0.1, 0, 0));
	model = glm::rotate(model, glm::radians(-ang5 * 0.7f), glm::vec3(0, -1, 0));
	model = glm::translate(model, glm::vec3(-0.1, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.2, 0.25, 0.3));//escalas
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.8, 0.2, .6));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;


	glBindVertexArray(0);

}

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 5", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, resize);

	glewInit();


	//Mis funciones
	//Datos a utilizar
	myData();
	glEnable(GL_DEPTH_TEST);

	// render loop
	// While the windows is not closed
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		my_input(window);

		// render
		// Backgound color
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Mi función de dibujo
		display();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movX += 0.05f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX -= 0.05f;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		movY += 0.05f;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		movY -= 0.05f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movZ -= 0.05f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movZ += 0.05f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		rotX += 0.6f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		rotX -= 0.6f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rotY -= 0.6f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rotY += 0.6f;
	if (glfwGetKey(window, GLFW_KEY_N)) {
		if (angHom < 90) {
			angHom += 0.2f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_M)) {
		if (angHom > -90) {
			angHom -= 0.2f;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_B)) {
		if (angCo < 0) {
			angCo += 0.8f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_V)) {
		if (angCo > -140) {
			angCo -= 0.8f;
		}
	}


	if (glfwGetKey(window, GLFW_KEY_L)) {
		if (angMu < 20) {
			angMu += 0.8f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_K)) {
		if (angMu > -50) {
			angMu -= 0.8f;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_J)) {
		if (ang1 < 80) {
			ang1 += 0.8f;
		}
		if (ang2 < 80) {
			ang2 += 0.8f;
		}
		if (ang3 < 80) {
			ang3 += 0.8f;
		}
		if (ang4 < 80) {
			ang4 += 0.8f;
		}
		if (ang5 < 80) {
			ang5 += 0.8f;
		}

	}
	if (glfwGetKey(window, GLFW_KEY_H)) {
		if (ang1 > 0) {
			ang1 -= 0.8f;
		}
		if (ang2 > 0) {
			ang2 -= 0.8f;
		}
		if (ang3 > 0) {
			ang3 -= 0.8f;
		}
		if (ang4 > 0) {
			ang4 -= 0.8f;
		}
		if (ang5 > 0) {
			ang5 -= 0.8f;
		}
	}




}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
}
