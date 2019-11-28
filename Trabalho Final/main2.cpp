// g++ main2.cpp -lglut -lGL -lGLU -lm; ./a.out
#include <GL/glut.h>
#include <time.h>
#include <math.h>
#include<bits/stdc++.h>

#define MAX_FLOCOS 500
#define PI 3.1415

using namespace std;

typedef struct Ponto2D{
    float x,y;
} ponto2D;

typedef struct Translation{
    float x,y,z;
} translation;

// Rotation amounts
static float xRot = 0.0f;
static float yRot = 0.0f;
static int opcao = '0';
static float grauSweep = 0;
static bool animando = false;
static int refresh_times = 0;
static float pointX = INT_MAX;
static float pointY = INT_MAX;
static int iColorDesc = 0;
static float stepRot = 1.0f;
static vector<float> rotations;
static vector<translation> translations;
static vector<ponto2D> pontos;

float radius = 1.0f;
float angle = 0.0f;
float minimizer = 0.2f;

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
    rotations.push_back(0);
}

// Respond to arrow keys
void SpecialKeys(int key, int x, int y){

    // Refresh the Window

}

void plotBezier(){

    vector<ponto2D> result = vector<ponto2D>();

    if(pontos.size() == 6){
        GLfloat t = 0;
        GLfloat step = 0.1;

        while( t <= (1 + step)){

            ponto2D aux;

            aux.x = pow(1 - t, 5) * pontos[0].x + \
                    5 * t * pow(1 - t, 4) * pontos[1].x + \
                    10 * t * t * pow(1 - t, 3) * pontos[2].x + \
                    10 * t * t * t * pow(1 - t, 2) * pontos[3].x + \
                    5 * t * t * t * t * (1 - t) * pontos[4].x + \
                    t * t * t * t * t * pontos[5].x;
            aux.y = pow(1 - t, 5) * pontos[0].y + \
                    5 * t * pow(1 - t, 4) * pontos[1].y + \
                    10 * t * t * pow(1 - t, 3) * pontos[2].y + \
                    10 * t * t * t * pow(1 - t, 2) * pontos[3].y + \
                    5 * t * t * t * t * (1 - t) * pontos[4].y + \
                    t * t * t * t * t * pontos[5].y;

            result.push_back(aux);
            t += step;
        }


        glBegin(GL_LINE_STRIP);
        for(int i = 0; i < result.size(); i++){
            glVertex2f(result[i].x, result[i].y);
        }
        glEnd();
    }

}

void rotate(){

    for(int i = 0; i < rotations.size(); i++){
        glPushMatrix();
            glRotatef(rotations[i], 0.0f, 1.0f, 0.0f);
            plotBezier();
        glPopMatrix();
    }
}

void translate(){

    float h = 0.25f;
    float total = 0;

    for(int i = 0; i < translations.size(); i++){
        while(total <= 1){
            glPushMatrix();
                glTranslatef(translations[i].x*h, translations[i].y*h, translations[i].z*h);
                rotate();
            glPopMatrix();
            total += h;
        }
    }

}

void OnDisplay(void){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(opcao == '1'){
        animando = true;
        opcao = 'n';
    }
    if(opcao == '0'){
        if(!animando){
            pontos.clear();
            rotations.clear();
            rotations.push_back(0);
        }
        opcao = 'n';
    }

    GLUquadricObj *pObj = gluNewQuadric();

    if(animando){
        glPushMatrix();

            glTranslatef(0.0f, .0f, -5.0f);

            glColor3f(1.0f, .0f, .0f);

            // plotBezier();
            rotations.push_back(yRot);
            rotate();

        glPopMatrix();

        yRot -= stepRot;
        if(yRot <= -360.0f){
            yRot = 0;
            animando = false;
        }
    }else{
        //glClear(GL_DEPTH_BUFFER_BIT);
        glPushMatrix();

            glColor3f(1.0f, .0f, .0f);

            glTranslatef(0.0f, .0f, -5.0f);

            if(pointX != INT_MAX){

                ponto2D aux;
                aux.x = pointX;
                aux.y = pointY;

                pontos.push_back(aux);

                while(pontos.size() > 6){
                    pontos.erase(pontos.begin());
                }

                pointX = INT_MAX;
                pointY = INT_MAX;
            }

            glBegin(GL_LINE_STRIP);
            for(int i = 0; i < pontos.size(); i++){
                glVertex2f(pontos[i].x , pontos[i].y);
            }
            glEnd();

            for(int i = 0; i < pontos.size(); i++){
                glTranslatef(pontos[i].x, pontos[i].y, .0f);
                gluSphere(pObj, radius*0.02f, 50, 25);
                glTranslatef(-pontos[i].x, -pontos[i].y, .0f);
            }

            rotate();

        glPopMatrix();
    }

    // Buffer swap
    glutSwapBuffers();

    glutPostRedisplay();
}

void drawPoint(int x, int y) {

	pointX = (x-400)*0.005f;
	pointY = (300-y)*0.005f;

}

void mouse(int bin, int state , int x , int y) {
	if(bin == GLUT_LEFT_BUTTON && state == GLUT_DOWN) drawPoint(x,y);
}

void keyboard(unsigned char key, int x, int y)
{
    // 'n' - nenhuma ação
    // '0'  - refresh
    // '1'  - sweep rotacional
    // '2'  - sweep translacional
	opcao = key;
    if(opcao == '0')
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
