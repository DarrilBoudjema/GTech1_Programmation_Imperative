#include <iostream>

/********************EXERCICE 2*********************/
//void echanger(int* a, int* b) {
//	if (a != nullptr && b != nullptr) {
//		int temp = *a;
//		*a = *b;
//		*b = temp;
//	}
//}
/***************************************************/
/********************EXERCICE 3*********************/
//int somme(const int* valeurs, int taille) {
//    int total = 0;
//    for (int i = 0; i < taille; ++i) {
//        total += valeurs[i];
//    }
//    return total;
//}
//
//
//
//int maximum(const int* valeurs, int taille) {
//    int max = 0;
//    for (int i = 0; i < taille; ++i) {
//        if (max < valeurs[i]) max = valeurs[i];
//    }
//    return max;
//}
//void inverser(int* valeurs, int taille) {
//    for (int i = 0; i < taille / 2; ++i) {
//        int indiceOppose = taille - 1 - i;
//        int temp = valeurs[i];
//        valeurs[i] = valeurs[indiceOppose];
//        valeurs[indiceOppose] = temp;
//    }
//}
/***************************************************/
/***********************EXECICE 4*******************/
//int* rechercher(int* valeurs, int taille, int cible, int nouvelleValeur) {
//    for (int i = 0; i < taille; ++i) {
//        if (valeurs[i] == cible) {
//            valeurs[i] = nouvelleValeur;
//            return &valeurs[i];
//        }
//    }
//    std::cout << "Valeur absente \n";
//    return nullptr;
//}
/***************************************************/

int main()
{
/***********************EXECICE 2*******************/
    //int a = 5;
    //int b = 10;

    //echanger(&a, &b);
    //std::cout << a << " " << b;
/***************************************************/
/***********************EXECICE 3*******************/
    //int tabl[5] = { 4, -2, 9, 1, 5 };
    //std::cout << somme(&tabl[0], 5) << "\n" << maximum(&tabl[0], 5) << "\n";
    //inverser(&tabl[0], 5);
    //for (int i = 0; i < 5; ++i) {
    //    std::cout << tabl[i] << ", ";
    //}
/***************************************************/
/***********************EXECICE 4*******************/
    //int tabl[5] = { 10, 25, 30, 25, 50 };
    //rechercher(&tabl[0], 5, 25, 99);
    //std::cout << tabl[1];
/***************************************************/
}
