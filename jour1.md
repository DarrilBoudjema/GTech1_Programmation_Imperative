# Jour 1 — Syntaxe C++ et tableaux

## Objectifs

À la fin de la journée, tu dois pouvoir écrire et compiler un programme, utiliser les types fondamentaux, découper un traitement en fonctions et parcourir un tableau de taille fixe.

## 1. Écrire et compiler un programme

Un fichier source C++ porte généralement l'extension `.cpp`. Le compilateur transforme le code source en programme exécutable.

```cpp
#include <iostream>

int main() {
    std::cout << "Bonjour !" << '\n';
    return 0;
}
```

- `#include <iostream>` donne accès aux flux d'entrée et de sortie.
- `main` est le point d'entrée du programme.
- Les accolades délimitent un bloc d'instructions.
- Une instruction se termine généralement par un point-virgule.
- `std::cout` affiche du texte ; `<<` envoie une valeur vers ce flux.
- `\n` est un saut de ligne.
- `return 0` indique une fin normale du programme.
  
### Créer le premier projet

1. Ouvrir **Visual Studio 2022**, puis **Créer un nouveau projet**.
2. Filtrer sur **C++**, **Windows** et **Console**.
3. Choisir **Application console**, puis **Suivant**.
4. Nommer le projet `Jour1_CPP` et choisir un dossier, par exemple `C:\CoursCPP`.
5. Cliquer sur **Créer**. Dans l'**Explorateur de solutions**, ouvrir le fichier `.cpp` généré et remplacer son contenu par l'exemple ci-dessus.

Une solution organise un ou plusieurs projets ; chaque projet construit ici son propre exécutable. Pour chaque exercice indépendant, créer un projet console distinct ou remplacer le contenu du fichier courant. Ne pas conserver plusieurs fonctions `main` dans le même projet. Si plusieurs projets sont présents, clic droit sur celui à exécuter → **Définir comme projet de démarrage**.

### Réglages du projet

Clic droit sur le **projet**, puis **Propriétés**. Choisir **Toutes les configurations** et la plateforme **x64** pour les réglages communs suivants :

| Page | Réglage |
|---|---|
| C/C++ → Langage | Norme du langage C++ : **ISO C++17 (`/std:c++17`)** |

Cliquer sur **Appliquer**, puis **OK**. Dans la barre d'outils, travailler en **Debug** et **x64** pour les exercices. Les libellés peuvent légèrement varier avec la langue de l'installation ; les options entre parenthèses permettent de les reconnaître.

### Compiler, exécuter et déboguer

Les raccourcis suivants correspondent au profil habituel de Visual Studio. En cas de différence de profil, utiliser le menu indiqué.

| Action | Raccourci habituel | Menu ou usage |
|---|---|---|
| Enregistrer | `Ctrl+S` | Fichier → Enregistrer |
| Générer la solution | `Ctrl+Maj+B` | Générer → Générer la solution |
| Exécuter sans débogage | `Ctrl+F5` | Déboguer → Exécuter sans débogage |
| Démarrer/continuer le débogage | `F5` | Déboguer → Démarrer le débogage |
| Poser/enlever un point d'arrêt | `F9` | Ou cliquer dans la marge gauche |
| Exécuter la prochaine instruction sans entrer dans une fonction | `F10` | Pas à pas principal |
| Entrer dans une fonction | `F11` | Pas à pas détaillé |
| Arrêter le débogage | `Maj+F5` | Déboguer → Arrêter le débogage |

Les résultats et les saisies de `std::cin` apparaissent dans la **console du programme**, pas dans la fenêtre **Sortie** de l'IDE. Avec `Ctrl+F5`, Visual Studio permet normalement de lire le résultat avant de fermer la console.

En cas d'erreur, ouvrir **Affichage → Sortie**, choisir la sortie de génération et lire le premier diagnostic. La **Liste d'erreurs** peut mélanger diagnostics du compilateur et d'IntelliSense : vérifier le résultat réel de la génération. Ne pas lancer une ancienne version du programme après une génération échouée.

### Première observation avec le débogueur

Après l'exercice 1, poser un point d'arrêt sur le calcul, lancer avec `F5` et ouvrir **Déboguer → Fenêtres → Variables locales**. Avec `F10`, suivre les valeurs. La ligne surlignée est généralement la prochaine instruction à exécuter : observer le changement après l'avoir exécutée.

Les extraits sans `main` de ce cours illustrent des instructions à placer dans une fonction ; les définitions de fonctions se placent hors de `main`. Ils ne constituent pas chacun un programme complet.

Une erreur de compilation empêche de produire l'exécutable. Une erreur logique laisse le programme fonctionner, mais avec un résultat incorrect. Lis d'abord le premier message du compilateur : les suivants peuvent en découler.

## 2. Variables, types et opérateurs

Une variable possède un type, un nom et une valeur. Initialise-la dès sa déclaration.

```cpp
int age = 20;
double prix = 12.50;
char initiale = 'A';
bool inscrit = true;
const int LIMITE = 20;
```

| Type | Usage |
|---|---|
| `int` | Entier signé ; sa plage est limitée |
| `double` | Nombre à virgule, avec une précision limitée |
| `char` | Un caractère |
| `bool` | `true` ou `false` |
| `std::string` | Texte ; nécessite `<string>` |

`const` interdit de modifier la valeur à travers ce nom. Une chaîne utilise des guillemets doubles (`"Alice"`), un caractère des guillemets simples (`'A'`).

```cpp
int a = 5;
int b = 2;
std::cout << a / b << '\n';                      // 2
std::cout << static_cast<double>(a) / b << '\n'; // 2.5
```

Quand les deux opérandes sont des entiers, la division est entière. `%` donne le reste d'une division entière : `7 % 3` vaut `1`. Ne divise jamais par zéro.

| Catégorie | Opérateurs |
|---|---|
| Calcul | `+`, `-`, `*`, `/`, `%` |
| Comparaison | `==`, `!=`, `<`, `>`, `<=`, `>=` |
| Logique | `&&`, `\|\|`, `!` |
| Affectation | `=`, `+=`, `-=`, `*=`, `/=` |
| Incrémentation | `++`, `--` |

Attention : `=` affecte une valeur, `==` compare deux valeurs. Utilise des parenthèses pour rendre les calculs complexes explicites.

### Affichage et saisie sous Windows

Les textes des exemples utilisent souvent des caractères ASCII afin d'éviter un problème d'encodage qui détournerait l'attention du C++. Enregistrer les fichiers en UTF-8 est une bonne pratique ; l'affichage des accents dépend aussi de l'encodage de la console. Ce réglage peut être traité à part si nécessaire.

Pour les nombres décimaux saisis dans les exercices, utiliser le point, par exemple `12.5`. Sans configuration explicite de locale, `12,5` n'est pas la saisie attendue pour un `double`.

### Saisie

```cpp
int age = 0;
std::cout << "Age : ";
std::cin >> age;
```

`std::cin >>` attend une valeur du type demandé. Une saisie non numérique peut placer le flux en état d'échec. Pour les exercices de base, on suppose que les saisies ont le bon type ; on valide ensuite leur plage.

## 3. Conditions, boucles et portée

```cpp
if (age >= 18) {
    std::cout << "Majeur\n";
} else {
    std::cout << "Mineur\n";
}
```

Pour plusieurs cas, ajouter `else if`. Pour tester un intervalle :

```cpp
if (age >= 0 && age <= 120) {
    std::cout << "Age accepte\n";
}
```

Une boucle `for` convient à un nombre d'itérations connu :

```cpp
for (int i = 0; i < 5; ++i) {
    std::cout << i << '\n';
}
```

Une boucle `while` répète tant que sa condition est vraie :

```cpp
int note = -1;
while (note < 0 || note > 20) {
    std::cout << "Note entre 0 et 20 : ";
    std::cin >> note;
}
```

La condition doit pouvoir devenir fausse, sinon la boucle ne termine pas. Une variable déclarée dans un bloc n'est accessible que dans sa portée : le `i` du `for` précédent ne peut pas être utilisé après la boucle.

## 4. Fonctions et références

Une fonction regroupe un traitement réutilisable. Ses paramètres fournissent les données ; son type de retour décrit le résultat.

```cpp
int carre(int nombre) {
    return nombre * nombre;
}
```

Place sa définition avant son utilisation ou déclare un prototype avant `main`. Une fonction qui ne renvoie pas de résultat utilise `void`.

### Passage par valeur

```cpp
void modifierCopie(int nombre) {
    nombre = 100;
}
```

Le paramètre est une copie. Si `x` vaut `5`, `modifierCopie(x)` ne modifie pas `x`.

### Passage par référence

```cpp
void modifierOriginal(int& nombre) {
    nombre = 100;
}
```

Une référence est un autre nom de l'objet fourni. `modifierOriginal(x)` modifie donc `x`. Une référence doit être initialisée et ne peut pas être réaffectée à un autre objet. Le `&` a ici un rôle dans la déclaration du type ; demain, tu verras son rôle d'opérateur d'adresse.

## 5. Tableaux de taille fixe

Un tableau regroupe des éléments du même type, contigus en mémoire.

```cpp
int notes[5] = {12, 15, 9, 18, 11};
int compteurs[5] = {}; // Tous les éléments valent zéro.
```

Un tableau de cinq éléments a les indices `0`, `1`, `2`, `3`, `4`. L'expression `notes[5]` est hors limites : son évaluation entraîne un comportement indéfini. Cela ne signifie pas qu'une erreur visible sera forcément affichée.

```cpp
int somme = 0;
for (int i = 0; i < 5; ++i) {
    somme += notes[i];
}
double moyenne = static_cast<double>(somme) / 5;
```

La taille d'un tableau intégré local doit être une expression constante à la compilation en C++ standard. Une taille saisie par l'utilisateur sera traitée au jour 3.

### Deux dimensions

```cpp
int grille[2][3] = {{1, 2, 3}, {4, 5, 6}};
for (int ligne = 0; ligne < 2; ++ligne) {
    for (int colonne = 0; colonne < 3; ++colonne) {
        std::cout << grille[ligne][colonne] << ' ';
    }
    std::cout << '\n';
}
```

Le premier indice choisit la ligne ; le second choisit l'élément de cette ligne.

## Exercices pratiques

### Exercice 1 — Calculatrice

Demander deux `double`. Afficher somme, différence, produit et quotient. Si le deuxième nombre vaut zéro, afficher un message à la place du quotient.

**Tests :** `8` et `2` donnent `10`, `6`, `16`, `4`. Avec `8` et `0`, le programme refuse la division. Avec `5` et `2`, le quotient vaut `2.5`.

**Manipulation Visual Studio :** observer les deux nombres et le quotient dans **Variables locales**, en avançant avec `F10`.

**Critères :** variables initialisées, saisie correcte, aucune division par zéro.

### Exercice 2 — Boucles et validation

1. Demander un entier `n` entre 1 et 100 ; recommencer s'il est hors intervalle.
2. Afficher les entiers de 1 à `n`.
3. Calculer leur somme avec une boucle.
4. Afficher uniquement les nombres pairs.

**Tests :** pour `n = 5`, somme `15`, pairs `2 4`. Pour `n = 1`, somme `1`, aucun pair. Tester aussi `0` puis une valeur valide.

### Exercice 3 — Fonctions

Écrire et appeler :

```cpp
int carre(int nombre);
bool estPair(int nombre);
void echanger(int& a, int& b);
```

Utiliser une variable temporaire dans `echanger`. Tester `carre(4)`, `estPair(7)` et l'échange de `3` et `8`.

**Attendu :** `16`, `false`, puis `a = 8` et `b = 3`. Expliquer pourquoi l'échange demande ici des références.

### Exercice 4 — Statistiques de notes

Saisir cinq notes entières entre 0 et 20 dans un tableau. Afficher les notes, leur moyenne, le minimum, le maximum et le nombre de notes au moins égales à 10.

**Test :** `12 15 9 18 11` donne moyenne `13`, minimum `9`, maximum `18`, quatre notes au moins égales à 10.

**Indice :** initialiser minimum et maximum avec le premier élément. Ne pas initialiser systématiquement le minimum à zéro.

## Challenge — Analyse d'une grille

**Pour ceux qui terminent en avance ; environ 20 à 40 min selon les extensions.**

Créer une grille `int grille[3][3]`. Saisir neuf nombres compris entre -100 et 100, puis :

1. Afficher la grille en trois lignes.
2. Calculer la somme de chaque ligne et de chaque colonne.
3. Afficher la somme de la diagonale principale.
4. Dire si chaque ligne et chaque colonne possède la même somme que la première ligne.

**Test de référence :** la grille `8 1 6 / 3 5 7 / 4 9 2` donne `15` pour chaque ligne, colonne et diagonale principale.

**Bonus :** vérifier aussi la seconde diagonale et l'absence de doublons. Pour qualifier un carré magique normal d'ordre 3, vérifier en plus que les nombres sont exactement ceux de 1 à 9.

**Réussite :** boucles imbriquées, indices valides, résultats corrects. Ne pas écrire neuf traitements presque identiques.

## Documentation Microsoft

- [Créer un projet console C++](https://learn.microsoft.com/fr-fr/cpp/build/vscpp-step-1-create?view=msvc-170)
- [Choisir la norme C++](https://learn.microsoft.com/fr-fr/cpp/build/reference/std-specify-language-standard-version?view=msvc-170)
- [Utiliser les points d’arrêt](https://learn.microsoft.com/fr-fr/visualstudio/debugger/using-breakpoints?view=vs-2022)
