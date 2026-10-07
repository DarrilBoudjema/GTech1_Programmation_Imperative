# Jour 3 — Pile, tas et allocation dynamique

## Objectifs

Distinguer portée et durée de vie, expliquer pile et tas, allouer et libérer un objet ou un tableau et identifier les principales erreurs mémoire. Découvrir ensuite les outils de gestion automatique du C++ moderne.

## 1. Portée et durée de vie

La **portée** indique où un nom peut être utilisé dans le code. La **durée de vie** indique pendant quelle période un objet existe. Ce sont deux notions différentes.

```cpp
void exemple() {
    int nombre = 10;
    {
        int temporaire = 20;
        std::cout << nombre + temporaire << '\n';
    } // Fin de vie de temporaire.
} // Fin de vie de nombre.
```

Les variables locales ordinaires ont une durée de stockage automatique : leur destruction se produit lorsque leur bloc se termine. Un pointeur ne prolonge pas la durée de vie de l'objet qu'il désigne.

### Exemple à analyser, sans déréférencement

```cpp
int* p = nullptr;
{
    int nombre = 42;
    p = &nombre;
}
```

De même, renvoyer l'adresse d'une variable locale d'une fonction produit un pointeur qui ne permet plus d'accéder à un objet vivant après le retour.

## 2. La pile et le tas

La **pile** sert généralement à gérer les appels de fonctions et leur stockage local. Le **tas** fournit généralement le stockage des allocations dynamiques. Le langage définit les durées de stockage ; le placement physique exact dépend de l'implémentation et des optimisations.

```cpp
int local = 10;
int* p = new int{42};
```

Dans ce bloc, `local` et le pointeur `p` ont une durée de stockage automatique. L'entier créé par `new` a une durée de stockage dynamique. La destruction du pointeur brut `p` ne libère pas automatiquement cet entier.

| Question | Stockage automatique | Stockage dynamique |
|---|---|---|
| Exemple | `int x = 10;` | `new int{10}` |
| Qui décide de la fin de vie ? | La sortie du bloc | Le code propriétaire, directement ou via un objet |
| Tableau intégré de taille choisie à l'exécution ? | Non en C++ standard | Oui avec `new[]` |
| Libération explicite nécessaire ? | Non | Oui pour une allocation détenue par un pointeur brut |

Toutes les données ne relèvent pas uniquement de ces deux catégories : il existe notamment une durée de stockage statique, par exemple pour certaines variables globales. Aujourd'hui, on se concentre sur stockage automatique et dynamique.

## 3. Allouer et libérer un objet

```cpp
int* nombre = new int{42};
std::cout << *nombre << '\n';
*nombre = 50;
delete nombre;
nombre = nullptr;
```

`new` réserve le stockage et initialise l'objet. `delete` détruit l'objet et libère le stockage correspondant. Par défaut, un échec d'allocation par `new` lève une exception, plutôt que de renvoyer `nullptr`.

Utiliser `new int{}` pour initialiser à zéro. `new int` sans initialiseur laisse la valeur de cet entier indéterminée : ne pas la lire avant une affectation.

### Propriété et alias

Le **propriétaire** est responsable de la libération. Un pointeur brut n'indique pas automatiquement s'il est propriétaire ou simple observateur.

```cpp
int* proprietaire = new int{7};
int* observateur = proprietaire;
delete proprietaire;
proprietaire = nullptr;
// observateur est maintenant pendant.
observateur = nullptr;
```

Mettre un pointeur à `nullptr` ne met pas les autres copies à jour. Ne pas libérer l'objet une seconde fois via `observateur`.

## 4. Erreurs mémoire courantes

| Erreur | Cause | Prévention |
|---|---|---|
| Fuite | Perte du dernier accès propriétaire sans libération | Identifier un propriétaire et libérer sur chaque chemin |
| Utilisation après libération | Accès à un objet déjà détruit | Ne plus utiliser les pointeurs observateurs après destruction |
| Double libération | Deux `delete` sur la même allocation | Une seule responsabilité de libération |
| Hors limites | Indice invalide | Conserver la taille et vérifier les indices |
| Mauvaise forme de libération | Mélange de `new[]` et `delete` | Faire correspondre les formes |

Une fuite peut se produire en écrasant le pointeur propriétaire :

```cpp
int* p = new int{10};
p = nullptr; // Allocation perdue : exemple incorrect.
```

Ne pas utiliser `delete` sur l'adresse d'une variable locale. Ne pas mélanger les allocations C++ avec `malloc`/`free`.

## 5. Tableaux dynamiques

La taille peut être choisie à l'exécution :

```cpp
int taille = 0;
std::cin >> taille;
if (taille < 1 || taille > 1000) {
    std::cout << "Taille invalide\n";
    return 1;
}
int* valeurs = new int[taille]{};
for (int i = 0; i < taille; ++i) {
    valeurs[i] = i * 10;
}
delete[] valeurs;
valeurs = nullptr;
```

Les accolades initialisent tous les éléments à zéro. Le pointeur ne mémorise pas la taille : il faut la conserver séparément.

| Allocation | Libération correspondante |
|---|---|
| `new int{42}` | `delete p` |
| `new int[taille]{}` | `delete[] p` |

Une fois alloué, ce tableau conserve sa taille. Pour l'agrandir, il faut allouer un autre tableau, copier, puis libérer l'ancien.

```cpp
int* agrandir(const int* ancien, int taille) {
    int* nouveau = new int[taille + 1]{};
    for (int i = 0; i < taille; ++i) {
        nouveau[i] = ancien[i];
    }
    return nouveau;
}
```

**Contrat pédagogique :** `0 <= taille < 1000`, et tableau valide lorsque la taille est positive. La fonction renvoie une nouvelle allocation ; l'appelant en devient propriétaire.

```cpp
int* nouveau = agrandir(valeurs, taille);
delete[] valeurs;
valeurs = nouveau;
++taille;
```

Les pointeurs qui désignaient des éléments de l'ancien tableau deviennent invalides après sa libération.

## 6. Diagnostic

Avec GCC ou Clang lorsque ces outils sont disponibles :

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -g \
    -fsanitize=address,undefined main.cpp -o programme
./programme
```

AddressSanitizer et UndefinedBehaviorSanitizer peuvent détecter de nombreux accès invalides ; la détection des fuites dépend aussi de la plateforme. Ils ne constituent pas une preuve que toutes les erreurs sont absentes. Ne fais pas d'accès volontairement invalide dans le programme final.

## 7. Gestion automatique : RAII

Le principe **RAII** consiste à confier une ressource à un objet qui la libère à sa destruction. Cela évite d'oublier une libération lors d'un retour anticipé ou d'une exception.

### `std::vector` : tableau de taille variable

```cpp
#include <vector>

std::vector<int> valeurs(5, 0);
valeurs[0] = 10;
valeurs.push_back(20);
std::cout << valeurs.size() << '\n'; // 6
```

Le vecteur conserve sa taille et gère ses allocations. `valeurs[i]` ne vérifie pas les limites ; `valeurs.at(i)` les vérifie et lève une exception si l'indice est invalide. Une opération qui provoque une réallocation invalide les pointeurs vers ses anciens éléments.

### `std::unique_ptr` : propriétaire unique

```cpp
#include <memory>

auto nombre = std::make_unique<int>(42);
std::cout << *nombre << '\n';
```

L'objet pointé est détruit automatiquement avec son propriétaire. Ne pas appeler `delete` sur cette allocation. `unique_ptr` n'est pas copiable ; le transfert de propriété existe, mais dépasse les objectifs obligatoires du jour.

## Exercices pratiques

### Exercice 1 — Durées de vie

Sur papier, annoter ces situations :

1. Une variable locale dans `main`.
2. Une variable locale dans un bloc imbriqué.
3. Un pointeur local vers une variable du bloc extérieur.
4. Un pointeur local contenant le résultat de `new int{5}`.
5. Un pointeur conservé après destruction de sa cible.

Pour chacune : indiquer les objets, leur durée de vie, le moment où un accès devient invalide et qui doit libérer une éventuelle allocation. Écrire un petit programme qui affiche les adresses uniquement pendant la vie des objets.

### Exercice 2 — Objet dynamique

Allouer un entier initialisé à zéro, demander sa valeur, la doubler et l'afficher. Utiliser une fonction `void doubler(int* valeur)`. Limiter la saisie à l'intervalle `[-1000, 1000]`, puis libérer l'objet.

Créer temporairement un second pointeur vers cet entier pour observer que les deux accès désignent le même objet. Après libération, ne déréférencer aucun des deux.

**Test :** `21` donne `42`. **Réussite :** une allocation, une libération, aucune utilisation après libération.

### Exercice 3 — Notes en quantité variable

1. Demander un nombre de notes entre 1 et 100.
2. Allouer un tableau dynamique de `double`.
3. Saisir des notes entre 0 et 20.
4. Calculer moyenne, minimum et maximum avec des fonctions.
5. Libérer le tableau avant de terminer.

**Test :** quatre notes `10 12 14 16` donnent moyenne `13`, minimum `10`, maximum `16`. Tester une seule note et une taille refusée.

### Exercice 4 — Corriger puis moderniser

Analyser ce fragment incorrect sans l'intégrer au programme final :

```cpp
int* valeurs = new int[3]{1, 2, 3};
std::cout << valeurs[3];
delete valeurs;
std::cout << valeurs[0];
```

Identifier trois erreurs et proposer une correction : borne valide, `delete[]`, absence d'accès après destruction. Compiler la correction avec les outils de diagnostic si disponibles.

Réécrire ensuite l'exercice 3 avec `std::vector<double>`. Expliquer quelles responsabilités disparaissent et lesquelles restent : validation des saisies et respect des indices restent nécessaires.

## Challenge — Construire un tableau extensible

Créer une collection d'entiers avec `int* donnees`, `int taille` et `int capacite`, sans `std::vector` pour cette première version.

1. Commencer avec une capacité de 2 et une taille de 0.
2. Ajouter les nombres de 1 à 10.
3. Lorsque le tableau est plein, doubler sa capacité : allouer, copier les `taille` éléments, libérer l'ancien tableau.
4. Afficher les éléments, la taille et la capacité.
5. Libérer l'allocation finale.

**Limite du challenge :** au plus 100 éléments ; ce n'est pas une bibliothèque générale de gestion mémoire.

**Attendu :** valeurs `1` à `10`, taille `10`, capacité `16`. Les emplacements non utilisés ne comptent pas dans la taille.

**Bonus :** ajouter la suppression du dernier élément, sans réduction de capacité. Refuser cette opération sur une collection vide. Expliquer pourquoi doubler la capacité évite de réallouer à chaque ajout.

**Critères :** distinguer taille et capacité, copier seulement les éléments utilisés, ne jamais perdre une allocation, ne pas conserver d'adresse vers un ancien tableau.
