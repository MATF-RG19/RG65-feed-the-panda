#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>
#define M_PI 3.14159265358979323846

static void on_keyboard(unsigned char key, int x, int y);
static void on_timer(int value);
static void on_reshape(int width, int height);
static void on_release(unsigned char key,int x,int y);
static void on_display(void);

static void nacrtaj_put();
static void nacrtaj_pandu();
static void lizalica();

static float animation_ongoing = 0;
static float r_telo = 0.7;
static float duzina = 100;
static int kretanja[] = {0, 0};

static float x_koordinata = 0;
static float y_koordinata = 0.75;
static float z_koordinata = 5;
static float x_ravni1 = 10;
static float y_ravni1 = 1;
static float z_ravni1 = 50;
static float x_ravni2 = 10;
static float y_ravni2 = 1;
static float z_ravni2 = 150;

typedef struct
{
    float x;
    float y;
    float z;
    int tip;
} Prepreka;


Prepreka prepreke1[50];
Prepreka prepreke2[50];
static int pos1;
static int pos2;

static void nacrtaj_prepreke(int tip);
static void postavi_prepreke(int tip);
static void postavi1();
static int prva = 1;


int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Feed the Panda");

    glutKeyboardFunc(on_keyboard);
    glutKeyboardUpFunc(on_release);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    glClearColor(255.0/255, 223.0/255, 211.0/255, 0); /*sa interneta boja*/
    glEnable(GL_DEPTH_TEST);    
    glEnable(GL_COLOR_MATERIAL);
    
    srand(time(NULL));
    glutFullScreen();

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
            kretanja[1] = 1;
            glutPostRedisplay();
            break;
        case 'a':
        case 'A':
            kretanja[0]= 1;
            glutPostRedisplay();
            break;
        
            
    
    }
}

static void on_release(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'a':
    case 'A':
        kretanja[0] -= 1;
        break;
    case 'd':
    case 'D':
        kretanja[1] -= 1;
        break;
    }
}

static void on_timer(int value){
 
    if (value != 0)
        return;
    
    z_ravni1-= 0.45;
    z_ravni2-= 0.45;
    int i;
    for ( i = 0; i < pos1; i++)
        prepreke1[i].z -= 0.45;

    for ( i = 0; i < pos2; i++)
        prepreke2[i].z -= 0.45;
   
    if(kretanja[0] && x_koordinata < 4.2)
        x_koordinata += 0.3;
    
    if(kretanja[1] && x_koordinata > -4.2)
        x_koordinata -= 0.3;
     
    if(z_ravni1 + 50 <= 0){
        z_ravni1 = 150;
        postavi_prepreke(1);
    }
    
    if(z_ravni2 + 50 < 0){
        z_ravni2 = 150;
        postavi_prepreke(2);
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
                   1, 150);
}

static void on_display(void){
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(x_koordinata, y_koordinata + 5, -5 + z_koordinata,
              x_koordinata, y_koordinata,  9 + z_koordinata,
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
        glTranslatef(x_koordinata,r_telo,z_koordinata);
        nacrtaj_pandu();
    glPopMatrix();
    
      if (prva){
        postavi1();
        postavi_prepreke(2);
        prva = 0;
    }

    nacrtaj_prepreke(1);
    nacrtaj_prepreke(2);
   
    glutSwapBuffers();
}

static void nacrtaj_put(){
    
    glPushMatrix();
        glColor3f(254.0/255,200.0/255,216.0/255); /*sa interneta boja*/
        glTranslatef(0, -y_ravni1/2, z_ravni1);
        glScalef(x_ravni1, y_ravni1/2, duzina);
        glutSolidCube(1);
    glPopMatrix();
    
     glPushMatrix();
        glColor3f(254.0/255,200.0/255,216.0/255); /*sa interneta boja*/
        glTranslatef(0, -y_ravni2/2, z_ravni2);
        glScalef(x_ravni2, y_ravni2/2, duzina);
        glutSolidCube(1);
    glPopMatrix();

}

static void nacrtaj_pandu(){
    
    glPushMatrix(); /*telo pande*/
        glColor3f(0, 0, 0.0001);
        glScalef(1.1, 0.4, 0.9);
        glutSolidSphere(r_telo, 100, 100);
    glPopMatrix();
    
    float r_glava = 0.8 * r_telo;

    glPushMatrix(); 
        glColor3f(1, 1, 1);
        glTranslatef(0, 0.5 + r_telo - 0.3, 0);
        glScalef(1, 0.5, 1);
        glutSolidSphere(r_glava, 100, 100);
    glPopMatrix();
    
    float r_uvo = 0.25 * r_telo;

    glPushMatrix(); 
        glColor3f(0, 0, 0);
        glTranslatef(-0.35, 1.25, 0);
        glutSolidSphere(r_uvo, 100, 100);
    glPopMatrix();

    glPushMatrix();  
        glColor3f(0, 0, 0);
        glTranslatef(0.35, 1.25, 0);	
        glutSolidSphere(r_uvo, 100, 100);
    glPopMatrix(); 
}
static void postavi_prepreke(int tip)
{
    if (tip == 1)
        pos1 = 0;
    else
        pos2 = 0;

  int i,j;
    for ( i = 0; i < 10; i++){
        int num = (int)rand() % 5;
        if (num == 0)
            num = 2;
        int hrana = 0;
        int free_positions[] = {1, 1, 1, 1, 1};
        for ( j = 0; j < num; j++){
            Prepreka p;
         
            int positions[] = {4, 2, 0, -2, -4};
            int pos = (int)rand() % 5;
            if (free_positions[pos] == 1){
                free_positions[pos] = 0;
              
                int t = (int)rand() % 2;
                if (t == 0 && !hrana){
                    p.tip = 0;
                    p.y = 0.5;
                    hrana = 1;
                }
                else{
                    p.tip = 1;
                    p.y = 0.75;
                }
                p.x = positions[pos];
                if (tip == 1){
                    p.z = z_ravni1 + 50 - i * 10;
                    prepreke1[pos1++] = p;
                }
                else {
                    p.z = z_ravni2 + 50 - i * 10;
                    prepreke2[pos2++] = p;
                }
            }
        }
    }
}


static void postavi1()
{
    int i, j;
    for ( i = 0; i <= 2; i++){
        int num = (int)rand() % 5;
        if (num < 4)
            num++;
        int hrana = 0;
        int free_positions[] = {0, 0, 0, 0, 0};
        for (j = 0; j < num; j++){
            Prepreka p;
            int positions[] = {4, 2, 0, -2, -4};
            int pos = (int)rand() % 5;
            if (free_positions[pos] == 0){
                free_positions[pos] = 1;
                int t = (int)rand() % 2;
                if (t == 0 && !hrana){
                    p.tip = 0;
                    p.y = 0.5;
                    hrana = 1;
                }
                else{
                    p.tip = 1;
                    p.y = 0.75;
                }
                p.x = positions[pos];
                p.z = z_ravni1+ 50 - i * 20;
                prepreke1[pos1++] = p;
            }
        }
    }
}


static void nacrtaj_prepreke(int tip)
{
    int len = 0;
    if (tip == 1)
        len = pos1;
    else
        len = pos2;
    int i;
    for (i = 0; i < len; i++){
        Prepreka p;
        if (tip == 1)
            p = prepreke1[i];
        else
            p = prepreke2[i];

        if (p.tip == 0){
            glPushMatrix();
            glColor3f(0.4,.1,.1);
            glTranslatef(p.x, p.y, p.z);
            glScalef(1, 1, 1);
            lizalica();
            glPopMatrix();
        }
        else {
            glPushMatrix();
            glColor3f(210.0/255,145.0/255,188.0/255);
            glTranslatef(p.x, p.y, p.z);
            glScalef(1, 1, 1);
            glutSolidCube(1);
            glPopMatrix();
        }
    }
}

static void lizalica(){
    glPushMatrix();
        glColor3f(1,1,1);
        glScalef(.5,2,0);
        glutSolidCube(0.3);
    glPopMatrix();
        
    GLUquadricObj* quadratic;
    quadratic = gluNewQuadric();
    gluQuadricNormals(quadratic, GLU_SMOOTH);
        
        glPushMatrix();
            glTranslatef(0,.8,0);
            
            glPushMatrix();
                glColor3f(224.0/255, 254.0/255, 254.0/255);
                gluDisk(quadratic,0,.05,32,32);
            glPopMatrix();
            
            glPushMatrix();
                glColor3f(1,250.0/255,129.0/255);
                gluDisk(quadratic,.05,.1,32,32);
            glPopMatrix();
            
            glPushMatrix();
                glColor3f(254.0/255,235.0/255,201.0/255);
                gluDisk(quadratic,.1,.2,32,32);
            glPopMatrix();
            
            glPushMatrix();
                glColor3f(1,1,1);
                gluDisk(quadratic,.2,.3,32,32);
            glPopMatrix();
            
            glPushMatrix();
                glColor3f(191.0/255,213.0/255,232.0/255);
                gluDisk(quadratic,.3,.4,32,32);
            glPopMatrix();
            
            glPushMatrix();
                glColor3f(251.0/255,182.0/255,209.0/255);
                gluDisk(quadratic,.4,.5,32,32);
                glPopMatrix();
    
        glPopMatrix();
}
