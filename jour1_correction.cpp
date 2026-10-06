#include <iostream>

/********************** FONCTION EXERCICE 3
//int carre(int nombre) {
//	return nombre * nombre;
//}
//
//bool estPair(int nombre) {
//	return nombre % 2 == 0;
//}
//void echanger(int& a, int& b) {
//	int temp = a;
//	a = b;
//	b = temp;
//}
**************************************************/

int main()
{

	// EXERCICE 1
	//double a = 0.0;
	//double b = 0.0;
	//std::cout << "Premier nombre \n";
	//std::cin >> a;
	//std::cout << "Deuxieme nombre \n";
	//std::cin >> b;
	//std::cout << "Somme: " << a + b << "\n" << a - b << "\n" << a * b << "\n";
	//if (b == 0.0) std::cout << "Division par 0";
	//else std::cout << a / b;

	// EXERCICE 2
	//int n = 0;

	//while (n < 1 || n > 100) {
	//	std::cin >> n;
	//}
	//int somme = 0;
	//for (int i = 1; i <= n; ++i){
	//	std::cout << i << "\n";
	//	somme += i;
	//
	//}

	//std::cout << somme << "\n";

	//for (int i = 1; i <= n; ++i) {
	//	if(i%2 == 0)
	//		std::cout << i << "\n";
	//}

	// EXERCICE 3
	//int a = 3;
	//int b = 8;
	//echanger(a, b);
	//std::cout << carre(4) << "\n" << estPair(7) << "\n" << a << ' ' << b;
	
	// EXERCICE 4
	/*const int nombreNotes = 5;
	int notes[nombreNotes] = {};
	
	for (int i = 0; i < nombreNotes; ++i) {
		int note = -1;
		while (note < 0 || note > 20) {
			std::cin >> note;
			notes[i] = note;
		}
	}
	
	int moyenne = 0;
	int max = notes[0];
	int min = notes[0];
	int nombreSupDix = 0;
	
	for (int i = 0; i < nombreNotes; ++i) {
		std::cout << notes[i] << " ";
		if (max < notes[i]) max = notes[i];
		if (min > notes[i]) min = notes[i];
		if (notes[i] >= 10) nombreSupDix++;
		moyenne += notes[i];
	}
	moyenne /= nombreNotes;
	std::cout << "\n" << min << " " << max << " " << nombreSupDix << " " << moyenne;*/

	
	return 0;
}


