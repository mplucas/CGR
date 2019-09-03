//gcc castelo.c -framework GLUT -framework OpenGL -framework Cocoa -lm -o castelo && ./castelo
// gcc castelo.c -lglut -lGL -lGLU -lm -o castelo && ./castelo
//#include <GL/glut.h> // para linux
//#include <GLUT/glut.h> // para macos
//#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

float raio = 1.0f;
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
float angle = 0.0f;

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
    glClearColor(0.0f, 0.7f, 1.0f, 1.0f);

	}

// Respond to arrow keys
void SpecialKeys(int key, int x, int y){

    if(key == GLUT_KEY_LEFT)
        yRot -= 5.0f;

    if(key == GLUT_KEY_RIGHT)
        yRot += 5.0f;


    if(key == GLUT_KEY_UP)
        xRot -= 5.0f;

    if(key == GLUT_KEY_DOWN)
        xRot += 5.0f;


    yRot = (GLfloat)((const int)yRot % 360);
    xRot = (GLfloat)((const int)xRot % 360);
    // Refresh the Window
    glutPostRedisplay();
}



void MakeCentralTower(GLUquadricObj *t,float x,float y ,float z){

  glTranslatef(x , y , z);

  glPushMatrix(); // Chapeu
  glTranslatef(0, 3 , 0);
  glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(t, raio*0.40f, 0, raio/2, 4, 25);
	glPopMatrix(); // Chapeu

  //glTranslatef(x , y, z);


	glPushMatrix(); // Base
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(t, raio*0.40f, raio*0.40f, 3*raio, 4, 25);
	glPopMatrix();	// Base

}

void MakeWall(GLUquadricObj *t,float x,float y ,float z, float a,float b){

  glPushMatrix(); // Parede para frente direita
	glTranslatef(x, y, z);


  if(b == 1){
    glRotatef(a, 1.0f , 0.0f, 0.0f);
  }
  if(b == 2){
    glRotatef(a, 0.0f , 1.0f, 0.0f);
  }
  if(b == 3){
  glRotatef(a, 0.0f , 0.0f, 1.0f);
  }

  glRotatef(-45 , 0.0f, 0.0f, 1.0f);

	gluCylinder(t, raio*0.40f, raio*0.40f, raio*3, 4, 25);
	glPopMatrix();	// Parede para frente direita

}

void MakeTower(GLUquadricObj *t,float x,float y ,float z){


  glTranslatef(x , y , z);

  glPushMatrix(); // Chapeu
  glTranslatef(0, 2 , 0);
  glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(t, raio*0.40f, 0, raio/2, 25, 2);
	glPopMatrix(); // Chapeu

  //glTranslatef(x , y, z);


	glPushMatrix(); // Base
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(t, raio*0.40f, raio*0.40f, 2*raio, 25, 2);
	glPopMatrix();	// Base


}
void OnDisplay(){

  GLUquadricObj *castelo = gluNewQuadric();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // +1
  glPushMatrix();


  glTranslatef(0.0f, -1.0f, -20.0f);
  glRotatef(xRot, 1.0f, 0.0f, 0.0f);
  glRotatef(yRot, 0.0f, 1.0f, 0.0f);


  glPushMatrix();
  glColor3f(1.0f, 0, 0);

  MakeTower(castelo,-raio*3/2 ,0.0f ,0.0f);

  MakeWall(castelo, 0.0f, raio/4,0.0f,90.0f,2.0f);

  MakeTower(castelo, +raio*3 ,0.0f ,0.0f);

  MakeWall(castelo, 0.0f, raio/4,0.0f,180.0f,2.0f);

  MakeTower(castelo,0 ,0.0f , - raio*3);

  MakeWall(castelo, 0.0f, raio/4,0.0f, -90.0f,2.0f);

  MakeTower(castelo,- raio*3 , 0 , 0);

  MakeWall(castelo, 0.0f, raio/4,0.0f, 0,2.0f);

  MakeCentralTower(castelo, raio*3/2,0,raio*3/2);

  glPopMatrix();
  glPopMatrix();
  // Buffer swap
  glutSwapBuffers();

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
