#define GLUT_DISABLE_ATEXIT_HACK
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <GL/glut.h>
#include "TextureManager.h"

#include <iomanip>
using std::fixed;
using std::setprecision;
using std::setw;

using namespace std;

#define RED 0
#define GREEN 0
#define BLUE 0
#define ALPHA 1

#define ECHAP 27


void init_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);


//variables para el gizmo
float delta_x = 0.0; 
float delta_y = 0.0;
float mouse_x, mouse_y;
float var_x = 0.0;
float var_z = -30.0;
float step = 0; //0.0 Posicion inicial. 1.0 Traslacion. 2.0 Primera Rotacion(en y). 3.0 Segunda Rotacion (en x) 4.0 Ultima Rotacion (en z)

// Materials

const GLfloat floorAmbient[4] = {0.4f, 0.4f, 0.4f, 1.0f};
const GLfloat floorDiffuse[4] = {0.5f, 0.5f, 0.5f, 1.0f};
const GLfloat floorSpecular[4] = {0.2f, 0.2f, 0.2f, 1.0f};
const GLfloat floorShininess = 3.0f;

const GLfloat wallAmbient[4] = {0.4f, 0.4f, 0.4f, 1.0f};
const GLfloat wallDiffuse[4] = {0.2f, 0.2f, 0.2f, 1.0f};
const GLfloat wallSpecular[4] = {0.6f, 0.6f, 0.6f, 1.0f};
const GLfloat wallShininess = 13.0f;

const GLfloat roofAmbient[4] = {0.4f, 0.4f, 0.4f, 1.0f};
const GLfloat roofDiffuse[4] = {0.5f, 0.5f, 0.5f, 1.0f};
const GLfloat roofSpecular[4] = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat roofShininess = 100.0f;

const GLfloat leavesAmbient[4] = {0.4f, 0.4f, 0.4f, 1.0f};
const GLfloat leavesDiffuse[4] = {0.3f, 0.3f, 0.3f, 1.0f};
const GLfloat leavesSpecular[4] = {0.2f, 0.2f, 0.2f, 1.0f};
const GLfloat leavesShininess = 8.0f;

const GLfloat logAmbient[4] = {0.4f, 0.4f, 0.4f};
const GLfloat logDiffuse[4] = {0.3f, 0.3f, 0.3f, 1.0f};
const GLfloat logSpecular[4] = {0.2f, 0.2f, 0.2f, 1.0f};
const GLfloat logShininess = 0.0f;


/// Textures
GLint texturePared;
GLint textureTecho;
GLint textureFloor;
GLint textureArbol;
GLint textureTronco;

//matrix modelview
GLdouble mvmatrixd[16];       // Holds the current model/view matrix

///////////////////////////////////////////////////////////////////////////////
//(1)
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		var_z += 0.5;
		glutPostRedisplay();
		break;

	case GLUT_KEY_DOWN:
		var_z -= 0.5;
		glutPostRedisplay();
		break;

	case GLUT_KEY_LEFT:
		var_x += 0.5;
		glutPostRedisplay();
		break;

	case GLUT_KEY_RIGHT:
		var_x -= 0.5;
		glutPostRedisplay();
		break;

	case GLUT_KEY_PAGE_UP:
		step++;
		glutPostRedisplay();
		break;
	case GLUT_KEY_PAGE_DOWN:
		step--;
		glutPostRedisplay();
		break;


	}
}



///////////////////////////////////////////////////////////////////////////////
//(2)
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	{
		mouse_x = x;
		mouse_y = y;
	}
}

///////////////////////////////////////////////////////////////////////////////
//(3)
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_motion(int x, int y)
{
	
	delta_x += x - mouse_x;
	delta_y += y - mouse_y;
	mouse_x = x;
	mouse_y = y;
	glutPostRedisplay();
}


//function called on each frame
GLvoid window_idle();

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);


	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("TP 3 : Transformations 3D Part 2");


	initGL();

	texturePared = TextureManager::Inst()->LoadTexture("textures/texture-pared1.jpg", GL_BGR_EXT, GL_RGB);
	textureTecho = TextureManager::Inst()->LoadTexture("textures/texture-roof1.jpeg", GL_BGR_EXT, GL_RGB);
	textureFloor = TextureManager::Inst()->LoadTexture("textures/texture-floor1.jpg", GL_BGR_EXT, GL_RGB);
	textureArbol = TextureManager::Inst()->LoadTexture("textures/texture-arbol1.jpeg", GL_BGR_EXT, GL_RGB);
	textureTronco = TextureManager::Inst()->LoadTexture("textures/texture-tronco.jpeg", GL_BGR_EXT, GL_RGB);

	std::cout << texturePared << std::endl;
	std::cout << textureTecho << std::endl;
	std::cout << textureFloor << std::endl;
	std::cout << textureArbol << std::endl;
	std::cout << textureTronco << std::endl;


	init_scene();

	
	glutDisplayFunc(&window_display);
	glutReshapeFunc(&window_reshape);
	
	
	glutMouseFunc(&callback_mouse);
	glutMotionFunc(&callback_motion);

	glutKeyboardFunc(&window_key);
	//glutKeyboardUpFunc(&window_key_up); //key release events
	glutSpecialFunc(&callback_special);
	//glutSpecialUpFunc(&callback_special_up); //key release events
	
	//function called on each frame
	glutIdleFunc(&window_idle);

	glutMainLoop();

	return 1;
}

void setlight(){
	//here you set the lights and parameters, example with one light
	float LightAmbient[] = { 0.1f, 0.1f, 0.05f, 1.0f };
	float LightEmission[] = { 1.0f, 1.0f, 0.8f, 1.0f };
	float LightDiffuse[] = { 1.0f, 1.0f, 0.8f, 1.0f };
	float LightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//float LightDirection[]={-0.5f, -0.5f, -0.5f};
	float LightDirection[]={-10.0f, 10.0f, 10.0f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, LightDirection);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void setupLights() {
  // Color values: red light.
  GLfloat Light0Amb[] = {0.75f, 0.75f, 0.75f, 1.0f};
  GLfloat Light0Dif[] = {0.9f, 0.9f, 0.9f, 1.0f};
  GLfloat Light0Spec[] = {0.4f, 0.4f, 0.4f, 1.0f};
  // Position values: puntual light.
  GLfloat Light0Pos[] = {0.0f, 20.0f, 20.0f, 1.0f};
  GLfloat direction[] = {0.0, -1.0, -1.0};

  // Light0 parameters.
  glLightfv(GL_LIGHT0, GL_AMBIENT, Light0Amb);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, Light0Dif);
  glLightfv(GL_LIGHT0, GL_SPECULAR, Light0Spec);
  glLightfv(GL_LIGHT0, GL_POSITION, Light0Pos);

  // Activate light.
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
}


GLvoid initGL()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	//for light
	
	//glShadeModel(GL_SMOOTH); // modelo de shading try GL_FLAT
	glEnable(GL_CULL_FACE); //no trata las caras escondidas
	glEnable(GL_DEPTH_TEST); // Activa el Z-Buffer
	//glDepthFunc(GL_LEQUAL); //Modo de funcionamiento del Z-Buffer
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Activa la corrección de perspectiva

	//setlight();
	setupLights();
	
	glClearColor(RED, GREEN, BLUE, ALPHA);
}


void Gizmo3D(){

	glBegin(GL_LINES);
	//glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(3.0f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	//glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 3.0f, 0.0f);
	glEnd();


	glBegin(GL_LINES);
	//glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 3.0f);
	glEnd();
	return;
}

void draw_quad(int width, int large){
    glBegin(GL_QUADS);
    glVertex2f(-width*0.5,large*0.5);
    glVertex2f(width*0.5,large*0.5);
    glVertex2f(width*0.5,-large*0.5);
    glVertex2f(-width*0.5,-large*0.5);
    glEnd();
}

void printmatrix(GLdouble thematrix[16])
{
	cout << endl;
   int i, j;  // MS-Vis C++ 6.0 sometimes chokes on var's
              //  declared in for's.

   for (i=0; i<4; ++i)     // i = row
   {
      for (j=0; j<4; ++j)  // j = column
      {
         cout << fixed << setprecision(4) << setw(7)
               << thematrix[j*4+i] << " ";
      }
      cout << endl;
   }
}

void draw_floor(int width, int large){
	glMaterialfv(GL_FRONT, GL_AMBIENT, floorAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, floorDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, floorSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, floorShininess);

    glBindTexture(GL_TEXTURE_2D, textureFloor);

    glTranslatef(0,0,0);
    //glRotatef(90,1,0,0);
    glNormal3f(0,0,1);
    glBegin(GL_QUADS);
    //glColor3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0,0);
    glVertex2f(-width*0.5,large*0.5);
    glTexCoord2f(1,0);
    glVertex2f(-width*0.5,-large*0.5);
    glTexCoord2f(1,1);
    glVertex2f(width*0.5,-large*0.5);
    glTexCoord2f(0,1);
    glVertex2f(width*0.5,large*0.5);
    glEnd();

}

void draw_home(float centerx, float centery, float width, float large){
	glMaterialfv(GL_FRONT, GL_AMBIENT, wallAmbient);
  	glMaterialfv(GL_FRONT, GL_DIFFUSE, wallDiffuse);
  	glMaterialfv(GL_FRONT, GL_SPECULAR, wallSpecular);
  	glMaterialf(GL_FRONT, GL_SHININESS, wallShininess);

  	glBindTexture(GL_TEXTURE_2D, texturePared);

	glPushMatrix(); // wall derecho
	    glRotatef(90,0,1,0);
	    glTranslatef(0.5*centerx,0.5*centery,0);
	    glNormal3f(1,0,0);
	    glBegin(GL_QUADS);
	    //glColor3f(1.0f, 0.0f, 0.0f);
	    glTexCoord2f(0,0);
	    glVertex2f(-width*0.5,width*0.5);
    	glTexCoord2f(1,0);
	    glVertex2f(-width*0.5,-width*0.5);
	    glTexCoord2f(1,1);
	    glVertex2f(width*0.5,-width*0.5);
	    glTexCoord2f(0,1);
	    glVertex2f(width*0.5,width*0.5);
		glEnd();
	glPopMatrix();
	
	glPushMatrix(); //wall izq
		glRotatef(90,0,1,0);
	    glTranslatef(0.5*centerx,0.5*centery,-large);
	    glNormal3f(-1,0,0);
		glBegin(GL_QUADS);
	    //glColor3f(1.0f, 0.0f, 0.0f);
		glTexCoord2f(0,0);	   
	    glVertex2f(-width*0.5,width*0.5);
		glTexCoord2f(0,1);	    
	    glVertex2f(width*0.5,width*0.5);
		glTexCoord2f(1,1);	    
	    glVertex2f(width*0.5,-width*0.5);
		glTexCoord2f(1,0);	   
	    glVertex2f(-width*0.5,-width*0.5);
	    glEnd();
	glPopMatrix();

	glPushMatrix();// wall fondo 
		glRotatef(90,1,0,0);
	    glTranslatef(-0.5*large,-0.5*centery,0);
	    glNormal3f(0,1,0);
		glBegin(GL_QUADS);
	    //glColor3f(1.0f, 0.0f, 0.0f);
	    glTexCoord2f(0,0);
	    glVertex2f(-large*0.5,width*0.5);
	    glTexCoord2f(1,0);
	    glVertex2f(large*0.5,width*0.5);
	    glTexCoord2f(1,1);
	    glVertex2f(large*0.5,-width*0.5);
	    glTexCoord2f(0,1);
	    glVertex2f(-large*0.5,-width*0.5);
	    glEnd();
	glPopMatrix();
	
	glPushMatrix(); //wall cercano
		glRotatef(90,1,0,0);
	    glTranslatef(-0.5*large,-0.5*centery,width);
	    glNormal3f(0,-1,0);
		glBegin(GL_QUADS);
	    //glColor3f(1.0f, 0.0f, 0.0f);
	    glTexCoord2f(0,0);
	    glVertex2f(-large*0.5,width*0.5);
	    glTexCoord2f(0,1);
	    glVertex2f(-large*0.5,-width*0.5);
	    glTexCoord2f(1,1);
	    glVertex2f(large*0.5,-width*0.5);
	    glTexCoord2f(1,0);
	    glVertex2f(large*0.5,width*0.5);
	    glEnd();
	glPopMatrix();


	/////////// techo

	float height = width;

	glPushMatrix(); //triangulo derecho
		glTranslatef(0,-0.5*width, height);
		glRotatef(90,0,-1,0);
		glBegin(GL_TRIANGLES);
	    //glColor3f(1.0f, 0.0f, 0.0f);
	    glTexCoord2f(-1,-1);
	    glVertex2f(0,width*0.5);
	    glTexCoord2f(0.5,0);
	    glVertex2f(width*0.5,0);
	    glTexCoord2f(1,-1);
	    glVertex2f(0,-width*0.5);
	    glEnd();
	glPopMatrix();

	glPushMatrix(); //triangulo izq
		glTranslatef(-large,-0.5*width, height);
		glRotatef(90,0,-1,0);
		glBegin(GL_TRIANGLES);
	    //glColor3f(1.0f, 0.0f, 0.0f);
	    glTexCoord2f(-1,-1);
	    glVertex2f(0,width*0.5);
	    glTexCoord2f(1,-1);
	    glVertex2f(0,-width*0.5);
	    glTexCoord2f(0.5,0);
	    glVertex2f(width*0.5,0);
	    glEnd();
	glPopMatrix();

	float hypotenuse = 14.14213562;

	glMaterialfv(GL_FRONT, GL_AMBIENT, roofAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, roofDiffuse);
  	glMaterialfv(GL_FRONT, GL_SPECULAR, roofSpecular);
  	glMaterialf(GL_FRONT, GL_SHININESS, roofShininess);
  	
	glBindTexture(GL_TEXTURE_2D, textureTecho);

//	cout << "---------------"  << endl;
	//cout << -large*0.5 << " " << hypotenuse*0.25 << endl;
	//cout << -large*0.5 << " " << -hypotenuse*0.25 << endl;
	//cout << large*0.5 << " " << -hypotenuse*0.25 << endl;
	//cout << large*0.5 << " " << hypotenuse*0.25 << endl;

	glPushMatrix(); //techo lado lejano
	    glTranslatef(-0.5*large,-width*0.25,height+width*0.25);
		glRotatef(45,-1,0,0);
		glBegin(GL_QUADS);
	    //glColor3f(0.752941, 0.752941, 0.752941);
	    glTexCoord2f(0,0);

	    glVertex2f(-large*0.5,hypotenuse*0.25);
	    //glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrixd);
		//printmatrix(mvmatrixd);
	    glTexCoord2f(0,1);
	    glVertex2f(-large*0.5,-hypotenuse*0.25);
	    //glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrixd);
		//printmatrix(mvmatrixd);
	    glTexCoord2f(1,1);
	    glVertex2f(large*0.5,-hypotenuse*0.25);
	    glTexCoord2f(1,0);
	    glVertex2f(large*0.5,hypotenuse*0.25);
	    glEnd();
	glPopMatrix();	

	glPushMatrix(); //techo lado cercano
	    glTranslatef(-0.5*large,-width+width*0.25,height+width*0.25);
		glRotatef(45,1,0,0);
		glBegin(GL_QUADS);
	    //glColor3f(0.752941, 0.752941, 0.752941);
	    glTexCoord2f(0,0);
	    glVertex2f(-large*0.5,hypotenuse*0.25);
	    glTexCoord2f(0,1);
	    glVertex2f(-large*0.5,-hypotenuse*0.25);
	    glTexCoord2f(1,1);
	    glVertex2f(large*0.5,-hypotenuse*0.25);
	    glTexCoord2f(1,0);
	    glVertex2f(large*0.5,hypotenuse*0.25);
	    glEnd();
	glPopMatrix();	
}

GLUquadricObj* myReusableQuadric = 0;
GLUquadricObj* myReusableQuadric2 = 0;
void draw_tree(double radiusBase, double radiusTop, double height, int slices, int stacks ){
	glMaterialfv(GL_FRONT, GL_AMBIENT, leavesAmbient);
  	glMaterialfv(GL_FRONT, GL_DIFFUSE, leavesDiffuse);
  	glMaterialfv(GL_FRONT, GL_SPECULAR, leavesSpecular);
  	glMaterialf(GL_FRONT, GL_SHININESS, leavesShininess);

	myReusableQuadric = gluNewQuadric();
	myReusableQuadric2 = gluNewQuadric();
	glTranslatef(10,10,0);
	////glColor3f(0.647059,0.164706,0.164706);
	//glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	gluQuadricTexture(myReusableQuadric,GLU_TRUE);
  	glBindTexture(GL_TEXTURE_2D, textureTronco);
	gluCylinder( myReusableQuadric, radiusBase, radiusTop, height, slices, stacks );

	float radiusTree = 5;
	glTranslatef(0,0, height+ radiusTree-1);
	//glColor3f(0,1,0);

	glMaterialfv(GL_FRONT, GL_AMBIENT, logAmbient);
  	glMaterialfv(GL_FRONT, GL_DIFFUSE, logDiffuse);
  	glMaterialfv(GL_FRONT, GL_SPECULAR, logSpecular);
  	glMaterialf(GL_FRONT, GL_SHININESS, logShininess);

	gluQuadricTexture(myReusableQuadric2,GLU_TRUE);
	glBindTexture(GL_TEXTURE_2D, textureArbol);

	gluSphere(myReusableQuadric2,radiusTree, 20, 50);
}

void draw_scene(){
    draw_floor(40,40);
    draw_home(-10,-10,10,15);
    draw_tree(2,2,7,50,50);
}


GLvoid window_display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	gluPerspective(100.0f, 1.0f, 0.01f, 50.0f);


	glTranslatef(var_x, 0.0, var_z);
	glRotatef(delta_x, 0.0, 1.0, 0.0);
	glRotatef(delta_y, 1.0, 0.0, 0.0);


	//Gizmo3D();
	glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrixd);
    draw_scene();


	glutSwapBuffers();

	glFlush();
}

GLvoid window_reshape(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
}



void init_scene()
{

}

GLvoid window_key(unsigned char key, int x, int y)
{
	switch (key) {
	case ECHAP:
		exit(1);
		break;
	default:
		printf("La touche %d non active.\n", key);
		break;
	}
}


//function called on each frame
GLvoid window_idle()
{
	glutPostRedisplay();
}





