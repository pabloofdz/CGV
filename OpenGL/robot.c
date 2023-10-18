#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define DEPTH -10.0f

GLint ancho=400;
GLint alto=400;
bool solid = true;
GLfloat cubeAngles[4][2][3] = {{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
                               {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
                               {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
                               {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}};
                               
int extremity = 0;
int velocidad = 100;
bool giroAuto = false;
int leg = 1;

//Función para que las piernas se muevan en función del movimiento del ratón
void motion(int x, int y){
  if (leg == 1){
    cubeAngles[2][0][1] = x;
    cubeAngles[2][0][0] = y;
    } else {
    cubeAngles[3][0][1] = x;
    cubeAngles[3][0][0] = y;
  }
}

//Función para seleccionar el movimiento de las piernas mediante el ratón y para cambiar entre una pierna y otra con CTRL
void mouse(int button, int state, int x, int y){
    if (button==GLUT_LEFT_BUTTON){
        if (glutGetModifiers()==GLUT_ACTIVE_CTRL) 
            leg = 2;
    else
        leg = 1;
    }
}

//Función que se encarga del giro automático de las piernas
void timerRotation(int value){
    if (giroAuto){
        cubeAngles[2][0][1] = cubeAngles[2][0][1] + 5.0f;
        cubeAngles[3][0][1] = cubeAngles[3][0][1] + 5.0f;
    }
    glutTimerFunc(velocidad, timerRotation, value); 
}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glOrtho(-20, 20, -20, 20, 1, 20);

    glMatrixMode(GL_MODELVIEW);
 
    ancho = width;
    alto = height;
}

void drawSphere(){
  glColor3f(0.0f, 1.0f, 0.0f);//Verde
  if (solid)
    glutSolidSphere(0.5f, 10, 10);
  else
    glutWireSphere(0.5f, 10, 10);
}

void drawCube(GLfloat cubeAngle[]){
   //Las operaciones de abajo permiten que el cubo rote alrededor de la esfera
    glRotatef(cubeAngle[0], 1.0f, 0.0f, 0.0f);
    glRotatef(cubeAngle[1], 0.0f, 1.0f, 0.0f);
    glRotatef(cubeAngle[2], 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, -1.5f, 0.0f);
    glPushMatrix();//Guardamos el estado actual de la pila de transformaciones para aplicar el escalado
    glScalef(1.0f, 2.0f, 1.0f);
    glColor3f(0.0f, 0.0f, 1.0f);//Azul
    
   if (solid)
     glutSolidCube(1.0f); 
   else
     glutWireCube(1.0f);
      
    glPopMatrix();//Recuperamos la matriz de transformaciones antes del escalado 
}

void drawTrunk(){
  glTranslatef(0.0f, 0.0f, DEPTH);
  glScalef(4.0f, 6.0f, 1.0f);
  glColor3f(0.0f, 1.0f, 0.0f);//Verde
  
  if (solid)
    glutSolidCube(1.0f); 
  else
    glutWireCube(1.0f);
    
  glLoadIdentity();
}

void drawExtremity(float x, float y, int n){
  glTranslatef(x, y, DEPTH);
  
  drawSphere(); 
  
  drawCube(cubeAngles[n][0]);
    
  glTranslatef(0.0f, -1.5f, 0.0f);
  drawSphere(); 

  drawCube(cubeAngles[n][1]);

  glLoadIdentity();
}

void display(){ 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  drawTrunk();

  drawExtremity(-2.5f, 2.5f, 0);

  drawExtremity(2.5f, 2.5f, 1);

  drawExtremity(-1.5f, -3.5f, 2);

  drawExtremity(1.5f, -3.5f, 3);

  glFlush();
  glutSwapBuffers();

}

void init()
{
    glClearColor(0,0,0,0);
    glEnable(GL_DEPTH_TEST);//Activamos el Z-Buffer 
    ancho = 400;
    alto = 400;
}
 

void idle()
{
    display();
}

//Para realizar las distintas rotaciones mediante el teclado
void keyboard(unsigned char key, int x, int y)
{
    static int actualAngle = 0; //Inicialmente, el ángulo actual es anguloCubo (el del primer cubo)

    switch (key) {
        case '1':
            extremity = 0;
            break;
        case '2':
            extremity = 1;
            break;
        case '3':
            extremity = 2;
            break;
        case '4':
            extremity = 3;
            break;
        case 'q':
        case 'Q':
            cubeAngles[extremity][actualAngle][0] += 5.0f;  
            break;
        case 'a':
        case 'A':
            cubeAngles[extremity][actualAngle][0] -= 5.0f;  
            break;
        case 'w':
        case 'W':
            cubeAngles[extremity][actualAngle][1] += 5.0f;  
            break;
        case 's':
        case 'S':
            cubeAngles[extremity][actualAngle][1] -= 5.0f;  
            break;
        case 'e':
        case 'E':
            cubeAngles[extremity][actualAngle][2] += 5.0f;  
            break;
        case 'd':
        case 'D':
            cubeAngles[extremity][actualAngle][2] -= 5.0f;  
            break;
        case 'r':
        case 'R'://Para cambiar el cubo que estamos rotando
            if (actualAngle == 0) {
                actualAngle = 1;//Cambiar al ángulo del cubo 2
            } else {
                actualAngle = 0;//Cambiar al ángulo del cubo 1
            }
            break;
        case 27://Escape
            exit(0);
            break;
    }
}

void menuVisualizacion(int id){
    switch (id){
    case 0:
        solid = true;
        break;

    case 1:
        solid = false;
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

void menu(int id){
    switch(id){
    case 0:
        giroAuto = !giroAuto;
        break;
    case 1:
        exit(-1);
        break;
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB| GLUT_DEPTH);//Activamos el Z-Buffer 
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(ancho, alto);
    glutCreateWindow("Robot");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutTimerFunc(velocidad, timerRotation, 0);
    glutKeyboardFunc(keyboard);
    glutMotionFunc(motion);
    glutMouseFunc(mouse);
    
    //Menú
    int submenuVisualizacion = glutCreateMenu(menuVisualizacion);
    glutAddMenuEntry("Objetos solidos", 0);
    glutAddMenuEntry("Objetos de alambre", 1);

    int submenuVelocidad = glutCreateMenu(menuVelocidad);
    glutAddMenuEntry("Lento", 0);
    glutAddMenuEntry("Medio", 1);
    glutAddMenuEntry("Rapido", 2);

    glutCreateMenu(menu);
    glutAddSubMenu("Visualizacion", submenuVisualizacion);
    glutAddMenuEntry("Activar giro automatico", 0);
    glutAddSubMenu("Velocidad", submenuVelocidad);
    glutAddMenuEntry("Salir", 1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();
    return 0;
}
