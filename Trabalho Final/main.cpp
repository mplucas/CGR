// g++ main.cpp -lglut -lGL -lGLU -lm; ./a.out
#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_FLOCOS 500
#define PI 3.1415

// Rotation amounts
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
static int grauBraco = 0;
static bool subindo = true;
static int opcao = -1;
static float grauSweep = 0;
static bool animando = false;
static bool animandoDireita = true;
static bool voltandoCabeca = false;

float radius = 1.0f;
float angle = 0.0f;

typedef struct floco {
	float pos[3];
	float vel;
} Floco;

Floco flocos[MAX_FLOCOS];



double bezier(double A,  // Start value
              double B,  // First control value
              double C,  // Second control value
              double D,  // Ending value
              double t)  // Parameter 0 <= t <= 1
{
    double s = 1 - t;
    double AB = A*s + B*t;
    double BC = B*s + C*t;
    double CD = C*s + D*t;
    double ABC = AB*s + BC*t;
    double BCD = BC*s + CD*t;
    return ABC*s + BCD*t;
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
    glClear(GL_DEPTH_BUFFER_BIT);
    
    if(opcao == '1'){
        animando = true;
        opcao = '0';
    }

    // +1
    glPushMatrix();

        // Move object back and do in place rotation
        glTranslatef(0.0f, 0.0f, -5.0f);
        glRotatef(xRot, 1.0f, 0.0f, 0.0f);
        glRotatef(yRot, 0.0f, 1.0f, 0.0f);

        glColor3f(255.0f, .0f, .0f);

        glTranslatef(.5f, 0.0f, 0.0f);
        
        // teste
        if(animando){

            glPushMatrix();

                glTranslatef(-0.0f, 0.0f, 0.0f);
                // glRotatef(90, 1.0f, 0.0f, 0.0f);
                // gluCylinder(pObj, radius*0.15f, radius*0.15f, 0.4f, 50, 25);
                glBegin(GL_LINES);
                    glVertex3f(0.f, 0.5f, 0.f);
                    glVertex3f(0.f, -0.5f, 0.f);
                glEnd();
                grauSweep++;

                if(grauSweep == 360){
                    animando = false;
                    grauSweep = 0;
                }

            glPopMatrix();

        }  
            
    glPopMatrix();
	
    // Buffer swap
    glutSwapBuffers();

    yRot = (GLfloat)((const int)yRot % 360);
    yRot -= 2.0;

    if(subindo){
        grauBraco++;
        if(grauBraco == 45)
            subindo = false;
    }else{
        grauBraco--;
        if(grauBraco == 0)
            subindo = true;
    }



    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	opcao = key;
}


int main(int argc, char *argv[]){

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Modeling with Quadrics");
    glutReshapeFunc(ChangeSize);
    glutSpecialFunc(SpecialKeys);
    glutDisplayFunc(OnDisplay);
    glutKeyboardFunc(keyboard);
    init();
    glutMainLoop();

    return 0;

}
