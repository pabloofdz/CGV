#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

GLint ancho = 800;
GLint alto = 800;
int hazPerspectiva = 0;
float rotation = 1.0f;
float rotation2 = 1.0f;
int velocidad = 100;
GLfloat anguloCabina = 0.0f;
GLfloat anguloTazas[3] = {0.0f, 0.0f, 0.0f};
GLfloat cubeAngleZ = 90.0f;
GLfloat color[3] = {0.0f, 255.0f, 0.0f};
bool giroAuto = true;
bool atraccion = true;
bool inicio = true;
void motion(int x, int y){
  if (atraccion){
    anguloTazas[0] = y;
    } else {
    cubeAngleZ = y;
  }
}

void mouse(int button, int state, int x, int y){
  if (button==GLUT_LEFT_BUTTON){
    if (glutGetModifiers()==GLUT_ACTIVE_ALT) 
      atraccion = true;
  else
    atraccion = false;
  }
}

//Función que se encarga del movimiento de la taza
void timerRotation(int value){
  if (giroAuto){
    anguloTazas[1] = anguloTazas[1]+rotation;
    if (cubeAngleZ == 135.0f){
      rotation2 = -rotation2;
      inicio = false;
    }else if (cubeAngleZ == 90.0f && inicio == false){
      rotation2 = -rotation2;
    }
    cubeAngleZ = cubeAngleZ + rotation2;
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

void drawCabin(){
  glTranslatef(0.0f, -2.0f, 0.0f);
  glRotatef(-cubeAngleZ, 0.0f, 0.0f, 1.0f);
  glColor3f(0.67f, 0.67f, 0.67f);
  glPushMatrix();//Guardamos el estado actual de la pila de transformaciones para aplicar el escalado
  glTranslatef(0.0f, 0.5f, 0.0f);
  glRotatef(90,1,0,0);
  glutSolidCone(1.5,0.5,16,16);
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
  glTranslatef(0.0f, 5.5f, 0.0f);
  glRotatef(cubeAngleZ, 0.0f, 0.0f, 1.0f);
  glTranslatef(0.0f, -2.0f, 0.0f);
  glPushMatrix();//Guardamos el estado actual de la pila de transformaciones para aplicar el escalado
  glScalef(1.0f, 4.0f, 1.0f);
  glColor3f(1.0f, 0.8f, 0.7f);
  glutSolidCube(1.0f); 
  glPopMatrix();//Recuperamos la matriz de transformaciones antes del escalado
}

void drawTazas(){
  glPushMatrix();
  drawCylinder(3.0f, 1.0f);
  glTranslatef(0.0f, 1.0f, 0.0f);
  glRotatef(-90, 1, 0, 0);
  glColor3f(0.46f, 0.64f, 0.71f);
  glutSolidCone(2.5f, 4.5f, 16, 16);
  glPopMatrix();
  drawArm();
  drawCabin();
}

void display(){ 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glTranslatef(0.0f, 0.0f, -15.0f);
  glRotatef(anguloTazas[0], 1.0f, 0.0f, 0.0f);
  glRotatef(anguloTazas[1], 0.0f, 1.0f, 0.0f);
  glRotatef(anguloTazas[2], 0.0f, 0.0f, 1.0f);
  drawTazas();

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
        cubeAngleZ = 90.0f;
      } else
        giroAuto = true;    
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
  glutCreateWindow("Tazas");
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
