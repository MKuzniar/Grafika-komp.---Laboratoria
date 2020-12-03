// Copyright [2020] <Miroslaw Kuzniar>"
#include <windows.h> // NOLINT
#include <iostream> // NOLINT
#include <gl/gl.h> // NOLINT
#include <glut.h> // NOLINT
#include <stdio.h> // NOLINT
#include <conio.h> // NOLINT
#include <math.h> // NOLINT

#define M_PI 3.14159265358979323846

using namespace std; // NOLINT

GLfloat light_position[] = { 30.0, 0.0, -10.0, 1.0 };

static GLfloat theta[] = { 0.0, 0.0, 0.0 };

typedef float point3[3];
const int node_number = 80;

typedef float point2[2];
point2** textureCord;

static int R = 30;
static GLfloat azimuth = 0.0;
float elevation = 0.0;

static GLfloat pix2angle;

int plik_z_tekstura;

// Funkcja sluzaca do odczytywania obrazu
// tekstury z pliku graficznego w formacie TGA
GLbyte* LoadTGAImage(const char* FileName, GLint* ImWidth,
GLint* ImHeight, GLint* ImComponents, GLenum* ImFormat) {
    // Struktura dla naglowka pliku TGA
    #pragma pack(1)
    typedef struct {
        GLbyte idlength;
        GLbyte colormaptype;
        GLbyte datatypecode;
        int16_t colormapstart;
        int16_t colormaplength;
        unsigned char colormapdepth;
        int16_t x_orgin;
        int16_t y_orgin;
        int16_t width;
        int16_t height;
        GLbyte bitsperpixel;
        GLbyte descriptor;
    }
    TGAHEADER;
    #pragma pack(8)

    FILE* pFile;
    TGAHEADER tgaHeader;
    int64_t lImageSize;
    int16_t sDepth;
    GLbyte* pbitsperpixel = NULL;

    // Wartosci domyslne zwracane w przypadku bledu
    *ImWidth = 0;
    *ImHeight = 0;
    *ImFormat = GL_BGR_EXT;
    *ImComponents = GL_RGB8;

    pFile = fopen(FileName, "rb");
    if (pFile == NULL)
        return NULL;

    // Przeczytanie naglowka pliku
    fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);

    // Odczytanie szerokosci, wysokosci i glebi obrazu
    *ImWidth = tgaHeader.width;
    *ImHeight = tgaHeader.height;
    sDepth = tgaHeader.bitsperpixel / 8;

    // Sprawdzenie, czy glebia spelnia zalozone warunki
    if (tgaHeader.bitsperpixel != 8 && tgaHeader.bitsperpixel != 24
        && tgaHeader.bitsperpixel != 32)
        return NULL;

    // Obliczenie rozmiaru bufora w pamieci
    lImageSize = tgaHeader.width * tgaHeader.height * sDepth;

    // Alokacja pamieci dla danych obrazu
    pbitsperpixel = reinterpret_cast<GLbyte*>
    (malloc(lImageSize * sizeof(GLbyte)));

    if (pbitsperpixel == NULL)
        return NULL;

    if (fread(pbitsperpixel, lImageSize, 1, pFile) != 1) {
        free(pbitsperpixel);
        return NULL;
    }

    // Ustawienie formatu OpenGL
    switch (sDepth) {
    case 3:
        *ImFormat = GL_BGR_EXT;
        *ImComponents = GL_RGB8;
        break;
    case 4:
        *ImFormat = GL_BGRA_EXT;
        *ImComponents = GL_RGBA8;
        break;
    case 1:
        *ImFormat = GL_LUMINANCE;
        *ImComponents = GL_LUMINANCE8;
        break;
    };

    fclose(pFile);

    return pbitsperpixel;
}

// Funkcja odpowiedzialna za rotacje
void rotateEgg() {
    theta[1] -= 0.5;
    if (theta[1] > 360.0) theta[1] -= 360.0;

    glutPostRedisplay();
}

// Funkcja odpowedzialna za wyrysowanie jaja w postaci trojkatow
void drawEgg() {
    float tab[100][100][3];

    float u = 0.0;
    float v = 0.0;

    point3** vectors3d = new point3 * [node_number];
    point3** vectorNorm = new point3 * [node_number];

    for (int i = 0; i < node_number; i++) {
        vectors3d[i] = new point3[node_number];
        vectorNorm[i] = new point3[node_number];
    }

    for (int i = 0; i < node_number; i++) {
        for (int j = 0; j < node_number; j++) {
            u = static_cast<float>(i) / (node_number - 1);
            v = static_cast<float>(j) / (node_number - 1);

            // Wyzaczenie wspolrzednych na podstawie rownan parametrycznych
            tab[i][j][0] = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3)
            + 180 * pow(u, 2) - 45 * u) * cos(M_PI * v);
            tab[i][j][1] = 160 * pow(u, 4) - 320 * pow(u, 3)
            + 160 * pow(u, 2) - 5;
            tab[i][j][2] = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3)
            + 180 * pow(u, 2) - 45 * u) * sin(M_PI * v);

            float ux, uz, uy, vz, vy, vx, length;

            // Obliczenie wzorow pozwalajacych na obliczenie wektorow normalnych
            ux = (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2)
            + 360 * u - 45) * cos(3.14 * v);
            uy = (640 * pow(u, 3) - 960 * pow(u, 2) + 320 * u);
            uz = (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2)
            + 360 * u - 45) * sin(3.14 * v);

            vx = 3.14 * (90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3)
            - 180 * pow(u, 2) + 45 * u) * sin(3.14 * v);
            vy = 0;
            vz = -3.14 * (90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3)
            - 180 * pow(u, 2) + 45 * u) * cos(3.14 * v);

            textureCord[i][j][0] = v;
            textureCord[i][j][1] = u;

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
                vectorNorm[i][j][0] = (uy * vz - uz * vy) / length;
                vectorNorm[i][j][1] = (uz * vx - ux * vz) / length;
                vectorNorm[i][j][2] = (ux * vy - uy * vx) / length;
            } else if (i > node_number / 2) {
                vectorNorm[i][j][0] = -1 * (uy * vz - uz * vy) / length;
                vectorNorm[i][j][1] = -1 * (uz * vx - ux * vz) / length;
                vectorNorm[i][j][2] = -1 * (ux * vy - uy * vx) / length;
            } else {
                vectorNorm[i][j][0] = 0;
                vectorNorm[i][j][1] = 1;
                vectorNorm[i][j][2] = 0;
            }
        }
    }
    int i = 0;

    // Wlaswiwe wyrysowanie jaja
    for (i = 0; i < node_number - 1;) {
        for (int j = 0; j < node_number - 1; j++) {
            glColor3f(255.0f, 255.0f, 255.0f);
            glBegin(GL_TRIANGLES);

            glNormal3fv(vectorNorm[i][j + 1]);
            glTexCoord2f(textureCord[i][j + 1][0], textureCord[i][j + 1][1]);
            glVertex3fv(tab[i][j + 1]);

            glNormal3fv(vectorNorm[i + 1][j]);
            glTexCoord2f(textureCord[i + 1][j][0], textureCord[i + 1][j][1]);
            glVertex3fv(tab[i + 1][j]);

            glNormal3fv(vectorNorm[i + 1][j + 1]);
            glTexCoord2f(textureCord[i + 1][j + 1][0],
            textureCord[i + 1][j + 1][1]);
            glVertex3fv(tab[i + 1][j + 1]);

            glNormal3fv(vectorNorm[i + 1][j]);
            glTexCoord2f(textureCord[i + 1][j][0], textureCord[i + 1][j][1]);
            glVertex3fv(tab[i + 1][j]);

            glNormal3fv(vectorNorm[i][j + 1]);
            glTexCoord2f(textureCord[i][j + 1][0], textureCord[i][j + 1][1]);
            glVertex3fv(tab[i][j + 1]);

            glNormal3fv(vectorNorm[i][j]);
            glTexCoord2f(textureCord[i][j][0], textureCord[i][j][1]);
            glVertex3fv(tab[i][j]);

            glEnd();
        }
        i++;
    }
}
void RenderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0, 0, 10.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);

    light_position[0] = R * cos(azimuth) * cos(elevation);
    light_position[1] = R * sin(elevation);
    light_position[2] = R * sin(azimuth) * cos(elevation);

    glRotatef(theta[0], 1.0, 0.0, 0.0);
    glRotatef(theta[1], 0.0, 1.0, 0.0);
    glRotatef(theta[2], 0.0, 0.0, 1.0);

    drawEgg();

    glFlush();
    glutSwapBuffers();
}

void MyInit(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Zmienne dla obrazu tekstury
    GLbyte* pBytes;
    GLint ImWidth, ImHeight, ImComponents;
    GLenum ImFormat;

    // Definicja materialu z jakiego zrobione jest jajo

    GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    // Wspolczynniki ka =[kar,kag,kab] dla swiatla otoczenia
    GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    // Wspolczynniki kd =[kdr,kdg,kdb] swiatla rozproszonego
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    // Wspolczynniki ks =[ksr,ksg,ksb] dla swiatla odbitego
    GLfloat mat_shininess = { 100.0 };
    // Wspolczynnik n opisujacy polysk powierzchni

    // Definicja zrodla swiatla

    GLfloat light_position[] = { 0.0, 0.0, 10.0, 1.0 };
    // Polozenie zrodla
    GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
    // Skladowe intensywnosci swiecenia zrodla swiatla otoczenia
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    // Skladowe intensywnosci swiecenia
    // zrodla swiatla powodujacego odbicie dyfuzyjne
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    // Skladowe intensywnosci swiecenia zrodla
    // swiatla powodujacego odbicie kierunkowe
    GLfloat att_constant = { 1.0 };
    // Skladowa stala ds dla modelu zmian oswietlenia
    // w funkcji odleglosci od zrodla
    GLdouble att_linear = { 0.05 };
    // Skladowa liniowa dl dla modelu zmian oswietlenia
    // w funkcji odleglosci od zrodla
    GLdouble att_quadratic = { 0.001 };
    // Skladowa kwadratowa dq dla modelu zmian oswietlenia
    // w funkcji odleglosci od zrodla

    // Ustawienie patrametrow materialu
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

    // Ustawienie parametrow zrodla
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);

    // Ustawienie opcji systemu oswietlania sceny
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    // Przeczytanie obrazu tekstury z pliku *.tga
    if (plik_z_tekstura == 1) {
        pBytes = LoadTGAImage("D1_t.tga", &ImWidth,
        &ImHeight, &ImComponents, &ImFormat);
    } else if (plik_z_tekstura == 2) {
        pBytes = LoadTGAImage("P4_t.tga", &ImWidth,
        &ImHeight, &ImComponents, &ImFormat);
    } else {
        pBytes = LoadTGAImage("P3_t.tga", &ImWidth,
        &ImHeight, &ImComponents, &ImFormat);
    }

    // Zdefiniowanie tekstury 2-D
    glTexImage2D(GL_TEXTURE_2D, 0, ImComponents, ImWidth,
    ImHeight, 0, ImFormat, GL_UNSIGNED_BYTE, pBytes);
    // Zwolnienie pamieci
    free(pBytes);
    // Wlaczenie mechanizmu teksturowania
    glEnable(GL_TEXTURE_2D);
    // Ustalenie trybu teksturowania
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    // Okreslenie sposobu nakladania tekstur
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void generateTable() {
    textureCord = new point2 * [node_number + 1];
    for (int i = 0; i < node_number + 1; i++) {
        textureCord[i] = new point2[node_number + 1];
    }
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

// Funkcja glowna
void main(void) {
    cout << "-----------------------------------------------" << endl;
    cout << " | 1 - D1_t.tga | 2 - P4_t.tga | 3 - P3_t.tga | " << endl;
    cout << "-----------------------------------------------" << endl;
    cout << " Plik z tekstura: ";
    cin >> plik_z_tekstura;

    generateTable();

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 1000);

    glutCreateWindow("Tekstury");

    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);

    glutIdleFunc(rotateEgg);

    MyInit();

    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
}
