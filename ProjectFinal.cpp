#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include<Windows.h>
#include<mmsystem.h>
const double PI = 3.14;
float camposz = 15.0; 


int miliseconds = 10;      //Time between each screen update
float max_angle = 50;       //Maximum value for angle
float incrementomax = 5;  //Maximum value for angle increments
float angle = -max_angle;  //The left sphere will start suspended with the highest valid angle
bool clockwise = false;     //and will move counter-clockwise
							//Spheres
int Spheres = 12;            //Number of spheres in total

int num_of_moving_spheres;        //Number of spheres that will be in motion

float sphere_diam = 1.0;       
float thread_len = 4;  
//The length of the thread is calculated based on the size of the tubes, the diameter of the spheres and
//the distance between the spheres and the base





void initRendering()  
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

}

void posicionarCamara() {
	glTranslatef(0, 0, -camposz);

}

double toDeg(double radian) {
	return radian * 180 / PI;
}

double toRad(double degree) {
	return degree*PI / 180;
}


void SphereAttributes(float angle) {
	glPushMatrix(); 
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.0f, -thread_len, 0.0f);
	glColor3f(0.9, 0.0, 0.2);
	//Lights         
	GLfloat mat_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
	GLfloat mat_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 300.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	//Draw Sphere
	glutSolidSphere(sphere_diam / 2, 30, 30);
	glRotatef(-angle, 0.0f, 0.0f, 1.0f);
	//Draw threads
	float distX = sin(toRad(angle))*thread_len;
	float distY = cos(toRad(angle))*thread_len;
	glColor3f(0.72f, 0.54f, 0.8f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-distX, distY, 0);
	glEnd();
	glPopMatrix();
}
void DrawSpheres()//draw spheres
{
	glPushMatrix();
glTranslatef(0.0f, 3, 0.0f); 
	for (int i = 1; i <= Spheres; i++) {
		glPushMatrix();
   	glTranslatef(-Spheres / 2.0f - sphere_diam / 2.0f + i*sphere_diam, 0.0f, 0.0f); //Center in X
	if (i <= num_of_moving_spheres && angle < 0)  
			SphereAttributes(angle);
else if (i > Spheres - num_of_moving_spheres && angle > 0) 
			SphereAttributes(angle);
		else
			SphereAttributes(0); // 
		glPopMatrix();
	}

	glPopMatrix();
}

void drawScene() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	posicionarCamara();
	DrawSpheres();
	glutSwapBuffers();
}

void update(int value) {
	float incremento = incrementomax - abs(angle) / max_angle *incrementomax*0.85;
	if (clockwise && angle <= -max_angle) {
		clockwise = false;
	}
	else if (!clockwise && angle >= max_angle) {
		clockwise = true;
	}
	if (clockwise)
		angle -= incremento;
	else
		angle += incremento;
	glutPostRedisplay();
	glutTimerFunc(miliseconds, update, 0);
}



void handleResize(int w, int h) {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
	glViewport(0, 0, w, h);
}





int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	scanf("%d", &num_of_moving_spheres);
	glutCreateWindow("Newton Cradle");
	PlaySound("newtons-cradle.wav", NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
	initRendering();
	glutDisplayFunc(drawScene);
	glutReshapeFunc(handleResize);
	glutTimerFunc(miliseconds, update, 0);
	glutMainLoop();
	return 0;
}