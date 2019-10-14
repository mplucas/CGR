// gcc main.c -lglut -lGL -lGLU -lm
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
    glTranslatef(0.0f, 0.0f, -height);

}

void OnDisplay(void){

    GLUquadricObj *pObj = gluNewQuadric();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // +1
    glPushMatrix();

        // Move object back and do in place rotation
        glTranslatef(0.0f, 0.0f, -5.0f);
        glRotatef(xRot, 1.0f, 0.0f, 0.0f);
        glRotatef(yRot, 0.0f, 1.0f, 0.0f);

        // vestido tronco
        glPushMatrix();

            glColor3f(204.0/255.0f, .0f, 68.0/255.0f);
            glTranslatef(0.0f, -radius*0.25f, 0.0f);
            glRotatef(90, 1.0f, 0.0f, 0.0f);
            gluCylinder(pObj, radius*0.15f, radius*0.3f, 0.4f, 50, 25);

            // Cabeça
            glPushMatrix();

                glRotatef(-90, 1.0f, 0.0f, 0.0f);
                glTranslatef(0.0f, radius*0.25f, 0.0f);
                glColor3f(1.0f, 153.0/255.0f, 204.0/255.0f);
                gluSphere(pObj, radius*0.3f, 50, 25);

                // nariz
                glPushMatrix();

                    glTranslatef(0.0f, 0.1f, 0.0f);
                    gluCylinder(pObj, radius*0.2f, radius*0.15f, 0.45f, 50, 25);
                    glTranslatef(0.0f, 0.0f, 0.45f);
                    gluDisk(pObj, 0.0, radius*0.15f, 300, 90);
                
                    // narina direita
                    glColor3f(.0f, .0f, .0f);
                    glTranslatef(.07f, 0.02f, 0.01f);
                    gluDisk(pObj, 0.0, radius*0.03, 300, 90);

                    // narina esquerda
                    glTranslatef(-.14f, 0.0f, 0.0f);
                    gluDisk(pObj, 0.0, radius*0.03, 300, 90);

                glPopMatrix();

                // olho esquerdo
                glPushMatrix();

                    glColor3f(1.0f, 1.0f, 1.0f);
                    glTranslatef( .15f, .2f, .15f );
                    gluSphere(pObj, radius*0.07f, 50, 25);
                    glColor3f(.0f, .0f, .0f);
                    glTranslatef( .0f, .03f, .03f );
                    gluSphere(pObj, radius*0.04f, 50, 25);

                glPopMatrix();

                // olho direito
                glPushMatrix();

                    glColor3f(1.0f, 1.0f, 1.0f);
                    glTranslatef( -.15f, .2f, .15f );
                    gluSphere(pObj, radius*0.07f, 50, 25);
                    glColor3f(.0f, .0f, .0f);
                    glTranslatef( .0f, .03f, .03f );
                    gluSphere(pObj, radius*0.04f, 50, 25);
                
                glPopMatrix();

                //orelha esquerda
                glPushMatrix();

                    glColor3f(1.0f, 153.0/255.0f, 204.0/255.0f);
                    glTranslatef( .07f, radius*0.25f, .0f );
                    glRotatef(-90, 1.0f, 0.0f, 0.0f);
                    cilinderRounded( pObj, radius*0.02f, radius*0.035f, 0.15f );

                glPopMatrix();

                //orelha direita
                glPushMatrix();

                    glColor3f(1.0f, 153.0/255.0f, 204.0/255.0f);
                    glTranslatef( -.07f, radius*0.25f, .0f );
                    glRotatef(-90, 1.0f, 0.0f, 0.0f);
                    cilinderRounded( pObj, radius*0.02f, radius*0.035f, 0.15f );

                glPopMatrix();

                // boca
                glPushMatrix();

                    glColor3f(.0f, .0f, .0f);
                    glTranslatef( .0f, -.12f, .27f );
                    gluDisk(pObj, 0.0, radius*0.1, 300, 90);

                glPopMatrix();

            glPopMatrix();

            // perna esquerda
            glPushMatrix();

                glColor3f(1.0f, 153.0/255.0f, 204.0/255.0f);
                glTranslatef(.07f, 0.0f, 0.4f);
                gluCylinder(pObj, radius*0.025f, radius*0.015f, 0.13f, 50, 25);
                
                // pé
                glPushMatrix();

                    glColor3f(102.0/255.0f, 51.0/255.0f, 0.0f);
                    glRotatef(-90, 1.0f, 0.0f, 0.0f);
                    glTranslatef(.0f, -.13f, -.02f);
                    gluSphere(pObj, radius*0.02f, 50, 25);
                    cilinderRounded( pObj, radius*0.02f, radius*0.025f, 0.15f );

                glPopMatrix();

            glPopMatrix();

            // perna direita
            glPushMatrix();

                glColor3f(1.0f, 153.0/255.0f, 204.0/255.0f);
                glTranslatef(-.07f, 0.0f, 0.4f);
                gluCylinder(pObj, radius*0.025f, radius*0.015f, 0.13f, 50, 25);
                
                // pé
                glPushMatrix();

                    glColor3f(102.0/255.0f, 51.0/255.0f, 0.0f);
                    glRotatef(-90, 1.0f, 0.0f, 0.0f);
                    glTranslatef(.0f, -.13f, -.02f);
                    gluSphere(pObj, radius*0.02f, 50, 25);
                    cilinderRounded( pObj, radius*0.02f, radius*0.025f, 0.15f );

                glPopMatrix();

            glPopMatrix();

            // braço esquerdo
            glPushMatrix();

                glColor3f(1.0f, 153.0/255.0f, 204.0/255.0f);
                glTranslatef(.17f, .0f, .12f);
                glRotatef(90, .0f, 1.0f, .0f);
                cilinderRounded( pObj, radius*0.013f, radius*0.013f, 0.17f );
                glTranslatef(.0f, .0f, .12f);
                glRotatef(45, 1.0f, .0f, .0f);
                cilinderRounded( pObj, radius*0.013f, radius*0.013f, 0.03f );
                glRotatef(-90, 1.0f, .0f, .0f);
                cilinderRounded( pObj, radius*0.013f, radius*0.013f, 0.03f );
                glRotatef(45, 1.0f, .0f, .0f);

            glPopMatrix();

            // braço direito
            glPushMatrix();

                glTranslatef(.0f, .0f, -.46f);
                glRotatef(180, 1.0f, .0f, .0f);
                cilinderRounded( pObj, radius*0.013f, radius*0.013f, 0.17f );
                glTranslatef(.0f, .0f, .12f);
                glRotatef(45, 1.0f, .0f, .0f);
                cilinderRounded( pObj, radius*0.013f, radius*0.013f, 0.03f );
                glRotatef(-90, 1.0f, .0f, .0f);
                cilinderRounded( pObj, radius*0.013f, radius*0.013f, 0.03f );
                glRotatef(45, 1.0f, .0f, .0f);

            glPopMatrix();

        glPopMatrix();

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
