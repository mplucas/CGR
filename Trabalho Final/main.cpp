// g++ main.cpp -lglut -lGL -lGLU -lm; ./a.out
#include <GL/glut.h>
#include <time.h>
#include <math.h>
#include<bits/stdc++.h>

#define MAX_FLOCOS 500
#define PI 3.1415

using namespace std;

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

typedef struct Ponto2D{
    float x,z;
} ponto2D;

void plotCurve1(){

    vector<ponto2D> result = vector<ponto2D>();
    ponto2D p0, p1, p2, p3, p4, p5;
    
    p0.x = -1.0;
    p0.z = -2.0;
    
    p1.x = 0.0;
    p1.z = -1.0;

    p2.x = 1.0;
    p2.z = 0.0;

    p3.x = 0.0;
    p3.z = 1.0;

    p4.x = -2.0;
    p4.z = 3.0;

    p5.x = 0.5-1;
    p5.z = 5.0;

    GLfloat t = 0;
    GLfloat step = 0.1;

    while( t <= (1 + step)){

        ponto2D aux;

        aux.x = pow(1 - t, 5) * p0.x + \
                 5 * t * pow(1 - t, 4) * p1.x + \
                 10 * t * t * pow(1 - t, 3) * p2.x + \
                 10 * t * t * t * pow(1 - t, 2) * p3.x + \
                 5 * t * t * t * t * (1 - t) * p4.x + \
                 t * t * t * t * t * p5.x;
        aux.z = pow(1 - t, 5) * p0.z + \
                 5 * t * pow(1 - t, 4) * p1.z + \
                 10 * t * t * pow(1 - t, 3) * p2.z + \
                 10 * t * t * t * pow(1 - t, 2) * p3.z + \
                 5 * t * t * t * t * (1 - t) * p4.z + \
                 t * t * t * t * t * p5.z;

        result.push_back(aux);
        t += step;
    }
    
    glBegin(GL_LINE_STRIP);
    for(int i = 0; i < result.size(); i+=2){        
        glVertex3f(result[i].x, result[i].z, .0f);
        /*glVertex3f(result[i+1].x, result[i+1].z, .0f);*/
        
    }
    glEnd();


}

void OnDisplay(void){

    GLUquadricObj *pObj = gluNewQuadric();
    glClear(GL_DEPTH_BUFFER_BIT);
    
    if(opcao == '1'){
        animando = true;
        opcao = '0';
    }
    if(opcao == '2'){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
                // glBegin(GL_LINES);
                //     glVertex3f(0.f, 0.5f, 0.f);
                //     glVertex3f(0.f, -0.5f, 0.f);
                //     glVertex3f(0.1f, -0.5f, 0.2f);
                //     glVertex3f(0.f, -1.f, 0.1f);
                //     glVertex3f(0.f, -0.3f, 0.2f);
                //     glVertex3f(0.1f, -1.3f, 0.2f);
                // glEnd();
                plotCurve1();
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
    yRot -= 1.;

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
