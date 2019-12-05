#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

static int timer_active;

static void on_keyboard(unsigned char key, int x, int y);
static void on_timer(int value);
static void on_reshape(int width, int height);
static void on_display(void);

static void nacrtaj_put();
static void nacrtaj_pandu();

float r_telo = 0.7;

int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Feed the Panda");

    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    timer_active = 0;

    glClearColor(0.560, 0.658, 0.7411, 0); /*sa interneta boja*/
    glEnable(GL_DEPTH_TEST); 

    glutMainLoop();

    return 0;
}

static void on_keyboard(unsigned char key, int x, int y){
    switch (key) {
        case 27:
            exit(0);
            break;
    }
}

static void on_timer(int value){
 
    if (value != 0)
        return;

    glutPostRedisplay();

    if (timer_active)
        glutTimerFunc(50, on_timer, 0);
}

static void on_reshape(int width, int height){
    
    glViewport(0, 0, width, height);

    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,
                   (float) width / height,
                   1, 1500);
}

static void on_display(void){
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(2, 2, -5,
              2, 0,  2,
              0, 1,  0);

   /* glBegin(GL_LINES);
        glColor3f(1,0,0);
        glVertex3f(0,0,0);
        glVertex3f(100,0,0);
        glVertex3f(0,0,0);
        glVertex3f(-100,0,0);
        
        glColor3f(0,1,0);
        glVertex3f(0,0,0);
        glVertex3f(0,100,0);
        glVertex3f(0,0,0);
        glVertex3f(0,-100,0);

        glColor3f(0,0,1);
        glVertex3f(0,0,0);
        glVertex3f(0,0,100);
        glVertex3f(0,0,0);
        glVertex3f(0,0,-100);
    glEnd(); */
    
    nacrtaj_put();
    nacrtaj_pandu();

   
    glutSwapBuffers();
}

static void nacrtaj_put(){

    glPushMatrix();
        glColor3f(0.29, 0.447, 0.584); /*sa interneta boja*/
        glTranslatef(2, -0.100, 2);
        glScalef(10, 0.5, 20);
        glutSolidCube(1);
    glPopMatrix();
}

static void nacrtaj_pandu(){
    
    glPushMatrix(); /*telo pande*/
        glColor3f(0, 0, 0);
        glTranslatef(2, 0.5, 0);
        glScalef(1, 0.4, 1.8);
        glutSolidSphere(r_telo, 100, 100);
    glPopMatrix();
    
    float r_glava = 0.8 * r_telo;

    glPushMatrix(); /*glava pande*/
        glColor3f(1, 1, 1);
        glTranslatef(2, 0.5 + r_telo - 0.1, 0);
        glScalef(1, 0.5, 2);
        glutSolidSphere(r_glava, 100, 100);
    glPopMatrix();
    
    float r_uvo = 0.25 * r_telo;

    glPushMatrix(); /*jedno uvo*/
        glColor3f(0, 0, 0);
        glTranslatef(2 + r_glava - r_uvo, 1.35, 0);
        glutSolidSphere(r_uvo, 100, 100);
    glPopMatrix();

    glPushMatrix();  /*drugo uvo*/
        glColor3f(0, 0, 0);
        glTranslatef(2 - r_glava + r_uvo, 1.35, 0);	
        glutSolidSphere(r_uvo, 100, 100);
    glPopMatrix();
}
