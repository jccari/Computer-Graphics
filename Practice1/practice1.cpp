//COMPILE: g++ practice1.cpp -lglut -lGL

#define GLUT_DISABLE_ATEXIT_HACK	
//#include <windows.h>
#include <math.h>
#include <GL/glut.h>

#define KEY_ESC 27
#define PI 3.1415926f

// function for exercice 1
void makeSquare(int center, int aristSize){
    float xinit = (float) center - (aristSize/2);
    float yinit = (float) center - (aristSize/2);
    glBegin(GL_LINE_LOOP);
        glColor3d(0,0,255);
        glVertex2f(xinit, yinit);
        glVertex2f(xinit+aristSize, yinit);
        glVertex2f(xinit+aristSize, yinit-aristSize);
        glVertex2f(xinit, yinit-aristSize );
    glEnd();
}

// function for exercice 2
void makeCircle(float xcenter, float ycenter, float radio){
    float x, y ;
    float delta = 0.01;
    float angle = 0;
    glBegin(GL_LINE_STRIP);
    	while(angle < 2*PI){
    		x = xcenter + radio*cos(angle);
    		y = ycenter + radio*sin(angle);

    		angle += delta;
    		glVertex2f(x,y);
    		//cout << angle << " = ("<< x << " , " << y << ")" << endl;
    	}
    glEnd();
}

// function for exercice 3
void makeCircles(float xcenter, float ycenter, float radio, int circles, float reduction){
	for (int i = 0; i < circles; ++i){
		makeCircle(xcenter, ycenter, radio);

		//cout << " center = ("<< xcenter << " , " << ycenter << ")" << endl;
		//cout << " radio = " << radio << endl;

		xcenter = xcenter + reduction;
		radio = radio + reduction;
	}
}

// function for exercice 3
void makeCirclesXAxis(float xcenter, float ycenter, float radio, int circles, float reduction){
	for (int i = 0; i < circles; ++i){
		makeCircle(xcenter, ycenter, radio);

		//cout << " center = ("<< xcenter << " , " << ycenter << ")" << endl;
		//cout << " radio = " << radio << endl;
		
		xcenter = xcenter + radio + (radio - radio*reduction/100.0f);
		radio = radio - radio*reduction/100.0f;
	}
}

double gradesToRadians(float grades){
	return (double) (grades*PI)/180.0f;
}

void makeCircles(float xcenter, float ycenter, float radio, int circles, float reduction, int angle){
	float xcenter_last, ycenter_last;

	double radians = gradesToRadians(angle);

	for (int i = 0; i < circles; ++i){

		makeCircle(xcenter, ycenter, radio);

		xcenter_last = xcenter;
		ycenter_last = ycenter;

		xcenter = xcenter + radio + (radio - radio*reduction/100.0f);


		xcenter = ( xcenter - xcenter_last)*cos(radians) - (ycenter - ycenter_last)*sin(radians) + xcenter_last; 
		ycenter = ( xcenter - xcenter_last)*sin(radians) + (ycenter - ycenter_last)*sin(radians) + ycenter_last;


		radio = radio - radio*reduction/100.0f;	

	}
}


//
//funcion llamada a cada imagen
void glPaint(void) {

	//El fondo de la escena al color initial
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro
	glLoadIdentity();
	

	//makeSquare(0, 5); 		// ejercicio 1
	//makeCircle(0,0,20); 		// ejercicio 2
	//makeCircles(1,1,5,5, 3);	// ejercicio 3
	//makeCirclesXAxis(-30,1,10,5, 25); // ejercicio 4
	makeCircles(2,2,10,5, 25, 180); // ejercicio 5

	//doble buffer, mantener esta instruccion al fin de la funcion
	glutSwapBuffers();
}

//
//inicializacion de OpenGL
//
void init_GL(void) {
	//Color del fondo de la escena
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro

	//modo projeccion 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

//en el caso que la ventana cambie de tamaño
GLvoid window_redraw(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glOrtho(-50.0f,  50.0f,-50.0f, 50.0f, -1.0f, 1.0f); 
	// todas la informaciones previas se aplican al la matrice del ModelView
	glMatrixMode(GL_MODELVIEW);
}

GLvoid window_key(unsigned char key, int x, int y) {
	switch (key) {
	case KEY_ESC:
		exit(0);
		break;

	default:
		break;
	}

}
//
//el programa principal
//
int main(int argc, char** argv) {

	//Inicializacion de la GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600); //tamaño de la ventana
	glutInitWindowPosition(100, 100); //posicion de la ventana
	glutCreateWindow("TP1 OpenGL : hello_world_OpenGL"); //titulo de la ventana

	init_GL(); //funcion de inicializacion de OpenGL

	glutDisplayFunc(glPaint); 
	glutReshapeFunc(&window_redraw);
	// Callback del teclado
	glutKeyboardFunc(&window_key);

	glutMainLoop(); //bucle de rendering

	return 0;
}