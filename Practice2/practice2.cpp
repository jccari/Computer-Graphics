#define GLUT_DISABLE_ATEXIT_HACK
//#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <GL/glut.h>
using namespace std;

#define RED 0
#define GREEN 0
#define BLUE 0
#define ALPHA 1

#define ECHAP 27
void init_scene();
void render_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);

//function called on each frame
GLvoid window_idle();


//practice
float angle = 0;
float x_position = 0;
float direction = 1;
void translate_rotate(float angleVar, int desplace){
	angle += angleVar;

	glRotatef(angle,0,0,1);
	glTranslatef(desplace,0,0);

	glutSolidTeapot(1);
}

void translate_rotate_idavuelta(float angleVar, float x_ini, float x_fin){
	if( abs(x_position) > 8 )
		direction = -1*direction;

	angle += angleVar*100;
	x_position -= direction*0.1;

	//cout << x_position << endl;

	glTranslatef(x_position,0,0);
	glRotatef(angle,0,0,1);
	glTranslatef(x_fin,0,0);

	glutSolidTeapot(1);
}

float positionx = 8.0f;
float positiony = 0.0f;
float positionz = 0.0f;
float angulo = 0.0f;
bool goback = true;
float tor_angle = 0.0f;
float pi = 3.14159f;
float dir = 1;

void exercise3(){
    if( abs(positionx) > 8){
    	dir = -1*dir;
    }
    positionx -= dir*0.1f;

	glTranslatef(positionx, positiony, positionz);
	glColor3f(0.0,0.0,1.0);
    glutSolidSphere(1.0,20,20);    

    glPushMatrix();
	    float radio = 10.0;
	    glTranslatef(radio*cos(angulo), radio*sin(angulo), 0.0f);
	    glRotatef(angle,0.0f,0.0f,1.0f);
	    glColor3f(1.0,1.0,1.0);
	    glutSolidTeapot(1.0);

	    glTranslatef(3.0f*sin(tor_angle), 0.0, 3.0f*cos(tor_angle));
	    glColor3f(0.0,1.0,0.0);
	    glutSolidTorus(0.2,0.7,20,20);
    glPopMatrix();
    
    glPushMatrix();
	    //glTranslatef(positionx, positiony, positionz);
	    
	    glTranslatef(0.0f, radio*sin(2.0f*angulo), radio*cos(2.0f*angulo));
	    glRotatef(angle,2.0f,0.0f,0.0f);
	    glColor3f(1.0,1.0,1.0);
	    glutSolidCube(2.0);
    glPopMatrix();
	
    angulo+=0.02f;
    angle=angulo * 180/ pi;
    tor_angle = 3*angulo;
}

float earth_t = 0.0f, mars_t = 0.0f, moon_t = 0.0f, sun_t = 0.0f ;
float earth_rotation =0.0f, mars_rotation =0.0f, moon_rotation =0.0f, sun_rotation =0.0f;
////
void exercise4(){
	// sun
    glPushMatrix();
	    glColor3f(1.0,1.0,0.0);
	    glRotatef(sun_rotation, 0.0f,1.0f,0.0f);
	    glutSolidSphere(4.0f,40,40);
    glPopMatrix();
    // earth
    glPushMatrix();
	    float d_earth = 10.0;
	    glTranslatef(d_earth*sin(earth_t),0.0f, d_earth*cos(earth_t));
	    glRotatef(earth_rotation,0.0f,1.0f,0.0f);
	    glColor3f(0.0,0.0,1.0);
	    glutSolidSphere(2.0,40,40);

	    // moon
	    float d_moon = 4.0;
	    glTranslatef(d_moon*sin(moon_t), 0.0f, d_moon*cos(moon_t));
	    glRotatef(moon_rotation, 0.0f,1.0f,0.0f);
	    glColor3f(1.0,1.0,1.0);
	    glutSolidSphere(1.0,40,40);
    glPopMatrix();
    glPopMatrix();
    // Mars
    glPushMatrix();
	    float d_mars = 18.0;
	    glTranslatef(d_mars*sin(mars_t), 0.0f, d_mars*cos(mars_t));
	    glRotatef(mars_rotation, 0.0f, 1.0f, 0.0f);
	    glColor3f(1.0,0.0f,0.0f);
	    glutSolidSphere(2.0f, 40, 40);
    glPopMatrix();

    sun_rotation+=0.2f;
    earth_rotation = 3.0f*sun_rotation;
    moon_rotation = 1.5f*sun_rotation;
    mars_rotation = sun_rotation;
    earth_t += 0.05f;
    moon_t = earth_t*2.0f;
    mars_t = earth_t;
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);


	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("TP 2 : Transformaciones");


	initGL();
	init_scene();

	glutDisplayFunc(&window_display);

	glutReshapeFunc(&window_reshape);

	glutKeyboardFunc(&window_key);

	//function called on each frame
	glutIdleFunc(&window_idle);

	glutMainLoop();

	return 1;
}



GLvoid initGL()
{
	GLfloat position[] = { 0.0f, 5.0f, 10.0f, 0.0 };

	//enable light : try without it
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHTING);
	//light 0 "on": try without it
	glEnable(GL_LIGHT0);

	//shading model : try GL_FLAT
	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);

	//enable material : try without it
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glClearColor(RED, GREEN, BLUE, ALPHA);
}


int a = 0;
GLvoid window_display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-25.0f, 25.0f, -25.0f, 25.0f, -25.0f, 25.0f);


	/*dibujar aqui*/
	//translate_rotate(0.1 , 10); 	// ejercicios 1
	//translate_rotate_idavuelta(0.1,8,-8); // ejercicio 2
 	exercise3(); // ejercicio 3//exercise4(); // ejercicio 4
 	//exercise4();

	glutSwapBuffers();

	glFlush();
}

GLvoid window_reshape(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-25.0f, 25.0f, -25.0f, 25.0f, -25.0f, 25.0f);

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
