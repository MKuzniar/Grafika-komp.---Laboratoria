#include <windows.h>
#include <gl/gl.h>
#include <glut.h>
#include <cmath>

using namespace std;

typedef float point3[3];

static float theta[] = { 0.0, 0.0, 0.0 };
float u, v;
const int node_number = 50;

struct point
{
	double x;
	double y;
	double z;
};

point nodeMatrix[node_number][node_number];


static GLfloat viewer[] = { 0.0, 0.0, 10.0 }; // Po³o¿enie obserwatora 

// Wartoœci k¹tów azymutu i elewacji 
static GLfloat Azimuth = 0.0;
static GLfloat Elevation = 0.0;

// Przeliczniki pikseli na stopnie
static GLfloat pix2angle_X;
static GLfloat pix2angle_Y;

static int status = 0; // Stan klawiszy myszy

// Poprzednie pozycje kursora myszy oraz ró¿nice pomiêdzy pozycj¹ bie¿¹c¹ i poprzedni¹ kursora
static int x_pos_old = 0; 
static int delta_x = 0;   

static int y_pos_old = 0; 
static int delta_y = 0;   

static GLfloat R_val_old = 0.0;
static GLfloat delta_R = 0.0;
static GLfloat R = 10.0;

bool reset = false;

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

void drawEgg() //funkcja odpowedzialna za wyrysowanie jaja w postaci linii 
{
	for (int i = 0; i < node_number - 1; i++)
	{
		for (int j = 0; j < node_number - 1; j++)
		{
			glBegin(GL_LINE_STRIP);
			glVertex3f((float)nodeMatrix[i][j].x, (float)nodeMatrix[i][j].y - 5.0f, (float)nodeMatrix[i][j].z);
			glVertex3f((float)nodeMatrix[(i + 1) % node_number][j].x, (float)nodeMatrix[(i + 1) % node_number][j].y - 5.0f, (float)nodeMatrix[(i + 1) % node_number][j].z);
			glVertex3f((float)nodeMatrix[(i + 1) % node_number][(j + 1) % node_number].x, (float)nodeMatrix[(i + 1) % node_number][(j + 1) % node_number].y - 5.0f, (float)nodeMatrix[(i + 1) % node_number][(j + 1) % node_number].z);
			glVertex3f((float)nodeMatrix[i][(j + 1) % node_number].x, (float)nodeMatrix[i][(j + 1) % node_number].y - 5.0f, (float)nodeMatrix[i][(j + 1) % node_number].z);
			glEnd();
		}
	}
}

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	if (status == 1)
	{
		// Modyfikacja k¹ta azymutu i elewacji o kat proporcjonalny do ró¿nicy po³o¿eñ kursora myszy w odpowiednich p³aszczyznach
		Azimuth += delta_x * pix2angle_X * 0.01; 
		Elevation += delta_y * pix2angle_Y * 0.01;
	}
	else if (status == 2)
	{
		// Modyfikacja promienia R sfery po której powierzchni porusza siê obserwator 
		R += delta_R * pix2angle_Y;
	}

	if (reset == true) // Nadanie wartoœci pocz¹tkowych w przypadku Resetu
	{
		Azimuth = 0.0;
		Elevation = 0.0;
		R = 10.0;
	}

	// Wyzanczenie po³o¿enia obserwatora na podstawie k¹ta azymutu i elewacji oraz promienia sfery 
	viewer[0] = R * cos(Azimuth) * cos(Elevation);
	viewer[1] = R * sin(Elevation);
	viewer[2] = R * sin(Azimuth) * cos(Elevation);

	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); //definiowanie po³o¿enia obserwatora

	Axes(); 
	glColor3f(1.0f, 1.0f, 1.0f);
	drawEgg();

	glFlush();
	glutSwapBuffers();
}

void Mouse(int btn, int state, int x, int y) // Funkcja "bada" stan myszy i ustawia wartoœci odpowiednich zmiennych globalnych
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) // Wciœniêcie lewego przycisku myszy umo¿liwia przemieszczanie kamery 
	{
		x_pos_old = x;
		y_pos_old = y;
		status = 1;
	}
	else if (btn == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) // Wciœniêcie scrolla i jednoczesne przemieszczanie myszy przód - ty³ umo¿liwa przybli¿anie/oddalanie 
	{
		R_val_old = y;
		status = 2;
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) // Wciœniêcie prawego przycisku myszy umo¿wiwa reset widoku (najskuteczniej jest wcisn¹æ prawy przycisk myszy i jednoczeœnie lekko przemieœciæ mysz)
	{
		reset = true;
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		reset = false;
	}
	else
	{
		status = 0;
	}
}

void Motion(GLsizei x, GLsizei y) // Funkcja "monitoruje" po³o¿enie kursora myszy i ustawia wartoœci odpowiednich zmiennych globalnych
{
	// Obliczanie ró¿nicy po³o¿enia kursora myszy
	delta_x = x - x_pos_old;  
	delta_y = y - y_pos_old; 
	delta_R = y - R_val_old;

	// Podstawianie bie¿¹cego po³o¿enia jako poprzednie 
	x_pos_old = x;
	y_pos_old = y; 
	R_val_old = y;

	glutPostRedisplay(); // Przerysowanie obrazu sceny
}

void ChangeSize(GLsizei horizontal, GLsizei vertical) // Funkcja ma za zadanie utrzymanie sta³ych proporcji rysowanych w przypadku zmiany rozmiarów okna.
{
	// Przeliczenie pikseli na stopnie
	pix2angle_X = 360.0 / (float)horizontal; 
	pix2angle_Y = 360.0 / (float)vertical;   

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 1.0, 1.0, 30.0);

	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);
	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(void)
{
	generateTable();

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("Action and reaction");

	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);

	glutMouseFunc(Mouse);   // Ustala funkcjê zwrotn¹ odpowiedzialn¹ za badanie stanu myszy
	glutMotionFunc(Motion); // Ustala funkcjê zwrotn¹ odpowiedzialn¹ za badanie ruchu myszy

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
}