//------------------------------
// Miros≥aw Kuüniar (248870)
// Semestr zimowy 2020/2021
// Grafika komp. - Lab 2
//------------------------------

#include <iostream>
#include <windows.h>
#include <gl/gl.h>
#include <glut.h>

typedef float point3[3];

static float theta[] = { 0.0, 0.0, 0.0 }; //punkt zero uk≥adu wspolrzednych 
float u, v;								  //zmienne potrzebne do opisu powierzchni jaja
const int node_number = 50;				  //zmienna przechowywujaca ilosc wezlow

int model_number = 1; //zmienna przechowujaca numer decydujacy o modelu 
int axis_number = 0;  //zmienna przechowujaca numer decydujacy o osi obrotu  

struct point //punkt - wspolrzedne x,y,z
{
	double x;
	double y;
	double z;
};

point nodeMatrix[node_number][node_number]; //tablica s≥uøaca do zapisywania wspolrzednych punktow w przestrzeni 3-D

void rotate() //funkcja odpowiedzialna za rotacje 
{
	for (int i = 0; i < 3; i++)
	{
		theta[i] = theta[i] - 0.11; //zmieniajac 0.11 na inna wartosc mozna przyspierszyc/ zwolnic ruch 
		if (theta[i] > 360.0)
			theta[i] -= 360.0;
	}
	glutPostRedisplay();
}
void generateTable() //funkcja odpowiedzialna za generacje zbioru punktow przechowywanych w tablicy 
{
	for (int i = 0; i < node_number; ++i)
	{
		for (int j = 0; j < node_number; ++j)
		{
			u = (double)i / (node_number - 1);
			v = (double)j / (node_number - 1);
			nodeMatrix[i][j].x = (-(90 * pow(u, 5)) + (225 * pow(u, 4)) - (270 * pow(u, 3)) + (180 * pow(u, 2)) - 45 * u) * cos(3.14 * v);
			nodeMatrix[i][j].y = (160 * pow(u, 4)) - (320 * pow(u, 3)) + (160 * pow(u, 2));
			nodeMatrix[i][j].z = (-(90 * pow(u, 5)) + (225 * pow(u, 4)) - (270 * pow(u, 3)) + (180 * pow(u, 2)) - 45 * u) * sin(3.14 * v);
		}
	}
};
void drawEgg() //funkcja odpowedzialna za wyrysowanie jaja w postaci...
{
	for (int i = 0; i < node_number - 1; i++)
	{
		for (int j = 0; j < node_number - 1; j++)
		{
			if (model_number == 1)
			{
				glBegin(GL_POINTS); // ...punktow
				glVertex3f((float)nodeMatrix[i][j].x, (float)nodeMatrix[i][j].y - 5.0f, (float)nodeMatrix[i][j].z);
				glVertex3f((float)nodeMatrix[(i + 1) % node_number][j].x, (float)nodeMatrix[(i + 1) % node_number][j].y - 5.0f, (float)nodeMatrix[(i + 1) % node_number][j].z);
				glVertex3f((float)nodeMatrix[i][(j + 1) % node_number].x, (float)nodeMatrix[i][(j + 1) % node_number].y - 5.0f, (float)nodeMatrix[i][(j + 1) % node_number].z);
				glVertex3f((float)nodeMatrix[(i + 1) % node_number][(j + 1) % node_number].x, (float)nodeMatrix[(i + 1) % node_number][(j + 1) % node_number].y - 5.0f, (float)nodeMatrix[(i + 1) % node_number][(j + 1) % node_number].z);
				glEnd();

			}

			if (model_number == 2)
			{
				glBegin(GL_LINE_STRIP); // ...linii
				glVertex3f((float)nodeMatrix[i][j].x, (float)nodeMatrix[i][j].y - 5.0f, (float)nodeMatrix[i][j].z);
				glVertex3f((float)nodeMatrix[(i + 1) % node_number][j].x, (float)nodeMatrix[(i + 1) % node_number][j].y - 5.0f, (float)nodeMatrix[(i + 1) % node_number][j].z);
				glVertex3f((float)nodeMatrix[(i + 1) % node_number][(j + 1) % node_number].x, (float)nodeMatrix[(i + 1) % node_number][(j + 1) % node_number].y - 5.0f, (float)nodeMatrix[(i + 1) % node_number][(j + 1) % node_number].z);
				glVertex3f((float)nodeMatrix[i][(j + 1) % node_number].x, (float)nodeMatrix[i][(j + 1) % node_number].y - 5.0f, (float)nodeMatrix[i][(j + 1) % node_number].z);
				glEnd();
			}
			if (model_number == 3)
			{
				glBegin(GL_QUADS); // ...trojkatow
				glVertex3f((float)nodeMatrix[i][j].x, (float)nodeMatrix[i][j].y - 5.0f, (float)nodeMatrix[i][j].z);
				glVertex3f((float)nodeMatrix[(i + 1) % node_number][j].x, (float)nodeMatrix[(i + 1) % node_number][j].y - 5.0f, (float)nodeMatrix[(i + 1) % node_number][j].z);
				glVertex3f((float)nodeMatrix[(i + 1) % node_number][(j + 1) % node_number].x, (float)nodeMatrix[(i + 1) % node_number][(j + 1) % node_number].y - 5.0f, (float)nodeMatrix[(i + 1) % node_number][(j + 1) % node_number].z);
				glVertex3f((float)nodeMatrix[i][(j + 1) % node_number].x, (float)nodeMatrix[i][(j + 1) % node_number].y - 5.0f, (float)nodeMatrix[i][(j + 1) % node_number].z);
				glEnd();
			}
		}
	}
}

void rotateEgg(void) //funkcja odpowedzialana za rotacje jaja wzg. wybranej osi  
{
	if (axis_number == 0)
	{
		drawEgg();
	}
	if (axis_number == 1)
	{
		glRotatef(theta[1], 1.0, 0.0, 0.0);
		drawEgg();
	}
	if (axis_number == 2)
	{
		glRotatef(theta[2], 0.0, 0.0, 1.0);
		drawEgg();
	}
}

void Axes(void) //funkcja rysujaca osie ukladu wspolrzednych 
{
	point3  x_min = { -3.0, 0.0, 0.0 };
	point3  x_max = { 3.0, 0.0, 0.0 };
	
	point3  y_min = { 0.0, -3.0, 0.0 };
	point3  y_max = { 0.0,  3.0, 0.0 };
	
	point3  z_min = { 0.0, 0.0, -3.0 };
	point3  z_max = { 0.0, 0.0,  3.0 };
	
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES); 
	glVertex3fv(x_min);
	glVertex3fv(x_max);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f); 
	glBegin(GL_LINES);

	glVertex3fv(y_min);
	glVertex3fv(y_max);
	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f); 
	glBegin(GL_LINES);

	glVertex3fv(z_min);
	glVertex3fv(z_max);
	glEnd();
}

void RenderScene(void) //funkcja okreúlajπca co ma byÊ rysowane
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	Axes();
	glColor3f(1.0f, 1.0f, 1.0f);
	rotateEgg();

	glFlush();
	glutSwapBuffers();
}

void keys_model(unsigned char key, int x, int y) //funkcja odpowiedzialna za tryb wyswietlania jaja
{
	if (key == 'p') model_number = 1; //wcisniece przycisku p powoduje wyswietlenie jaja w postaci punktow
	if (key == 'l') model_number = 2; //wcisniece przycisku l powoduje wyswietlenie jaja w postaci linii
	if (key == 't') model_number = 3; //wcisniece przycisku t powoduje wyswietlenie jaja w postaci trojkatow

}

void keys_axis(int key, int state, int x, int y) //funkcja odpowiedzilana za tryb rotacji jaja
{
	if (key == 0) axis_number = 1; //wcisniece prawego przycisku myszy powoduje rotacje wzg. osi Z
	if (key == 1) axis_number = 0; //wcisniece scrolla zatrzymuje ruch 
	if (key == 2) axis_number = 2; //wcisniece lewego przycisku myszy powoduje rotacje wzg. osi X
}

void ChangeSize(GLsizei horizontal, GLsizei vertical) //funkcja ma za zadanie utrzymanie sta≥ych proporcji rysowanych obiektow w przypadku zmiany rozmiarÛw okna
{
	GLfloat AspectRatio;
	if (vertical == 0)
		vertical = 1;
	glViewport(0, 0, horizontal, vertical);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;

	if (horizontal <= vertical)

		glOrtho(-7.5, 7.5, -7.5 / AspectRatio, 7.5 / AspectRatio, 10.0, -10.0);
	else

		glOrtho(-7.5 * AspectRatio, 7.5 * AspectRatio, -7.5, 7.5, 10.0, -10.0);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
}

int main(int argc, char** argv) //funkcja glowna 
{
	generateTable();

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("3D Egg Shape");

	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);

	glutIdleFunc(rotate);
	glutKeyboardFunc(keys_model);
	glutMouseFunc(keys_axis);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
}