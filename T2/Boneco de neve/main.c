// gcc snowman.c -lglut -lGL -lGLU -lm -o snowman && ./snowman

#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_FLOCOS 500
#define PI 3.1415

// gcc main.c -Wall -lGL -lGLU -lglut -lm

// Rotation amounts
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
float radius = 1.0f;
float angle = 0.0f;

typedef struct floco {
	float pos[3];
	float vel;
} Floco;

Floco flocos[MAX_FLOCOS];

// Change viewing volume and viewport.  Called when window is resized
void ChangeSize(int w, int h) {
    GLfloat fAspect;

    // Prevent a divide by zero
    if(h == 0)
        h = 1;

    // Set Viewport to window dimensions
    glViewport(0, 0, w, h);

    fAspect = (GLfloat)w/(GLfloat)h;

    // Reset coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Produce the perspective projection
    gluPerspective(30.0f, fAspect, 1.0, 40.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}



void init(){
    glEnable(GL_DEPTH_TEST);    
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);     
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glClearColor(102.0/255.0f, 204.0/255.0f, 1.0f, 1.0f);
}

// Respond to arrow keys
void SpecialKeys(int key, int x, int y){

    // Refresh the Window


}

void cilinderRounded(GLUquadric* quad, GLdouble base, GLdouble top, GLdouble height){

	gluCylinder(quad, base, top, height, 50, 25);
	glTranslatef(0.0f, 0.0f, height);
	gluSphere(quad, top, 50, 25);

}

void OnDisplay(void){

    GLUquadricObj *pObj = gluNewQuadric();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // +1
    glPushMatrix();

	// Move object back and do in place rotation
	glTranslatef(0.0f, -0.7f, -5.0f);
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	//gluQuadricNormals(pObj, GLU_SMOOTH);

	// Esfera de baixo
	// +2
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	gluSphere(pObj, radius*0.30f, 50, 25);
	
	// Esfera do meio
	glTranslatef(0.0f, 0.40f, 0.0f);
	gluSphere(pObj, radius*0.25f, 50, 25);
	
	// Esfera de cima
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(0.0f, 0.36f, 0.0f);
	gluSphere(pObj, radius*0.2f, 50, 25);

	// Olhos
	glColor3f(0.0f, 0.0f, 0.0f);
	
	glTranslatef(0.1f, 0.1f, 0.15f);
	gluSphere(pObj, radius*0.02f, 12, 6);

	glTranslatef(-0.2f, 0.0f, 0.0f);
	gluSphere(pObj, radius*0.02f, 12, 6);

	// Nariz
	glColor3f(1.0f, 153.0/255.0f, 51.0/255.0f);

	glTranslatef(0.1f, -0.12f, 0.0f);
	cilinderRounded(pObj, radius*0.02f, radius*0.01f, 0.25f);

	glPopMatrix();
	// -2

	// braços
	// braço esquerdo
	glPushMatrix();
	glColor3f(102.0/255.0f, 51.0/255.0f, 0.0f);

	glTranslatef(radius*0.25f, 0.40f, 0.0f);
	glRotatef(90, 0.0f, 1.0f, 0.0f);
	cilinderRounded(pObj, radius*0.025f, radius*0.02f, 0.27f);
	glTranslatef(0.0f, 0.0f, -0.1f);
	glRotatef(30, 1.0f, 0.0f, 0.0f);
	cilinderRounded(pObj, radius*0.01f, radius*0.005f, 0.1f);

	glPopMatrix();

	// braço direito
	glPushMatrix();

	glTranslatef(-radius*0.25f, 0.40f, 0.0f);
	glRotatef(-90, 0.0f, 1.0f, 0.0f);
	cilinderRounded(pObj, radius*0.025f, radius*0.02f, 0.27f);
	glTranslatef(0.0f, 0.0f, -0.17f);
	glRotatef(30, 1.0f, 0.0f, 0.0f);
	cilinderRounded(pObj, radius*0.01f, radius*0.005f, 0.1f);
	glTranslatef(0.0f, 0.0f, -0.1f);
	glRotatef(-30, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, -0.03f);
	glRotatef(-30, 1.0f, 0.0f, 0.0f);
	cilinderRounded(pObj, radius*0.01f, radius*0.005f, 0.1f);

	glPopMatrix();

	//cachecol
	glPushMatrix();
	glColor3f(153.0/255.0f, 0.0f, 0.0f);

	glTranslatef(0.0f, 0.40f + radius*0.25f, 0.0f);
	glRotatef(90, 1.0f, 0.0f, 0.0f);
	gluCylinder(pObj, radius*0.2f, radius*0.2f, 0.08f, 50, 25);
	glTranslatef(radius*0.12f, radius*0.12f, 0.02f);
	glRotatef(-30, 1.0f, 0.0f, 0.0f);
	gluCylinder(pObj, radius*0.04f, radius*0.04f, 0.22f, 50, 25);
	glTranslatef(0.0f, 0.0f, 0.22f);
	glRotatef(30, 1.0f, 0.0f, 0.0f);
	gluCylinder(pObj, radius*0.04f, radius*0.04f, 0.22f, 50, 25);
	gluSphere(pObj, radius*0.04f, 50, 25);

	glPopMatrix();

    // Restore the matrix state
	// -1
    glPopMatrix();
	
    // Buffer swap
    glutSwapBuffers();

    yRot = (GLfloat)((const int)yRot % 360);
    yRot -= 1.0;

    glutPostRedisplay();
}


int main(int argc, char *argv[]){

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Modeling with Quadrics");
    glutReshapeFunc(ChangeSize);
    glutSpecialFunc(SpecialKeys);
    glutDisplayFunc(OnDisplay);
    init();
    glutMainLoop();

    return 0;

}
