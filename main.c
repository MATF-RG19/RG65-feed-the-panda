#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

static void on_keyboard(unsigned char key, int x, int y);
static void on_timer(int value);
static void on_reshape(int width, int height);

/*static void on_release(unsigned char key,int x,int y);*/
static void on_display(void);

static void nacrtaj_put();
static void nacrtaj_pandu();


float animation_parameter = 0;
float animation_ongoing = 0;
int kretanje_levo, kretanje_desno;


float r_telo = 0.7;
float x_koordinata = 0;
float y_koordinata = 0;
float z_koordinata = 10;
float x_ravni1 = 10;
float y_ravni1 = 1;
float z_ravni1 = 100;
float x_ravni2 = 10;
float y_ravni2 = 1;
float z_ravni2 = 200;
float duzina = 100;



int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Feed the Panda");

    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    glClearColor(0.560, 0.658, 0.7411, 0); /*sa interneta boja*/
    glEnable(GL_DEPTH_TEST);
    
     GLfloat light_position[] = { 2, 5, 20, 0};
    
    GLfloat light_ambient[] = { 0, 0, 0, 1 };

    GLfloat light_diffuse[] = { 1, 1, 1, 1 };

    GLfloat light_specular[] = { 1, 1, 1, 1 };

    GLfloat model_ambient[] = { 0.4, 0.4, 0.4, 1 };
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);
    
    glEnable(GL_COLOR_MATERIAL);

    glutMainLoop();

    return 0;
}

static void on_keyboard(unsigned char key, int x, int y){
    switch (key) {
        case 27:
            exit(0);
            break;
        case 'g':
        case 'G':
            if(!animation_ongoing){
                animation_ongoing = 1;
                glutTimerFunc(20, on_timer, 0);
            }
            break;
        case 'd':
        case 'D':
                x_koordinata-=1;
                glutPostRedisplay();
            break;
        case 'a':
        case 'A':
            x_koordinata+= 1;
            glutPostRedisplay();
            break;
        
            
    
    }
}


static void on_timer(int value){
 
    if (value != 0)
        return;
    
    z_ravni1-= 0.5;
    z_ravni2-= 0.5;
    
    if(z_ravni1 + 50 < 0){
        z_ravni1 = 200;
    }
    
    if(z_ravni2 + 50 < 0){
        z_ravni2 = 200;
    }
    

    glutPostRedisplay();

    if (animation_ongoing)
        glutTimerFunc(20, on_timer, 0);
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
    gluLookAt(x_koordinata, y_koordinata + 3, -5 + z_koordinata,
              x_koordinata, y_koordinata,  7 + z_koordinata,
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
    
    
    glPushMatrix();
        nacrtaj_put();
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(1,1,1);
    glutSolidSphere(0.5,50,50);
    glPopMatrix();
    
    glPushMatrix();
         
         glTranslatef(x_koordinata,r_telo,z_koordinata+2);
        nacrtaj_pandu();
    glPopMatrix();

   
    glutSwapBuffers();
}

static void nacrtaj_put(){
    
     glPushMatrix();
        glColor3f(0.29, 0.447, 0.584); /*sa interneta boja*/
        glTranslatef(0, -y_ravni2/2, z_ravni2/2 + duzina/2);
        glScalef(x_ravni2, -y_ravni2/2, duzina);
        glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
        glColor3f(0.29, 0.447, 0.584); /*sa interneta boja*/
        glTranslatef(0, -y_ravni1/2, z_ravni1/2);
        glScalef(x_ravni1, -y_ravni1/2, duzina);
        glutSolidCube(1);
    glPopMatrix();
}

static void nacrtaj_pandu(){
    
    glPushMatrix(); /*telo pande*/
        glColor3f(0, 0, 0.0001);
        glTranslatef(2, 0.5, 0);
        glScalef(1, 0.4, 1);
        glutSolidSphere(r_telo, 100, 100);
    glPopMatrix();
    
    float r_glava = 0.8 * r_telo;

    glPushMatrix(); /*glava pande*/
        glColor3f(1, 1, 1);
        glTranslatef(2, 0.5 + r_telo - 0.1, 0);
        glScalef(1, 0.5, 1);
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
