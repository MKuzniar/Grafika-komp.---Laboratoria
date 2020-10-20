//------------------------------
// Miros³aw KuŸniar (248870)
// Semestr zimowy 2020/2021
// Grafika komp. - Lab 1
//------------------------------

#include <windows.h>
#include <glut.h>
#include <cstdlib>
#include <ctime>
#include <iostream>

int zad_l_poziomów; //zmienna przechowuj¹ca wprowadzon¹ liczbê poziomów

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
	for (int i = 0; i < 3; i++) //rysowanie kwadratów horyzontalnie
	{
		for (int j = 0; j < 3; j++) //rysowanie kwadratów wertykalnie
		{
			if ((i == 1) && (j == 1))
			{
				//w momencie gdy 'i' i 'j' równaj¹ siê 1 nie rysujemy kwadratu tym samym pozostawiaj¹c puste miejsce w œrodku wiêkszego kwadratu 
			}
			else
			{
				if (obec_poziom > zad_l_poziomów - 2)
				{
					rysuj_Kwadrat(x + i * (rozmiar / 3) + losuj_nierownosci(rozmiar), y - j * (rozmiar / 3) + losuj_nierownosci(rozmiar), rozmiar / 3); //w momencie, w którym osi¹gneliœmy zadan¹ .poziomów nastêpuje rysowanie kwadratów o rozmiarze w³aœciwym dla tej g³êbokoœci 
				}
				else
				{
					rysuj_Dywan_Sierpinskiego(x + i * (rozmiar / 3) + losuj_nierownosci(rozmiar), y - j * (rozmiar / 3) + losuj_nierownosci(rozmiar), rozmiar / 3, obec_poziom + 1); //do momentu w którym nie osi¹gniemy zadanej l.poziomów zmienjszamy rozmiar kwadratów (1/3 poprzedniego rozmiaru) 
				}
			}

		}

	}
}

void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT); // Czyszczenie okna aktualnym kolorem czyszcz¹cym
	rysuj_Dywan_Sierpinskiego(-50.0f, 50.0f, 100.0f, 0); // Wyrysowanie struktury o zadanych rozmiarach 
	glFlush(); // Przekazanie poleceñ rysuj¹cych do wykonania
}

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	GLfloat AspectRatio; // Deklaracja zmiennej AspectRatio okreœlaj¹cej proporcjê wymiarów okna

	if (vertical == 0) // Zabezpieczenie pzred dzieleniem przez 0
		vertical = 1;

	glViewport(0, 0, horizontal, vertical); // Ustawienie wielkoœciokna okna urz¹dzenia (Viewport). W tym przypadku od (0,0) do (horizontal, vertical)
	glMatrixMode(GL_PROJECTION); // Okreœlenie uk³adu wspó³rzêdnych obserwatora
	glLoadIdentity();  // Okreœlenie przestrzeni ograniczaj¹cej
	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical; // Wyznaczenie wspó³czynnika proporcji okna

	if (horizontal <= vertical)
		glOrtho(-100.0, 100.0, -100.0 / AspectRatio, 100.0 / AspectRatio, 1.0, -1.0);

	else
		glOrtho(-100.0 * AspectRatio, 100.0 * AspectRatio, -100.0, 100.0, 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW); // Okreœlenie uk³adu wspó³rzêdnych
	glLoadIdentity();
}

int main(int argc, char** argv)
{
	std::cout << "Wprowadz liczbe poziomow: ";
	std::cin >> zad_l_poziomów; //wprowadzenie liczby poziomów

	srand(time(NULL));
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);   // Ustawienie trybu wyœwietlania
	glutCreateWindow("Lab1 - Dywan Sierpiñskiego"); // Utworzenie okna i okreœlenie treœci napisu w nag³ówku okna
	glutDisplayFunc(RenderScene);				    // Okreœlenie, ¿e funkcja RenderScene bêdzie funkcj¹ zwrotn¹ (callback)
	glutReshapeFunc(ChangeSize);					// Dla aktualnego okna ustala funkcjê zwrotn¹ odpowiedzialn¹ za zmiany rozmiaru okna
	glutMainLoop();									// Funkcja uruchamia szkielet biblioteki GLUT
}