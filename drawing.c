#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "image.h"

#include "drawing.h"
#define M_PI 3.14159265358979323846

#define FILENAME0 "clouds.bmp"
#define FILENAME1 "pozadina.bmp"
#define FILENAME2 "gameover.bmp"
#define FILENAME3 "startgame.bmp"

float r_telo = 0.7;

void nacrtaj_pandu(){
    
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

void nacrtaj_lizalicu(){
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

void nacrtaj_bombonu(){

    glPushMatrix();
        glScalef(.4,.4,.4);
        glPushMatrix();
        glColor3f(1,250.0/255,129.0/255);
        glScalef(.7,.5,.5);
        glTranslatef(-2,0,0);
        glutSolidTetrahedron();
        glPopMatrix();
        glPushMatrix();
        glColor3f(1,1,1);
        glScalef(1,.8,.8);
        glutSolidIcosahedron();
        glPopMatrix();
        glPushMatrix();
        glColor3f(1,250.0/255,129.0/255);
        glScalef(.7,.5,.5);
        glTranslatef(2,0,0);
        glRotatef(180,0,1,0);
        glutSolidTetrahedron();
        glPopMatrix();
    glPopMatrix();
    
    
    
}
