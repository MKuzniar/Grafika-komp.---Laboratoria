// Copyright [2020] <Miroslaw Kuzniar>"
#include <windows.h>
#include <iostream>
#include <gl/gl.h>
#include <glut.h>
#include <math.h>
#include <cmath>
#include <time.h>
#include <vector>

#define M_PI 3.14159265358979323846

using namespace std;

GLfloat light_one_position[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light_two_position[] = { 0.0, 0.0, 0.0, 1.0 };

static float theta[] = { 0.0, 0.0, 0.0 };

typedef float point3[3];
const int node_number = 50;

static GLint status = 0;

static GLfloat azimuth_1 = 0.0;
float elevation_1 = 0.0;

static GLfloat azimuth_2 = 0.0;
float elevation_2 = 0.0;

static GLfloat pix2angle;

static int x_pos_old = 0;
static int delta_x = 0;

static int y_pos_old = 0;
static int delta_y = 0;

double R = 10;

int kolor_1, kolor_2;

// Funkcja odpowiedzialna za rotacje
void rotate() {
    for (int i = 0; i < 3; i++) {
     theta[i] = theta[i] - 0.11;
     if (theta[i] > 360.0)
     theta[i] -= 360.0;
    }
    glutPostRedisplay();
}

// Funkcja odpowedzialna za wyrysowanie jaja w postaci trojkatow
void drawEgg()  {
    float tab[100][100][3];

    float u = 0.0;
    float v = 0.0;

    point3 **vectors3d = new point3*[node_number];
    point3 **vectorNorm = new point3*[node_number];

    for (int i = 0; i < node_number; i++) {
        vectors3d[i] = new point3[node_number];
        vectorNorm[i] = new point3[node_number];
    }

    for (int i = 0; i < node_number; i++) {
        for (int j = 0; j < node_number; j++) {
            u = static_cast<float>(i) / (node_number - 1);
            v = static_cast<float>(j) / (node_number - 1);

            // Wyzaczenie wspolrzednych na podstawie rownan parametrycznych
            tab[i][j][0] = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 *
            pow(u, 3) + 180 * pow(u, 2) - 45 * u)*cos(M_PI * v);

            tab[i][j][1] = 160 * pow(u, 4) - 320 * pow(u, 3) + 160
            * pow(u, 2) - 5;

            tab[i][j][2] = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270
            * pow(u, 3) + 180 * pow(u, 2) - 45 * u)*sin(M_PI * v);

            float ux, uz, uy, vz, vy, vx, length;

            // Obliczenie wzorow pozwalajacych na obliczenie wektorow normalnych
            ux = (-450 * pow(u, 4) + 900 * pow(u, 3) - 810
            * pow(u, 2) + 360 * u - 45)*cos(3.14*v);
            uy = (640 * pow(u, 3) - 960 * pow(u, 2) + 320 * u);
            uz = (-450 * pow(u, 4) + 900 * pow(u, 3) - 810
            * pow(u, 2) + 360 * u - 45)*sin(3.14*v);

            vx = 3.14*(90 * pow(u, 5) - 225 * pow(u, 4) + 270
            * pow(u, 3) - 180 * pow(u, 2) + 45 * u)*sin(3.14*v);
            vy = 0;
            vz = -3.14*(90 * pow(u, 5) - 225 * pow(u, 4) + 270
            * pow(u, 3) - 180 * pow(u, 2) + 45 * u)*cos(3.14*v);

            vectorNorm[i][j][0] = uy * vz - uz * vy;
            vectorNorm[i][j][1] = uz * vx - ux * vz;
            vectorNorm[i][j][2] = ux * vy - uy * vx;

            // Obliczanie dlogosci wektora
            length = sqrt(vectorNorm[i][j][0] * vectorNorm[i][j][0]
            + vectorNorm[i][j][1] * vectorNorm[i][j][1]
            + vectorNorm[i][j][2] * vectorNorm[i][j][2]);

            // Ustalenie warunkow oswietlenia jaja
            if (i == 0 || i == node_number) {
                vectorNorm[i][j][0] = 0;
                vectorNorm[i][j][1] = -1;
                vectorNorm[i][j][2] = 0;
            } else if (i < node_number / 2) {
                vectorNorm[i][j][0] = (uy*vz - uz * vy) / length;
                vectorNorm[i][j][1] = (uz*vx - ux * vz) / length;
                vectorNorm[i][j][2] = (ux*vy - uy * vx) / length;
            } else if (i > node_number / 2) {
                vectorNorm[i][j][0] = -1 * (uy*vz - uz * vy) / length;
                vectorNorm[i][j][1] = -1 * (uz*vx - ux * vz) / length;
                vectorNorm[i][j][2] = -1 * (ux*vy - uy * vx) / length;
            } else {
                vectorNorm[i][j][0] = 0;
                vectorNorm[i][j][1] = 1;
                vectorNorm[i][j][2] = 0;
            }
        }
    }
    int i = 0;
// Wlaswiwe wyrysowanie jaja
for (i = 0; i < node_number - 1; )  {
        for (int j = 0; j < node_number-1 ; j++) {
        glColor3f(255.0f, 255.0f, 255.0f);
        glBegin(GL_TRIANGLES);

        glNormal3fv(vectorNorm[i][j + 1]);
        glVertex3fv(tab[i][j + 1]);

        glNormal3fv(vectorNorm[i + 1][j]);
        glVertex3fv(tab[i + 1][j]);

        glNormal3fv(vectorNorm[i + 1][j + 1]);
        glVertex3fv(tab[i + 1][j + 1]);

        glNormal3fv(vectorNorm[i + 1][j]);
        glVertex3fv(tab[i + 1][j]);

        glNormal3fv(vectorNorm[i][j + 1]);
        glVertex3fv(tab[i][j + 1]);

        glNormal3fv(vectorNorm[i][j]);
        glVertex3fv(tab[i][j]);

        glEnd();
        }
    i++;
    }
}

// Funkcja odpowedzialana za rotacje jaja wzg wybranej osi
void rotateEgg(void) {
    glRotatef(theta[1], 0.0, 1.0, 0.0);
    drawEgg();
}

void RenderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0, 0, 10.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);

    // Modyfikacja kata azymutu i elewacji o kat proporcjonalny do
    // roznicy polozen kursora myszy w odpowiednich plaszczyznach
    if (status == 1) {
        azimuth_1 += delta_x * pix2angle;
        elevation_1 += delta_y * pix2angle;
    }
    if (status == 2) {
        azimuth_2 += delta_x * pix2angle;
        elevation_2 += delta_y * pix2angle;
    }

    float pi = 3.14;

    if (elevation_1 > 2 * pi) elevation_1 = 2 * pi;
    if (azimuth_1 > 2 * pi) azimuth_1 = 2 * pi;
    if (elevation_1 < 0) elevation_1 = 0;
    if (azimuth_1 < 0) azimuth_1 = 0;

    if (elevation_2 > 2 * pi) elevation_2 = 2 * pi;
    if (azimuth_2 > 2 * pi) azimuth_2 = 2 * pi;
    if (elevation_2 < 0) elevation_2 = 0;
    if (azimuth_2 < 0) azimuth_2 = 0;

    // Wyzanczenie polozenia zrodla swiatla
    light_one_position[0] = R * cos(azimuth_2) * cos(elevation_2);
    light_one_position[1] = R * sin(elevation_2);
    light_one_position[2] = R * sin(azimuth_2) * cos(elevation_2);

    light_two_position[0] = R * cos(azimuth_1) * cos(elevation_1);
    light_two_position[1] = R * sin(elevation_1);
    light_two_position[2] = R * sin(azimuth_1) * cos(elevation_1);

    glLightfv(GL_LIGHT0, GL_POSITION, light_one_position);
    glLightfv(GL_LIGHT1, GL_POSITION, light_two_position);

    glColor3f(1.0f, 1.0f, 1.0f);
    rotateEgg();

    glFlush();
    glutSwapBuffers();
}


void MyInit(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Definicja materialu z jakiego zrobione jest jajo

    GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    // Wspolczynniki ka =[kar,kag,kab] dla swiatla otoczenia
    GLfloat mat_diffuse[] = { 5.0, 5.0, 5.0, 1.0 };
    // Wspolczynniki kd =[kdr,kdg,kdb] swiatla rozproszonego
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    // Wspolczynniki ks =[ksr,ksg,ksb] dla swiatla odbitego
    GLfloat mat_shininess = { 20.0 };
    // Wspolczynnik n opisujacy polysk powierzchni

    // Definicja zrodla swiatla

    GLfloat light_1_position[] = { 0.0, 0.0, 0.0, 1.0 };
    // Polozenie zrodla
    GLfloat light_1_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
    // Skladowe intensywnosci swiecenia zrodla swiatla otoczenia
    GLfloat light_1_diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
    // Skladowe intensywnosci swiecenia
    // zrodla swiatla powodujacego odbicie dyfuzyjne

    if (kolor_1 == 1) {
        light_1_diffuse[0] = 1.0;
        light_1_diffuse[1] = 0.0;
        light_1_diffuse[2] = 0.0;
    } else if (kolor_1 == 2) {
        light_1_diffuse[0] = 0.0;
        light_1_diffuse[1] = 1.0;
        light_1_diffuse[2] = 0.0;
    } else {
        light_1_diffuse[0] = 0.0;
        light_1_diffuse[1] = 0.0;
        light_1_diffuse[2] = 1.0;
}

    GLfloat light_1_specular[] = {0.0, 1.0, 1.0, 0.0 };
    // Skladowe intensywnosci swiecenia zrodla
    // swiatla powodujacego odbicie kierunkowe
    GLfloat att_1_constant = { 1.0 };
    // Skladowa stala ds dla modelu zmian oswietlenia
    // w funkcji odleglosci od zrodla
    GLfloat att_1_linear = {  0.05 };
    // Skladowa liniowa dl dla modelu zmian oswietlenia
    // w funkcji odleglosci od zrodla
    GLfloat att_1_quadratic = {  0.001 };
    // Skladowa kwadratowa dq dla modelu zmian oswietlenia
    // w funkcji odleglosci od zrodla

    GLfloat light_2_position[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_2_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
    GLfloat light_2_diffuse[] = { 0.0, 0.0, 1.0, 10.0 };

    if (kolor_2 == 4) {
        light_2_diffuse[0] = 1.0;
        light_2_diffuse[1] = 0.0;
        light_2_diffuse[2] = 0.0;
    } else if (kolor_2 == 5) {
        light_2_diffuse[0] = 0.0;
        light_2_diffuse[1] = 1.0;
        light_2_diffuse[2] = 0.0;
    } else {
        light_2_diffuse[0] = 0.0;
        light_2_diffuse[1] = 0.0;
        light_2_diffuse[2] = 1.0;
    }

    GLfloat light_2_specular[] = { 1.0, 0.0, 1.0, 10.0 };
    GLfloat att_2_constant = { 1.0 };
    GLfloat att_2_linear = {  0.05 };
    GLfloat att_2_quadratic = {  0.001 };

    // Ustawienie patrametrow materialu
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

    // Ustawienie parametrow zrodla
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_1_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_1_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_1_specular);

    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_1_constant);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_1_linear);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_1_quadratic);

    glLightfv(GL_LIGHT0, GL_POSITION, light_1_position);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_2_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_2_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_2_specular);

    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, att_2_constant);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, att_2_linear);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, att_2_quadratic);
    glLightfv(GL_LIGHT1, GL_POSITION, light_2_position);

    // Ustawienie opcji systemu oswietlania sceny
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_DEPTH_TEST);
}

// Funkcja bada stan myszy i ustawia wartosci odpowiednich zmiennych globalnych
void Mouse(int btn, int state, int x, int y) {
    // Wcisniety lewy przycisk myszy - modyfikacja polozenia zrodla swiatla 1
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        x_pos_old = x;
        y_pos_old = y;
        status = 1;
    // Wcisniety prawy przycisk myszy - modyfikacja polozenia zrodla swiatla 2
    } else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        x_pos_old = x;
        y_pos_old = y;
        status = 2;
    } else {
        status = 0;
    }
}

// Funkcja monitoruje polozenie kursora myszy
void Motion(GLsizei x, GLsizei y) {
    delta_x = x - x_pos_old;
    delta_y = y - y_pos_old;

    x_pos_old = x;
    y_pos_old = y;

    glutPostRedisplay();
}

// Funkcja ma za zadanie utrzymanie stalych
// proporcji rysowanych w przypadku zmiany rozmiarow okna
void ChangeSize(GLsizei horizontal, GLsizei vertical) {
    pix2angle = 360.0 / static_cast<float>(horizontal);
    pix2angle *= 0.009;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLdouble aspect = (GLdouble)horizontal / vertical;

    gluPerspective(70, 1.0, 1.0, 30.0);

    if (horizontal <= vertical)
        glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);
    else
        glViewport((horizontal - vertical) / 2, 0, vertical, vertical);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void main(void) {
    // Ustalenie kolorow zrodel swiatla
    cout << " | 1 - Czerwony | 2 - Zielony | 3 - Niebieski | " << endl;
    cout << " Kolor pierwszego zrodla swiatla: ";
    cin >> kolor_1;
    cout << " | 4 - Czerwony | 5 - Zielony | 6 - Niebieski | " << endl;
    cout << " Kolor drugiego zrodla swiatla: ";
    cin >> kolor_2;

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 1000);

    glutCreateWindow("I stalo sie swiatlo");

    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);

    glutIdleFunc(rotate);
    glutMouseFunc(Mouse);
    glutMotionFunc(Motion);

    MyInit();

    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
}
