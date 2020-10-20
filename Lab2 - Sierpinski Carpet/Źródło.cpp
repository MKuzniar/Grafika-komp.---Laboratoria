//------------------------------
// Miros�aw Ku�niar (248870)
// Semestr zimowy 2020/2021
// Grafika komp. - Lab 1
//------------------------------

#include <windows.h>
#include <glut.h>
#include <cstdlib>
#include <ctime>
#include <iostream>

int zad_l_poziom�w; //zmienna przechowuj�ca wprowadzon� liczb� poziom�w

float losuj_kolor()
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX); //generowana liczba typu float w celu ustalenia losowego koloru 
}

float losuj_nierownosci(float wspolczynnik)
{
	return wspolczynnik * 0.02 * (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)); //generowana liczba typu float w celu ustalenia wielkosci nierownosci 
}

void rysuj_Kwadrat(float x, float y, float rozmiar)						//Rysowanie pojedynczego kwadratu
{
	glRectf(x,y, x + rozmiar, y - rozmiar);								//rysowanie kwadratu 
	glColor3f(losuj_kolor(), losuj_kolor(), losuj_kolor());				//ustalenia koloru kwadratu
}

void rysuj_Dywan_Sierpinskiego(float x, float y, float rozmiar, float obec_poziom) {
	for (int i = 0; i < 3; i++) //rysowanie kwadrat�w horyzontalnie
	{
		for (int j = 0; j < 3; j++) //rysowanie kwadrat�w wertykalnie
		{
			if ((i == 1) && (j == 1))
			{
				//w momencie gdy 'i' i 'j' r�wnaj� si� 1 nie rysujemy kwadratu tym samym pozostawiaj�c puste miejsce w �rodku wi�kszego kwadratu 
			}
			else
			{
				if (obec_poziom > zad_l_poziom�w - 2)
				{
					rysuj_Kwadrat(x + i * (rozmiar / 3) + losuj_nierownosci(rozmiar), y - j * (rozmiar / 3) + losuj_nierownosci(rozmiar), rozmiar / 3); //w momencie, w kt�rym osi�gneli�my zadan� .poziom�w nast�puje rysowanie kwadrat�w o rozmiarze w�a�ciwym dla tej g��boko�ci 
				}
				else
				{
					rysuj_Dywan_Sierpinskiego(x + i * (rozmiar / 3) + losuj_nierownosci(rozmiar), y - j * (rozmiar / 3) + losuj_nierownosci(rozmiar), rozmiar / 3, obec_poziom + 1); //do momentu w kt�rym nie osi�gniemy zadanej l.poziom�w zmienjszamy rozmiar kwadrat�w (1/3 poprzedniego rozmiaru) 
				}
			}

		}

	}
}

void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT); // Czyszczenie okna aktualnym kolorem czyszcz�cym
	rysuj_Dywan_Sierpinskiego(-50.0f, 50.0f, 100.0f, 0); // Wyrysowanie struktury o zadanych rozmiarach 
	glFlush(); // Przekazanie polece� rysuj�cych do wykonania
}

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	GLfloat AspectRatio; // Deklaracja zmiennej AspectRatio okre�laj�cej proporcj� wymiar�w okna

	if (vertical == 0) // Zabezpieczenie pzred dzieleniem przez 0
		vertical = 1;

	glViewport(0, 0, horizontal, vertical); // Ustawienie wielko�ciokna okna urz�dzenia (Viewport). W tym przypadku od (0,0) do (horizontal, vertical)
	glMatrixMode(GL_PROJECTION); // Okre�lenie uk�adu wsp�rz�dnych obserwatora
	glLoadIdentity();  // Okre�lenie przestrzeni ograniczaj�cej
	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical; // Wyznaczenie wsp�czynnika proporcji okna

	if (horizontal <= vertical)
		glOrtho(-100.0, 100.0, -100.0 / AspectRatio, 100.0 / AspectRatio, 1.0, -1.0);

	else
		glOrtho(-100.0 * AspectRatio, 100.0 * AspectRatio, -100.0, 100.0, 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW); // Okre�lenie uk�adu wsp�rz�dnych
	glLoadIdentity();
}

int main(int argc, char** argv)
{
	std::cout << "Wprowadz liczbe poziomow: ";
	std::cin >> zad_l_poziom�w; //wprowadzenie liczby poziom�w

	srand(time(NULL));
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);   // Ustawienie trybu wy�wietlania
	glutCreateWindow("Lab1 - Dywan Sierpi�skiego"); // Utworzenie okna i okre�lenie tre�ci napisu w nag��wku okna
	glutDisplayFunc(RenderScene);				    // Okre�lenie, �e funkcja RenderScene b�dzie funkcj� zwrotn� (callback)
	glutReshapeFunc(ChangeSize);					// Dla aktualnego okna ustala funkcj� zwrotn� odpowiedzialn� za zmiany rozmiaru okna
	glutMainLoop();									// Funkcja uruchamia szkielet biblioteki GLUT
}