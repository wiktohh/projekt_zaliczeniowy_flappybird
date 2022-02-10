#include <iostream>
#include <vector>
#include <SFML/graphics.hpp>
#include <fstream>
#include <random>


using namespace std;
using namespace sf;

struct Grafika {
	Texture ptak;
	Texture rura;
	Texture tlo;
} grafika;

struct Ptak {
	double v = 0;
	Sprite sprite;
} ptak;

enum statusGry {
	waiting,
	started,
	gameover
};

struct Gra {
	int wynik{};
	int najwyzszyWynik{};
	int frames{};
	statusGry statusGry;
	Sprite tlo[3];
	Sprite gameover;
	Text wcisnijSpacje;
	Text wynikText;
	Text najwyzszyWynikText;
	Font font;
} gra;


vector<Sprite> rury;

bool kolizje(float, float, float, float, float, float, float, float);
void tekstury();
void wyswietlanieWyniku();
void zapis();
void odczyt();




int main()
{
	RenderWindow window(VideoMode(1000, 600), "Flappy bird"); //renderowanie okna aplikacji
	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);

	odczyt();
	tekstury();

	while (window.isOpen())
	{

		wyswietlanieWyniku();

		float fx = ptak.sprite.getPosition().x;
		float fy = ptak.sprite.getPosition().y;
		float fw = ptak.sprite.getScale().x * 34;
		float fh = ptak.sprite.getScale().y * 24;

		if (gra.statusGry == started)
		{
			ptak.sprite.move(0, ptak.v);
			ptak.v += 0.5;
			if (fy < 0) {
				ptak.sprite.setPosition(250, 0);
				ptak.v = 0;
			}
			else if(fy>600){ //wykrywanie kolizji z podloga
				ptak.v = 0;
				gra.statusGry = gameover;
			}
		}

		for (vector<Sprite>::iterator itr = rury.begin(); itr != rury.end(); itr++) {
			if (gra.statusGry == started && (*itr).getPosition().x == 250) //liczenie punktow
			{
				gra.wynik++;
				if (gra.wynik > gra.najwyzszyWynik) {
					zapis();
					gra.najwyzszyWynik = gra.wynik;
				}
				break;
			}
		}
		if (gra.statusGry == started && gra.frames % 100 == 0) {
			mt19937 generator(time(nullptr));
			uniform_int_distribution <int > distribution(75, 275);
			int r = distribution(generator); //losowe generowanie rur
			const int gap = 200;

			Sprite ruraDol;
			ruraDol.setTexture(grafika.rura);
			ruraDol.setPosition(1000, r + gap);
			ruraDol.setScale(2, 2);

			Sprite ruraGora;
			ruraGora.setTexture(grafika.rura);
			ruraGora.setPosition(1000, r);
			ruraGora.setScale(2, -2);

			rury.push_back(ruraDol);
			rury.push_back(ruraGora);
		}
		//animacja ruchu planszy
		if (gra.statusGry == started) {
			for (vector<Sprite>::iterator itr = rury.begin(); itr != rury.end(); itr++) {
				(*itr).move(-3, 0);
			}
		}
		if (gra.frames % 100 == 0) {
			vector<Sprite>::iterator startitr = rury.begin();
			vector<Sprite>::iterator enditr = rury.begin();

			for (enditr; enditr != rury.end(); enditr++) {
				if ((*enditr).getPosition().x > -104) {
					break;
				}
			}

			rury.erase(startitr, enditr);
		}

		if (gra.statusGry == started) {
			for (vector<Sprite>::iterator itr = rury.begin(); itr != rury.end(); itr++) {

				float px, py, pw, ph;

				if ((*itr).getScale().y > 0) {
					px = (*itr).getPosition().x;
					py = (*itr).getPosition().y;
					pw = 52 * (*itr).getScale().x;
					ph = 320 * (*itr).getScale().y;
				}
				else {
					pw = 52 * (*itr).getScale().x;
					ph = -320 * (*itr).getScale().y;
					px = (*itr).getPosition().x;
					py = (*itr).getPosition().y - ph;
				}

				if (kolizje(fx, fy, fw, fh, px, py, pw, ph)) {
					gra.statusGry = gameover;
				}
			}
		}

		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}
			//sterowanie
			else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left){
				if (gra.statusGry == waiting) {
					gra.statusGry = started;
				}

				if (gra.statusGry == started) {
					ptak.v = -8;
				}

			
			}
			else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space && gra.statusGry == gameover) {
				gra.statusGry = waiting;
				ptak.sprite.setPosition(250, 300);
				gra.wynik = 0;
				rury.clear();
			}
		}

	
		window.clear();
		window.draw(gra.tlo[0]);
		window.draw(gra.tlo[1]);
		window.draw(gra.tlo[2]);
		window.draw(ptak.sprite);


		//rysowanie rur
		for (vector<Sprite>::iterator itr = rury.begin(); itr != rury.end(); itr++) {
			window.draw(*itr);
		}

		
		window.draw(gra.wynikText);
		window.draw(gra.najwyzszyWynikText);

	
		if (gra.statusGry == gameover) {
			window.draw(gra.gameover);
			window.draw(gra.wcisnijSpacje);
		}
		window.display();

	
		gra.frames++;
	}

	return 0;
}

//sprawdzanie kolizji ptaka z rura
bool kolizje(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {
	if (x1 + w1 >= x2 && x1 <= x2 + w2 && y1 + h1 >= y2 && y1 <= y2 + h2) {
		return true;
	}
	return false;
}
void tekstury() {
	//nadanie obiektom tekstury
	grafika.tlo.loadFromFile("../grafika/background.png");
	grafika.rura.loadFromFile("../grafika/pipe.png");
	grafika.ptak.loadFromFile("../grafika/flappy1.png");
	ptak.sprite.setPosition(250, 300);
	ptak.sprite.setScale(2, 2);
	gra.font.loadFromFile("../czcionka/flappy.ttf");
	gra.tlo[0].setTexture(grafika.tlo);
	gra.tlo[1].setTexture(grafika.tlo);
	gra.tlo[2].setTexture(grafika.tlo);
	gra.tlo[0].setScale(1.3, 2.5);
	gra.tlo[1].setScale(1.3, 2.5);
	gra.tlo[2].setScale(1.3, 2.5);
	gra.tlo[1].setPosition(333, 0);
	gra.tlo[2].setPosition(666, 0);
	gra.wcisnijSpacje.setString("Wcisnij spacje, aby kontynuowac");
	gra.wcisnijSpacje.setFont(gra.font);
	gra.wcisnijSpacje.setFillColor(Color::Black);
	gra.wcisnijSpacje.setCharacterSize(50);
	gra.wcisnijSpacje.setOrigin(gra.wcisnijSpacje.getLocalBounds().width / 2, 0);
	gra.wcisnijSpacje.setPosition(500, 250);
	gra.wynikText.setFont(gra.font);
	gra.wynikText.setCharacterSize(50);
	gra.wynikText.setFillColor(Color::Black);
	gra.wynikText.move(30, 0);
	gra.najwyzszyWynikText.setFont(gra.font);
	gra.najwyzszyWynikText.setCharacterSize(30);
	gra.najwyzszyWynikText.setFillColor(Color::Black);
	gra.najwyzszyWynikText.move(30, 60);
	gra.statusGry = waiting;
	ptak.sprite.setTexture(grafika.ptak);

}
void wyswietlanieWyniku()
{
	gra.wynikText.setString("Aktualny wynik: " + to_string(gra.wynik));
	gra.najwyzszyWynikText.setString("Najwyzszy wynik: " + to_string(gra.najwyzszyWynik));
}

//zapis do pliku najwyzszego wyniku
void zapis()
{
	ofstream plik;
	plik.open("wynik.txt");
	if (plik.good()){
		plik << gra.wynik << endl;
	}
	plik.close();
}

//odczyt do pliku najwyzszego wyniku
void odczyt()
{
	ifstream plik;
	int liczba;
	plik.open("wynik.txt");
	if (plik.good()){
		plik >> liczba;
		gra.najwyzszyWynik = liczba;
	}
	plik.close();
}
