#include <iostream>
#include <GL/glut.h>
#include <math.h>
#include <glm\glm\glm.hpp>

const int SCREEN_WIDTH = 300;
const int SCREEN_HEIGHT = 340;
glm::vec3 camera(0.0f, 0.0f, 11.0f);
glm::vec3 lookat(0.0f, 0.0f, 0.0f);

float headSize = 20.0f;
float bodyHeight = 100.0f;
float limbSize = bodyHeight/100.0f * 20.0f;
float armLength = limbSize * 10.0f;
float legLength = armLength;

void init() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 400, 0, 500);
}

void updateValues() {
	limbSize = bodyHeight / 100.0f * 7.0f;
	armLength = limbSize * 10.0f;
	legLength = armLength;
}

void drawFigure() {

	init();
	updateValues();
	gluLookAt(camera.x, camera.y, camera.z, lookat.x, lookat.y, lookat.z, 0, 1, 0);
	glPushMatrix();
		
		// Body
		std::cout << bodyHeight << std::endl;
		glTranslatef(SCREEN_WIDTH - bodyHeight, SCREEN_HEIGHT - bodyHeight + legLength, 0);
		glColor3f(0.0f, 0.8f, 0.8f);
		glBegin(GL_QUADS);
		
		//top
		glVertex3f(-bodyHeight/2, bodyHeight, -10);
		glVertex3f(bodyHeight/2, bodyHeight, -10);
		glVertex3f(bodyHeight/2, bodyHeight, 10);
		glVertex3f(-bodyHeight/2, bodyHeight, 10);

		//bot
		glVertex3f(-bodyHeight / 2, -bodyHeight, 10);
		glVertex3f(bodyHeight / 2, -bodyHeight, 10);
		glVertex3f(bodyHeight / 2, -bodyHeight, -10);
		glVertex3f(-bodyHeight / 2, -bodyHeight, -10);

		//front
		glVertex3f(-bodyHeight / 2, bodyHeight, 10);
		glVertex3f(bodyHeight / 2, bodyHeight, 10);
		glVertex3f(bodyHeight / 2, -bodyHeight, 10);
		glVertex3f(-bodyHeight / 2, -bodyHeight, 10);

		//back
		glVertex3f(-bodyHeight / 2, -bodyHeight, -10);
		glVertex3f(bodyHeight / 2, -bodyHeight, -10);
		glVertex3f(bodyHeight / 2, bodyHeight, -10);
		glVertex3f(-bodyHeight / 2, bodyHeight, -10);

		//left
		glVertex3f(-bodyHeight / 2, bodyHeight, -10);
		glVertex3f(-bodyHeight / 2, bodyHeight, 10);
		glVertex3f(-bodyHeight / 2, -bodyHeight, 10);
		glVertex3f(-bodyHeight / 2, -bodyHeight, -10);

		//right
		glVertex3f(bodyHeight / 2, bodyHeight, 10);
		glVertex3f(bodyHeight / 2, bodyHeight, -10);
		glVertex3f(bodyHeight / 2, -bodyHeight, -10);
		glVertex3f(bodyHeight / 2, -bodyHeight, 10);

		glEnd();

		// Head
		glPushMatrix();
			glTranslatef(0, bodyHeight + headSize, 0);
			glColor3f(1.0f, 0.0f, 0.0f);
			glutSolidSphere(headSize, 20, 2);
		glPopMatrix();

		// Arms
		glPushMatrix();
			glTranslatef(bodyHeight/2, bodyHeight - limbSize, 0);
			glLineWidth(limbSize);
			glColor3f(0.0, 1.0, 0.0);
			glBegin(GL_LINES);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(armLength, 0, 0);
			glEnd();
			glPushMatrix();
				glTranslatef(armLength, 0, 0);
				glLineWidth(limbSize);
				glColor3f(1.0, 0.0, 0.0);
				glBegin(GL_LINES);
				glVertex3f(0.0, 0.0, 0.0);
				glVertex3f(armLength, 0, 0);
				glEnd();
			glPopMatrix();

			glTranslatef(-bodyHeight, 0, 0);
			glLineWidth(limbSize);
			glColor3f(0.0, 1.0, 0.0);
			glBegin(GL_LINES);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(-armLength, 0, 0);
			glEnd();
			glPushMatrix();
				glTranslatef(-armLength, 0, 0);
				glLineWidth(limbSize);
				glColor3f(1.0, 0.0, 0.0);
				glBegin(GL_LINES);
				glVertex3f(0.0, 0.0, 0.0);
				glVertex3f(-armLength, 0, 0);
				glEnd();
			glPopMatrix();

		glPopMatrix();

		// Legs
		glPushMatrix();
			glTranslatef(bodyHeight / 2 - limbSize, -bodyHeight, 0);
			glLineWidth(limbSize);
			glColor3f(0.0, 1.0, 0.0);
			glBegin(GL_LINES);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(0, -legLength, 0);
			glEnd();
			glPushMatrix();
				glTranslatef(0, -legLength, 0);
				glLineWidth(limbSize);
				glColor3f(1.0, 0.0, 0.0);
				glBegin(GL_LINES);
				glVertex3f(0.0, 0.0, 0.0);
				glVertex3f(0, -legLength, 0);
				glEnd();
			glPopMatrix();

			glTranslatef(-bodyHeight + limbSize * 2, 0, 0);
			glLineWidth(limbSize);
			glColor3f(0.0, 1.0, 0.0);
			glBegin(GL_LINES);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(0, -legLength, 0);
			glEnd();
			glPushMatrix();
				glTranslatef(0, -legLength, 0);
				glLineWidth(limbSize);
				glColor3f(1.0, 0.0, 0.0);
				glBegin(GL_LINES);
				glVertex3f(0.0, 0.0, 0.0);
				glVertex3f(0, -legLength, 0);
				glEnd();
			glPopMatrix();

		glPopMatrix();


		glFlush();

	glPopMatrix();

	glutSwapBuffers();
	
}

void keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'a':
		camera.z -= 0.1f;
		init();
		glutPostRedisplay();
		break;
	case 'd':
		camera.z += 0.1f;
		init();
		glutPostRedisplay();
		break;
	case 'w':
		bodyHeight += 1;
		glutPostRedisplay();
		break;
	case 's':
		bodyHeight -= 1;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

int  main() {
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(500, 500);
	glutCreateWindow("Figure");
	glutDisplayFunc(drawFigure);
	//glutIdleFunc(drawFigure);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}