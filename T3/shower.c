// gcc shower.c -lglut -lGL -lGLU -lm
#include <unistd.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>

#define MAX_GOTAS 1000
#define PI 3.1415

// Rotation amounts
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
float radius = 1.0f;
float angle = 0.0f;

typedef struct floco {
	float pos[3];
	float vel;
} Floco;

Floco gotas[MAX_GOTAS];

void cilinderRounded(GLUquadric* quad, GLdouble base, GLdouble top, GLdouble height){

	gluCylinder(quad, base, top, height, 50, 25);
	glTranslatef(0.0f, 0.0f, height);
	gluSphere(quad, top, 50, 25);

}

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
    glClearColor(0.7f, 0.7f, 1.0f, 1.0f);

	int i;
	srand(time(NULL));
	
	for(i=0; i<MAX_GOTAS; i++){
		// Gerar um x aleatorio
		float x = (float) (rand() % 12)/100;
		x *= (rand() % 2 ? 1 : -1);

		float y = (float) (rand() % 86) /100;

		float z = (float) (rand() % 12) /100;
		z *= (rand() % 2 ? 1 : -1);

		gotas[i].pos[0] = x;
		gotas[i].pos[1] = y;
		gotas[i].pos[2] = z;

		float v = (float) (1.0 / ((rand() % 50) + 100));
		gotas[i].vel = v;

		printf("\nx: %f - y: %f - z: %f - v: %f", x,y,z,v);
	}
}

void OnDisplay(void){

    GLUquadricObj *pObj = gluNewQuadric();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // +1
    glPushMatrix();

	// Move object back and do in place rotation
	glTranslatef(0.0f, -0.5f, -5.0f);
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	gluQuadricNormals(pObj, GLU_SMOOTH);

	// +2
	glPushMatrix();

	// Chuveiro
	glColor3f(153.0/255.0f, 102.0/255.0f, 51.0/255.0f);

	glRotatef(-90, 1.0f, 0.0f, 0.0f);
	cilinderRounded( pObj, radius*0.02f, radius*0.02f, 1.0f );
	
	glRotatef(90, 1.0f, 0.0f, 0.0f);
	cilinderRounded( pObj, radius*0.02f, radius*0.02f, 0.2f );

	glRotatef(90, 1.0f, 0.0f, 0.0f);
	cilinderRounded( pObj, radius*0.02f, radius*0.02f, 0.05f );
	gluCylinder(pObj, radius*0.02f, radius*0.15f, 0.1f, 50, 25);

	// gotas
	glPushMatrix();

	glTranslatef(0.0f, 0.0f, 0.1f);
	glRotatef(-90, 1.0f, 0.0f, 0.0f);
	glColor3f(52.0/255.0f, 235.0/255.0f, 222.0/255.0f);	
	int i;

	//PAREI AQUI

	for(i=0; i<MAX_GOTAS; i++){
		glPushMatrix();
		//sleep(rand() % 2);
		glTranslatef(gotas[i].pos[0], gotas[i].pos[1], gotas[i].pos[2]);

		if( gotas[i].pos[1] <= 0 )
			gluSphere(pObj, 0.005f,12 , 6);

		gotas[i].pos[1] -= gotas[i].vel;
		// gotas[i].pos[0] += sin(angle)*0.001f;
		// gotas[i].pos[0] += cos(i)*0.001f;
		if(gotas[i].pos[1] < -0.85f)
			gotas[i].pos[1] = 0.0;
		// if(gotas[i].pos[0] < -2 || gotas[i].pos[0] > 2){
		// 	gotas[i].pos[0] *= -1;
		// 	if(gotas[i].pos[0] >= 2)
		// 		gotas[i].pos[0] -= 0.3;
		// 	else if(gotas[i].pos[0] <= -2)
		// 		gotas[i].pos[0] += 0.3;
		// }
		glPopMatrix();
	}
	angle+=0.1f;
	glPopMatrix();
	
	glPopMatrix();
	// -2
	
    glPopMatrix();
	
    // Buffer swap
    glutSwapBuffers();

    yRot = (GLfloat)((const int)yRot % 360);
    yRot++;

    glutPostRedisplay();
}


int main(int argc, char *argv[]){

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Modeling with Quadrics");
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(OnDisplay);
    init();
    glutMainLoop();

    return 0;

}
