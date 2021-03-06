#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "image.h"
#include "drawing.h"

#define M_PI 3.14159265358979323846

/*Teksture koje cemo koristiti*/

#define FILENAME0 "clouds.bmp"
#define FILENAME1 "pozadina.bmp"
#define FILENAME2 "gameover.bmp"
#define FILENAME3 "startgame.bmp"


static void on_keyboard(unsigned char key, int x, int y);
static void on_timer(int value);
static void on_reshape(int width, int height);
static void on_release(unsigned char key,int x,int y);
static void on_display(void);

/*Niz u koji smestamo teksture*/

static GLuint names[4];


/*Deklaracije koriscenih funkcija*/

static void nacrtaj_put();

static void game_over();
static void startgame();

static float udaljenost();
static void kolizija();

/*Promenljive koje sluze za kretanje i uvecavanje pande*/
static float r_telo = 0.7;
static int kretanja[] = {0, 0};
static float brzina = 0.5;
static float panda_param = 1;
static float x_koordinata = 0;
static float y_koordinata = 0.5;
static float z_koordinata = 5;

/*Promenljive koje sluze za iscrtavanje puta*/
static float duzina = 100;
static float x_ravni1 = 15;
static float y_ravni1 = 1;
static float z_ravni1 = 50;
static float x_ravni2 = 15;
static float y_ravni2 = 1;
static float z_ravni2 = 150;

/*Promenljive koje sluze za kontrolisanje igre, da li je u toku, koji je nivo i koji score*/
static int score = 0;
static int nivo = 1;
static int start = 0;

/*Struktura koja predstavlja jednu prepreku*/
typedef struct{
    /*X,Y,Z koordinata prepreke*/
    float x;
    float y;
    float z;
    /* Tip1 odredjuje da li je crtamo prepreku ili hranu, tip2 odredjuje podvrstu prepreke/hrane*/
    int tip1;
    int tip2;
} Prepreka;

/*Nizovi koji sadrze prepreke za svaku od ravni*/
Prepreka prepreke1[110];
Prepreka prepreke2[110];

/*Pomocne promenljive koje koristimo pri postavljanju prepreka*/
static int prva_poz;
static int druga_poz;

/*Promenljiva koja nam govori da li smo na pocetku, kada postavljamo prepreke samo na kraju prve ravni i jednom delu druge*/
static int prva = 1;

/*Deklaracija funkcija kojim generisemo prepreke*/
static void nacrtaj_prepreke(int tip);
static void postavi_prepreke(int tip);
static void postavi1();


int main(int argc, char **argv){
    
    /*Inicijalizacija*/
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Feed the Panda");
    
    /*Objekat koji predstavlja teskturu ucitanu iz fajla*/
    Image * image;

    /*Ukljucuju se teksture.*/
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV,
              GL_TEXTURE_ENV_MODE,
              GL_REPLACE);

    /*
     *Inicijalizuje se objekat koji ce sadrzati teksture ucitane iz
     *fajla
     */
    image = image_init(0, 0);
      /*Generisu se identifikatori tekstura*/
    glGenTextures(4, names);


    /*Kreira se prva tekstura*/
    image_read(image, FILENAME0);
    
    glBindTexture(GL_TEXTURE_2D, names[0]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    
    /*Kreira se druga tekstura*/
    image_read(image, FILENAME1);

    glBindTexture(GL_TEXTURE_2D, names[1]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    
    /*Kreira se treca tekstura*/
    image_read(image, FILENAME2);

    glBindTexture(GL_TEXTURE_2D, names[2]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    
    /*Kreira se cetvrta tekstura*/
    image_read(image, FILENAME3);
  
    glBindTexture(GL_TEXTURE_2D, names[3]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    
    /*Iskljucujemo aktivnu teksturu*/
    glBindTexture(GL_TEXTURE_2D, 0);

    /*Unistava se objekat za citanje tekstura iz fajla*/
    image_done(image);

    /*Pozivamo funkcije*/
    glutKeyboardFunc(on_keyboard);
    glutKeyboardUpFunc(on_release);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(startgame);
    
    glClearColor(255.0/255, 223.0/255, 211.0/255, 0); /*sa interneta boja*/
    
    glEnable(GL_DEPTH_TEST);    
    glEnable(GL_COLOR_MATERIAL);
    
    /*Postavljamo 'sito' kako bismo kasnije koristili rand za generisanje prepreka*/
    srand(time(NULL));
    
    /*Igrica je u fullscreen mode-u*/
    glutFullScreen();


    glutMainLoop();

    return 0;
}

static void on_keyboard(unsigned char key, int x, int y){
    
    switch (key) {
        /*Na esc dugme izlazi se iz igrice*/
        case 27:
            exit(0);
            break;
        /*Na g/G se pokrece igrica*/
        case 'g':
        case 'G':
            if(!start){
                start = 1;
                glutDisplayFunc(on_display);
                glutTimerFunc(20, on_timer, 0);
            }
            break;
        /*Na d/D panda se krece u desno*/
        case 'd':
        case 'D':
            kretanja[1] = 1;
            glutPostRedisplay();
            break;
        /*Na a/A panda se krece u levo*/
        case 'a':
        case 'A':
            kretanja[0] = 1;
            glutPostRedisplay();
            break;
        /*Na r/R ponovo se pokrece igra*/
        case 'r':
        case 'R':
            score = 0;
            start = 0;
            x_koordinata = 0;
            y_koordinata = 0.5;
            z_koordinata = 5;
            x_ravni1 = 15;
            y_ravni1 = 1;
            z_ravni1 = 50;
            x_ravni2 = 15;
            y_ravni2 = 1;
            z_ravni2 = 150;
            prva = 1;
            nivo = 0;
            prva_poz = 0;
            druga_poz = 0;
            brzina = 0.5;
            panda_param = 1;
            glutDisplayFunc(on_display);
            glutPostRedisplay();
            break;
    }
}

static void on_release(unsigned char key, int x, int y){
    
    switch (key){
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
 
    if(value != 0)
        return;

    /*Na svakih 100 poena povecava se brzina i panda*/
    if(score == nivo*100 || score == nivo*100 + 5){
	
        nivo++;
    	brzina = brzina + (nivo-1)*0.03;
        panda_param+=0.1;
    }
    
    /*Naizmenivno postavljamo prvu i drugu ravan*/
    if(z_ravni1 <= -50){
    
        z_ravni1 = 150;
        postavi_prepreke(1);
    }
    
    if(z_ravni2 <= -50){
        
        z_ravni2 = 150;
        postavi_prepreke(2);
    }
    
    /*Pomeramo ravni i prepreke za dozivljaj kretanja*/
    z_ravni1-= brzina;
    z_ravni2 -= brzina;
    
    int i;
    for ( i = 0; i < prva_poz; i++)
        prepreke1[i].z -= brzina;

    for ( i = 0; i < druga_poz; i++)
        prepreke2[i].z -= brzina;
   
    /*U zavisnosti od toga da li se treba kretati levo ili desno, menjamo x koordinatu panda*/
    /*Postavljeno ogranicenje kako se ne bi kretala van staze*/
    
    if(kretanja[0] && x_koordinata < 6)
        x_koordinata += 0.3;
    
    if(kretanja[1] && x_koordinata > -6)
        x_koordinata -= 0.3;
     
    /*Pozivamo funkciju koja razresava koliziju*/
    kolizija();
    
    glutPostRedisplay();

    if (start)
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

    gluLookAt(x_koordinata, y_koordinata + 7, -6 + z_koordinata,
              x_koordinata, y_koordinata,  7 + z_koordinata,
              0, 1,  0);
    
    /*Na ekranu iscrtavamo SCORE: koji prikazuje trenutni score pande*/
    glPushMatrix();
        glColor3f(0,0,0);
        glRasterPos3f(x_koordinata + 5, y_koordinata + 5, 5);
    
        char score_display[50] = "SCORE: ";
        char score_value[50];
    
        sprintf(score_value, " %d ", score);
        strcat(score_display, score_value);
        int len = (int)strlen(score_display);
        int i;
        for (i = 0; i < len; i++){
            glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, score_display[i]);
        }
    
    glPopMatrix();
    
    /*Postavljamo pozadinu*/
    glPushMatrix();
        glRotatef(110,1,0,0);
        glTranslatef(x_koordinata-2,y_koordinata+50,z_koordinata);
        glScalef(7,1,15);
  
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, names[1]);
	
		glBegin(GL_POLYGON);
			glTexCoord2f(0, 0);
			glVertex3f(-10, 0, -5);
			
			glTexCoord2f(1, 0);
			glVertex3f(10, 0, -5);
			
			glTexCoord2f(1, 3);
			glVertex3f(10, 0, 5);
			
			glTexCoord2f(0, 3);
			glVertex3f(-10, 0, 5);
		glEnd();
	
        glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();

    /*Koordinatni sistem, pomoc pri orijentaciji*/
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
    
    /*Iscrtavamo put*/
    glPushMatrix();
        nacrtaj_put();
    glPopMatrix();

    
    /*Crtamo pandu na odgovarajucoj poziciji*/
    glPushMatrix();
        glTranslatef(x_koordinata,r_telo,z_koordinata);
        glScalef(panda_param,panda_param,panda_param);
        nacrtaj_pandu();
    glPopMatrix();
    
    /*Postavljamo prepreke. Ukoliko smo na pocetku igre, postavljamo ih samo na deo u 'daljini'*/
    
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
    
    /*Iscrtavamo jednu ravan i postavljamo njene teksture*/
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, names[0]);
        glEnable(GL_TEXTURE_2D);

        glBegin(GL_POLYGON);
			glTexCoord2f(0, 0);
			glVertex3f(-7, 0, -30);
			
			glTexCoord2f(1, 0);
			glVertex3f(7, 0, -30);
			
			glTexCoord2f(1, 1);
			glVertex3f(7, 0, 30);
			
			glTexCoord2f(0, 1);
			glVertex3f(-7, 0, 30);
		glEnd();
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
      
        glTranslatef(0, -y_ravni1/2, z_ravni1);
        glScalef(x_ravni1, y_ravni1/2, duzina);
        glutSolidCube(1);
     glPopMatrix();
    
     /*Iscrtavamo drugu ravan i postavljamo njene teksture*/
     glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, names[0]);
      
        glEnable(GL_TEXTURE_2D);

     	glBegin(GL_POLYGON);
			glTexCoord2f(0, 0);
			glVertex3f(-7, 0, 30);
			
			glTexCoord2f(1, 0);
			glVertex3f(7, 0, 30);
			
			glTexCoord2f(1, 1);
			glVertex3f(7, 0, 60);
			
			glTexCoord2f(0, 1);
			glVertex3f(-7, 0, 60);
		glEnd();

        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        
        glTranslatef(0, -y_ravni2/2, z_ravni2);
        glScalef(x_ravni2, y_ravni2/2, duzina);
        glutSolidCube(1);
    glPopMatrix();

}

static void postavi_prepreke(int tip){
    
    /*Prosledjena promenljiva tip odredjuje na kojoj ravni postavljamo prepreke*/
    if (tip == 1)
        prva_poz = 0;
    else
        druga_poz = 0;

    /*Delimo ravan na 12 traka*/
    int i,j;
    for ( i = 0; i < 12; i++){
        
        /*Svaku traku delimo na 7 delova. Generisemo random broj koji odredjuje koliko ce
        prepreka biti u jednoj traci. Ukoliko je taj broj 0, postavljamo ga na 2 da ne bi bilo prazno*/
        
        int broj_pr = (int)rand() % 7;
        if(broj_pr == 0)
        broj_pr = 2;
       
        /*Pomocni niz koji odredjuje da li je slobodna zeljena pozicija u traci*/
        int moze_prepreka[] = {1, 1, 1, 1, 1, 1, 1};
        
        for ( j = 0; j < broj_pr; j++){
            Prepreka p;
            
            /*Delimo traku na 7 delova*/          
            int pozicije[] = {-6, -4, -2, 0, 2, 4, 6};
            
            /*Biramo poziciju*/
            int pos = (int)rand() % 7;
            
            /*Ukoliko je data pozicija slobodna, zauzimamo je i postavljamo prepreku na nju*/
            if (moze_prepreka[pos] == 1){
                moze_prepreka[pos] = 0;
              
                int t = (int)rand() % 2; /*Promenljivu t koristimo da izaberemo da li cemo postaviti prepreku ili hranu*/
                int k = (int)rand() % 2; /*Kada smo izabrali hrana/prepreka, biramo podvrstu*/
                
                /*Za nasu prepreku p postavljamo vrednosti tip1 i tip2 na osnovu onoga sto smo generisali za k i t*/
                if (t == 0){
                    if(k == 0)
                        p.tip2 = 0; 
                    else
                        p.tip2 = 1;
                
                    p.tip1 = 0;
                    p.y = 0.5;
                  
                }
                else{
                    if(k ==0)
                        p.tip2 = 0;
                    else
                        p.tip2 = 1;
                    p.tip1 = 1;
                    p.y = 0.75;
                }
                p.x = pozicije[pos];
                
                /*U zavisnosti od toga na kojoj smo ravni, biramo odgovarajucu z koordinatu za prepreku*/
                if (tip == 1){
                    p.z = z_ravni1 + 50 - i * 8;
                    /*stavljamo napravljenu prepreku u odgovarajuci niz prepreka i povecavamo brojac*/
                    prepreke1[prva_poz++] = p; 
                }
                else {
                    p.z = z_ravni2 + 50 - i * 8;
                    /*stavljamo napravljenu prepreku u odgovarajuci niz prepreka i povecavamo brojac*/
                    prepreke2[druga_poz++] = p;
                }
            }
        }
    }
}

/*Radimo isto sto i u gornjoj funkciji. Razlika je u tome sto se prepreke ne postavljaju na celu ravan na pocetku igre vec samo na udaljeni deo prve ravni*/
static void postavi1(){
    
    int i, j;
    
    /*Koristimo manji broj traka*/
    for ( i = 0; i <= 3; i++){
        
        int broj_pr = (int)rand() % 7;
        if(broj_pr == 0)
            broj_pr = 2;
        
        int moze_prepreka[] = {0, 0, 0, 0, 0, 0};
        for (j = 0; j < broj_pr; j++){
            
            Prepreka p;
            int pozicije[] = {-6,-4, -2, 0, 2, 4, 6};
            int pos = (int)rand() % 7;
            
            if (moze_prepreka[pos] == 0){
                
                moze_prepreka[pos] = 1;
                
                int t = (int)rand() % 2;
                int k = (int)rand() % 2;
                if (t == 0){
                    p.tip1 = 0;
                    p.y = 0.5;
                    if( k == 0)
                        p.tip2 = 0;
                    else
                        p.tip2 = 1;
                }
                else{
                    p.tip1 = 1;
                    p.y = 0.75;
                    if( k == 0)
                        p.tip2 = 0;
                    else
                        p.tip2 = 1;
                }
                p.x = pozicije[pos];
                p.z = z_ravni1+ 50 - i * 20;
                prepreke1[prva_poz++] = p;
            }
        }
    }
}

/*U zavisnosti od prosledjene promenljive tip, generisane prepreke se iscrtavaju na odgovarajucoj ravni*/
static void nacrtaj_prepreke(int tip){
    
    /*Promenljiva len oznacava ukupan broj prepreka*/
    int len = 0;
    if (tip == 1)
        len = prva_poz;
    else
        len = druga_poz;
    
    /*Uzimamo jednu po jednu prepreku iz niza i u zavisnoti od njenog tipa i podvrste, crtamo odgovarajuci objekat*/
    int i;
    for (i = 0; i < len; i++){
        
        Prepreka p;
        if (tip == 1)
            p = prepreke1[i];
        else
            p = prepreke2[i];

        if (p.tip1 == 0){ /*ako je tip1=0 u pitanju je hrana*/
            
            if(p.tip2 == 0){ /*ako je tip2=0 u pitanju je lizalica*/
                glPushMatrix();
                    glTranslatef(p.x, p.y, p.z);
                    glScalef(1.1, 1.1, 1.1);
                    nacrtaj_lizalicu();
                glPopMatrix();
        }
            else if (p.tip2 == 1){ /*ako je tip2=1 u pitanju je bombona*/
                glPushMatrix();
                    glTranslatef(p.x, p.y, p.z);
                    glScalef(1.1, 1.1, 1.1);
                    nacrtaj_bombonu();
                glPopMatrix();
            }
        }
        /*Tip1 nam je u suprotnom 1, sto znaci da postavljamo prepreku*/
        else {
            if(p.tip2 == 0){ /*ako je tip2=0 u pitanju je roze prepreka*/
                glPushMatrix();
                    glColor3f(221.0/255,99.0/255,98.0/255);
                    glTranslatef(p.x, p.y, p.z);
                    glScalef(1, 1, 1);
                    glutSolidCube(1);
                glPopMatrix();
            }
            else if(p.tip2 == 1){ /*ako je tip2=1 u pitanju je plava prepreka*/
                glPushMatrix();
                    glColor3f(135.0/255,145.0/255,188.0/255);
                    glTranslatef(p.x, p.y, p.z);
                    glScalef(1, 1, 1);
                    glutSolidCube(1);
                glPopMatrix();
            }
        }
    }
}

/*Pomocna funkcija koja sluzi da se odredi udaljenost pande od prepreke kako bismo znali da li se sudarila sa njom*/
static float udaljenost(Prepreka pr){
    
    float x = pow((pr.x - x_koordinata), 2);
    float y = pow((pr.y - y_koordinata), 2);
    float z = pow((pr.z - z_koordinata), 2);

    return sqrt(x + y + z);
}

/*Funkcija za razresavanje kolizije*/
static void kolizija(){
    
    int i;
    
    /*U zavisnosti od toga na kojoj se ravni nalazimo, proveravamo udaljenost pande od svake prepreke na toj ravni*/
    if(z_ravni1 < z_ravni2){
        
        for(i = 0; i < prva_poz; i++){
            
            Prepreka pr = prepreke1[i];
            
            float u = udaljenost(pr);
            if(u < 1.3*panda_param){ /*menja se jer panda raste*/
                if(prepreke1[i].tip1 == 0 && prepreke1[i].tip2 == 0){ /*ako smo udarili u lizalicu score se povecava za 10*/
                    score += 10;
                    prepreke1[i].x = 100; /*prepreka se "sklanja sa puta"*/
                }
                /*Ako smo udarili u bombonu score se povecava za 5*/
                else if(prepreke1[i].tip1 == 0 && prepreke1[i].tip2 == 1){
                    score += 5;
                    prepreke1[i].x = 100; /*prepreka se sklanja sa puta*/
                }
                /*Ako smo udarili u roze prepreku score se smanjuje za 5*/
                else if (prepreke1[i].tip1 == 1 && prepreke1[i].tip2 == 0){
                    score -= 5;
                    prepreke1[i].x = 100; /*prepreka se sklanja sa puta*/
                }
                /*Ako smo udarili u plavu prepreku, izgubili smo :(*/
                else if (prepreke1[i].tip1 == 1 && prepreke1[i].tip2 == 1){
                    
                    start = 0;
                    glutDisplayFunc(game_over); /*Postavlja se gameover slika*/
                    glutPostRedisplay();
            
                }
            }
        }
    }
    /*Radimo sve isto kao za prvu ravan*/
    else{
         for(i = 0; i < druga_poz; i++){
            
             Prepreka pr = prepreke2[i];
            float u = udaljenost(pr);
            
            if(u < 1.3*panda_param){
             
                if(prepreke2[i].tip1 == 0 && prepreke2[i].tip2 == 0){
                    score += 10;
                    prepreke2[i].x = 100;
                }
                else if(prepreke2[i].tip1 == 0 && prepreke2[i].tip2 == 1){
                    score += 5;
                    prepreke2[i].x = 100;
                }
                else if (prepreke2[i].tip1 == 1 && prepreke2[i].tip2 == 0){
                    score -= 5;
                    prepreke2[i].x = 100;
                }
                else if (prepreke2[i].tip1 == 1 && prepreke2[i].tip2 == 1){
                
                    start = 0;
                    glutDisplayFunc(game_over);
                    glutPostRedisplay();
                }
            }
         }  
    }
}
/*Funkcija kojom se postavlja gameover izgled ekrana*/
void game_over(){
    
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*Pozicioniramo kameru na odgovarajucu poziciju*/
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 3.5, 0,
              0, 0, 0,
              1, 0, 0);
    /*Postavljamo teksturu, tj sliku*/
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, names[2]);
	
		glBegin(GL_POLYGON);
			glTexCoord2f(0, 0);
			glVertex3f(-2, 0, -3);
			
			glTexCoord2f(0, 1);
			glVertex3f(2, 0, -3);
			
			glTexCoord2f(1, 1);
			glVertex3f(2, 0, 3);
			
			glTexCoord2f(1, 0);
			glVertex3f(-2, 0, 3);
		glEnd();
        
	glDisable(GL_TEXTURE_2D);
    
    /*Ispisujemo score na ekran*/
    
    glPushMatrix();
        glColor3f(0,0,0);
        glRasterPos3f(0.3, 0.1, -0.9);
        char score_display[50] = " ";
        char score_value[50];
        sprintf(score_value, " %d ", score);
        strcat(score_display, score_value);

        int len = (int)strlen(score_display);
        int i;
        for ( i = 0; i < len; i++){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, score_display[i]);
        }
    glPopMatrix();
    
    glutSwapBuffers();
	
}
/*Funkcija kojom se postavlja startgame izgled ekrana*/
/*Radimo sve isto kao u gameover funkciji, jedino se tekstura razlikuje*/
void startgame(){
    
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 3.5, 0,
              0, 0, 0,
              1, 0, 0);
    
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, names[3]);
	
		glBegin(GL_POLYGON);
			glTexCoord2f(0, 0);
			glVertex3f(-2, 0, -3);
			
			glTexCoord2f(0, 1);
			glVertex3f(2, 0, -3);
			
			glTexCoord2f(1, 1);
			glVertex3f(2, 0, 3);
			
			glTexCoord2f(1, 0);
			glVertex3f(-2, 0, 3);
		glEnd();
        
	glDisable(GL_TEXTURE_2D);
    

    glutSwapBuffers();
	
}
