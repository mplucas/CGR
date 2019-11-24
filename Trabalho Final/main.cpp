// g++ main.cpp -lglut -lGL -lGLU -lm; ./a.out
#include <GL/glut.h>
#include <time.h>
#include <math.h>
#include<bits/stdc++.h>

#define MAX_FLOCOS 500
#define PI 3.1415

using namespace std;

// Rotation amounts
static float xRot = 0.0f;
static float yRot = 0.0f;
static int opcao = '0';
static float grauSweep = 0;
static bool animando = false;
static int refresh_times = 0;
static float pointX = INT_MAX;
static float pointY = INT_MAX;

float radius = 1.0f;
float angle = 0.0f;
float minimizer = 0.2f;

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
    
    glClear(GL_COLOR_BUFFER_BIT);
	
	glViewport( 0,0, 500, 500 );
	glMatrixMode( GL_PROJECTION );
	glOrtho( 0.0, 500.0, 0.0, 500.0, 1.0, -1.0 );
	
	/* initialize viewing values */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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
    float x,y;
} ponto2D;

void plotCurve1(){

    vector<ponto2D> result = vector<ponto2D>();
    ponto2D p0, p1, p2, p3, p4, p5;
    
    p0.x = -1.0;
    p0.y = -2.0;
    
    p1.x = 0.0;
    p1.y = -1.0;

    p2.x = 1.0;
    p2.y = 0.0;

    p3.x = 0.0;
    p3.y = 1.0;

    p4.x = -2.0;
    p4.y = 3.0;

    p5.x = -0.5;
    p5.y = 5.0;

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
        aux.y = pow(1 - t, 5) * p0.y + \
                 5 * t * pow(1 - t, 4) * p1.y + \
                 10 * t * t * pow(1 - t, 3) * p2.y + \
                 10 * t * t * t * pow(1 - t, 2) * p3.y + \
                 5 * t * t * t * t * (1 - t) * p4.y + \
                 t * t * t * t * t * p5.y;

        aux.x *= minimizer;
        aux.y *= minimizer;

        result.push_back(aux);
        t += step;
    }
    
    
    glBegin(GL_LINE_STRIP);     
    for(int i = 0; i < result.size(); i++){   
        glVertex2f(result[i].x, result[i].y);
    }
    glEnd();

}

void OnDisplay(void){
    
    if(opcao == '1'){
        animando = true;
        opcao = '-1';
    }
    if(opcao == '0'){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if(!--refresh_times)
            opcao = '-1';
    }

    if(animando){
        GLUquadricObj *pObj = gluNewQuadric();
        glClear(GL_DEPTH_BUFFER_BIT);

        // +1
        glPushMatrix();

            // Move object back and do in place rotation
            glTranslatef(0.0f, -.5f, -5.0f);
            glRotatef(xRot, 1.0f, 0.0f, 0.0f);
            glRotatef(yRot, 0.0f, 1.0f, 0.0f);

            glColor3f(255.0f, .0f, .0f);
                    
            glPushMatrix();

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

            glPopMatrix();
                
        glPopMatrix();
        
        yRot -= 0.3f;
        if(yRot <= -360.0f){
            yRot = 0;
            animando = false;
        }
    }else{
        GLUquadricObj *pObj = gluNewQuadric();
        glClear(GL_DEPTH_BUFFER_BIT);

        // +1
        glPushMatrix();
            
            glColor3f(255.0f, .0f, .0f);

            // Move object back and do in place rotation
            glTranslatef(0.0f, .0f, -5.0f);

            // testes
            // gluSphere(pObj, radius*0.02f, 50, 25);
            // glBegin(GL_LINE_STRIP);     
            //     glVertex2f(0, 0);
            //     glVertex2f(1, 1);
            // glEnd();

            if(pointX != INT_MAX){

                glTranslatef(pointX, pointY, .0f);
                gluSphere(pObj, radius*0.02f, 50, 25);
                glTranslatef(-pointX, -pointY, .0f);

            }
                
        glPopMatrix();
    }

    // Buffer swap
    glutSwapBuffers();

    glutPostRedisplay();
}

void drawPoint(int x, int y) {
    // GLUquadricObj *pObj = gluNewQuadric();
	//y = 250-y;
	pointX = (x-400)*0.005f;
	pointY = (300-y)*0.005f;
    
    printf("%f , %f\n", pointX, pointY);
	//glFlush();
}

void mouse(int bin, int state , int x , int y) {
	if(bin == GLUT_LEFT_BUTTON && state == GLUT_DOWN) drawPoint(x,y);
}

void keyboard(unsigned char key, int x, int y)
{
    // '-1' - nenhuma ação
    // '0'  - refresh
    // '1'  - curva 1
    // '2'  - curva 2
    // '3'  - curva 3
    // '4'  - curva 4
    // '5'  - curva 5
	opcao = key;
    if(key == '0')
        refresh_times = 5;
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
    glutMouseFunc(mouse);
    init();
    glutMainLoop();

    return 0;

}
