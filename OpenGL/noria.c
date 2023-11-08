#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

GLint ancho = 800;
GLint alto = 800;
int hazPerspectiva = 0;
float rotation = 5.0f;
int velocidad = 100;
GLfloat anguloNoria[3] = {0.0f, 0.0f, 0.0f};
GLfloat cubeAngleZ = 0.0f;
GLfloat color[3] = {0.0f, 255.0f, 0.0f};
bool giroAuto = true;
bool ejeX = true;

//Función para que la noria se mueva en función del movimiento del ratón
void motion(int x, int y){
  if (ejeX){
    anguloNoria[0] = y;
    } else {
    anguloNoria[1] = x;
  }
}

//Función para seleccionar el movimiento de las noria mediante el ratón y para cambiar entre el eje X y el Y con ALT
void mouse(int button, int state, int x, int y){
  if (button==GLUT_LEFT_BUTTON){
    if (glutGetModifiers()==GLUT_ACTIVE_ALT) 
      ejeX = false;
  else
    ejeX = true;
  }
}

//Función que se encarga del giro automático de la noria
void timerRotation(int value){
  if (giroAuto){
    float rot = rotation;
    if (fabs(cubeAngleZ) > 180){
      rot = 2*rotation;
    }
  cubeAngleZ = fmod((cubeAngleZ + rot),360);
  }
  glutTimerFunc(velocidad, timerRotation, value); 
}


void reshape(int width, int height)
{
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
    
  if(hazPerspectiva)
    gluPerspective(120.0f, (GLfloat)width/(GLfloat)height, 1.0f, 30.0f);
  else       
    glOrtho(-30, 30, -30, 30, 1, 30);

  glMatrixMode(GL_MODELVIEW);
 
  ancho = width;
  alto = height;
}

void drawCircle(float radio) {
  float ang;
  glBegin(GL_POLYGON);
  for (int i = 0; i < 360; i++) {
    ang = i*M_PI/180;
    glVertex3f(radio*cos(ang), 0.0f, radio*sin(ang));
  }
  glEnd();
}

void drawCylinder(float radio, float height){
  glPushMatrix();
  glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
  glColor3f(0.67f, 0.67f, 0.67f);
  GLUquadric* objCylinder = gluNewQuadric();
  gluCylinder(objCylinder, radio, radio, height, 30, 30);
  glPopMatrix();
  
  glColor3f(0.76f, 0.76f, 0.76f);
  drawCircle(radio);
  
  glPushMatrix();
  glTranslatef(0.0f, height, 0.0f);
  drawCircle(radio);
  glPopMatrix();

}

void drawTriangularPrism(float sideX, float height, float sideZ) {
  glColor3f(0.46f, 0.64f, 0.71f);
  glBegin(GL_TRIANGLES);       //cara frontal
  glVertex3f(-0.5*sideX, 0.0f, 0.5*sideZ);
  glVertex3f(0.0f, height, 0.5*sideZ);
  glVertex3f(0.5*sideX, 0.0f, 0.5*sideZ);
  glEnd();

  glBegin(GL_TRIANGLES);       //cara trasera
  glVertex3f(-0.5*sideX, 0.0f, -0.5*sideZ);
  glVertex3f(0.0f, height, -0.5*sideZ);
  glVertex3f(0.5*sideX, 0.0f, -0.5*sideZ);
  glEnd();

  glColor3f(0.58f, 0.8f, 0.8f);
  glBegin(GL_QUADS);       //cara lateral izq
 glVertex3f(-0.5*sideX, 0.0f, 0.5*sideZ);
  glVertex3f(0.0f, height, 0.5*sideZ);
  glVertex3f(0.0f, height, -0.5*sideZ);
  glVertex3f(-0.5*sideX, 0.0f, -0.5*sideZ);
  glEnd();

  glBegin(GL_QUADS);       //cara lateral dcha
  glVertex3f(0.5*sideX, 0.0f, 0.5*sideZ);
  glVertex3f(0.0, height, 0.5*sideZ);
  glVertex3f(0.0, height, -0.5*sideZ);
  glVertex3f(0.5*sideX, 0.0f, -0.5*sideZ);
  glEnd(); 
    
  glBegin(GL_QUADS);       //cara abajo
  glVertex3f(-0.5*sideX, 0.0f, 0.5*sideZ);
  glVertex3f(0.5*sideX, 0.0f, 0.5*sideZ);
  glVertex3f(0.5*sideX, 0.0f, -0.5*sideZ);
  glVertex3f(-0.5*sideX, 0.0f, -0.5*sideZ);
  glEnd();
}

void drawCabin(){
  glTranslatef(0.0f, -2.0f, 1.5f);
  glRotatef(-cubeAngleZ, 0.0f, 0.0f, 1.0f);
  glColor3f(0.67f, 0.67f, 0.67f);
  glPushMatrix();//Guardamos el estado actual de la pila de transformaciones para aplicar el escalado
  glScalef(2.0f, 1.0f, 2.0f);
  glutSolidCube(1.0f); 
  glPopMatrix();//Recuperamos la matriz de transformaciones antes del escalado
    
  //Pasajeros
  glColor3f(color[0], color[1], color[2]);

  glPushMatrix();
  glTranslatef(-0.5f, 0.75f, 0.5f);
  glutSolidCube(0.5f);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-0.5f, 0.75f, -0.5f);
  glutSolidCube(0.5f);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0.5f, 0.75f, 0.5f);
  glutSolidCube(0.5f);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0.5f, 0.75f, -0.5f);
  glutSolidCube(0.5f);
  glPopMatrix();
}

void drawArm(){
  //Las operaciones de abajo permiten que la noria gire
  glTranslatef(0.0f, 5.5f, 1.0f);
  glRotatef(cubeAngleZ, 0.0f, 0.0f, 1.0f);
  glTranslatef(0.0f, -2.0f, 0.0f);
  glPushMatrix();//Guardamos el estado actual de la pila de transformaciones para aplicar el escalado
  glScalef(1.0f, 4.0f, 1.0f);
  glColor3f(1.0f, 0.8f, 0.7f);
  glutSolidCube(1.0f); 
  glPopMatrix();//Recuperamos la matriz de transformaciones antes del escalado
}

void drawNoria(){
  glPushMatrix();
  drawCylinder(3.0f, 1.0f);
  glTranslatef(0.0f, 1.0f, 0.0f);
  drawTriangularPrism(5.0f, 4.5f, 1.0f);
  glPopMatrix();
  drawArm();
  drawCabin();
}

void display(){ 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  glTranslatef(0.0f, 0.0f, -15.0f);
  glRotatef(anguloNoria[0], 1.0f, 0.0f, 0.0f);
  glRotatef(anguloNoria[1], 0.0f, 1.0f, 0.0f);
  glRotatef(anguloNoria[2], 0.0f, 0.0f, 1.0f);
  drawNoria();

  glFlush();
  glutSwapBuffers();

}

void init(){
  glClearColor(0,0,0,0);
  glEnable(GL_DEPTH_TEST);
  ancho = 800;
  alto = 800;
}

void idle()
{
  display();
}

void keyboard(unsigned char key, int x, int y ){
  switch(key){
    case 'p':
    case 'P':
      hazPerspectiva=1;
      reshape(ancho,alto);
      break;
    case 'o':
    case 'O':
      hazPerspectiva=0;
      reshape(ancho,alto);
      break;
    //Invertir 
    case 'r':
    case 'R':
      rotation = -rotation;
      break;
    case 's':
    case 'S':
      if (giroAuto){
        giroAuto = false;
        cubeAngleZ = 0.0f;
      } else
        giroAuto = true;    
      break;
    case 'q':
    case 'Q':
      anguloNoria[1] = anguloNoria[1] + rotation;
      break;
    case 'a':
    case 'A':      
      anguloNoria[1] = anguloNoria[1] - rotation;
      break; 
    case 27://Escape
      exit(0);
      break;
  }
}

void menuVelocidad(int id){
  switch(id){
    case 0:
        velocidad = 100;
        break;
    case 1:
        velocidad = 50;
        break;
    case 2:
        velocidad = 10;
        break;
  }
}

void menuColor(int id){
  switch (id){
    case 0:
      color[0]=0.0;
      color[1]=255.0;
      color[2]=0.0;
      break;

    case 1:
      color[0]=255.0;
      color[1]=0.0;
      color[2]=0.0;
      break;

    case 2:
      color[0]=255.0;
      color[1]=255.0;
      color[2]=0.0;
      break;
    
    case 3:
      color[0]=0.0;
      color[1]=0.0;
      color[2]=255.0;
      break;
  }
}

void menu(int id){
  if (id==0)
    exit(-1);
}

void createMenu(){
  int submenuVelocidad = glutCreateMenu(menuVelocidad);
  glutAddMenuEntry("Lento", 0);
  glutAddMenuEntry("Normal", 1);
  glutAddMenuEntry("Rapido", 2);

  int submenuColor = glutCreateMenu(menuColor);
  glutAddMenuEntry("Verde", 0);
  glutAddMenuEntry("Rojo", 1);
  glutAddMenuEntry("Amarillo", 2);
  glutAddMenuEntry("Azul", 3);

  glutCreateMenu(menu);
  glutAddSubMenu("Velocidad", submenuVelocidad);
  glutAddSubMenu("Color pasajeros", submenuColor);
  glutAddMenuEntry("Salir", 0);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB| GLUT_DEPTH);//Activamos el Z-Buffer 
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(ancho, alto);
  glutCreateWindow("Noria");
  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);
  glutTimerFunc(velocidad, timerRotation, 0);
  glutKeyboardFunc(keyboard);
  glutMotionFunc(motion);
  glutMouseFunc(mouse);
  createMenu();
  glutMainLoop();
  return 0;
}
