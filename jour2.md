# Jour 2 — Pointeurs, adresses et tableaux


## Préparer la séance dans Visual Studio 2022

Créer un projet **Application console** nommé `Jour2_CPP` (ou plusieurs projets, un par exercice). Reprendre les réglages du jour 1 : C++17 (`/std:c++17`), avertissements `/W4`, **Debug x64**. Un seul `main` par projet ; définir le bon projet de démarrage. `Ctrl+Maj+B` génère, `Ctrl+F5` exécute sans débogage et `F5` lance le débogueur.

Les extraits partiels sont à intégrer dans un programme : instructions dans `main` ou une fonction, définitions de fonctions hors de `main`, en-têtes nécessaires en haut du fichier.

## Objectifs

Comprendre une adresse mémoire, lire et modifier un objet à travers un pointeur, transmettre des tableaux à des fonctions et reconnaître un accès invalide.

## 1. Valeurs et adresses

Une variable désigne un objet. Cet objet possède un emplacement en mémoire ; l'opérateur `&` permet d'obtenir son adresse.

```cpp
int nombre = 42;
std::cout << nombre << '\n';
std::cout << &nombre << '\n';
```

L'adresse affichée varie selon l'exécution. Ne la mémorise pas et ne la traite pas comme une valeur constante du programme.

Un pointeur contient une adresse permettant de désigner un objet, ou une valeur spéciale telle que `nullptr`.

```cpp
int* p = &nombre;
```

Le type `int*` signifie « pointeur vers un entier ». Le pointeur est lui-même un objet : il a une valeur, qui est l'adresse contenue, et sa propre adresse, obtenue avec `&p`.

| Expression | Signification |
|---|---|
| `nombre` | Valeur de l'entier |
| `&nombre` | Adresse de l'entier |
| `p` | Adresse contenue dans le pointeur |
| `*p` | Entier désigné par le pointeur |
| `&p` | Adresse du pointeur lui-même |

## 2. Déréférencer et rediriger

Déréférencer consiste à accéder à l'objet désigné, avec `*`.

```cpp
int a = 10;
int b = 20;
int* p = &a;

*p = 15; // Modifie a.
p = &b;  // Modifie p, qui désigne maintenant b.
*p = 25; // Modifie b.
```

À la fin, `a` vaut `15` et `b` vaut `25`. Distingue toujours une affectation au pointeur (`p = ...`) d'une affectation à l'objet (`*p = ...`).

### Les différents sens de `*` et `&`

| Écriture | Rôle |
|---|---|
| `int* p` | Déclaration d'un pointeur |
| `*p` | Déréférencement |
| `a * b` | Multiplication |
| `int& r = a` | Déclaration d'une référence |
| `&a` | Obtention de l'adresse |

Écris une déclaration de pointeur par ligne pour éviter l'ambiguïté de `int* p, q;` : dans cette déclaration, seul `p` est un pointeur.

## 3. `nullptr` et validité

```cpp
int* p = nullptr;
if (p != nullptr) {
    std::cout << *p << '\n';
}
```

`nullptr` signifie ici que le pointeur ne désigne aucun objet. Il ne faut pas le déréférencer. Ne laisse pas non plus un pointeur local non initialisé.

**Attention :** être non nul ne suffit pas à être valide. L'objet peut avoir été détruit, ou le pointeur peut désigner une position hors limites. Un accès invalide entraîne un comportement indéfini : plantage possible, résultat incohérent ou absence apparente de problème.

## 4. Fonctions et pointeurs

```cpp
void doubler(int* valeur) {
    if (valeur != nullptr) {
        *valeur *= 2;
    }
}
```

Appel :

```cpp
int nombre = 7;
doubler(&nombre); // nombre vaut 14.
doubler(nullptr); // Aucun effet, conformément au contrat choisi.
```

L'adresse est passée par valeur : la fonction reçoit une copie du pointeur, mais cette copie désigne le même entier.

```cpp
void rediriger(int* p, int* destination) {
    p = destination;
}
```

Cette fonction ne change pas le pointeur de l'appelant. Elle change seulement sa copie locale. Il faudrait une référence au pointeur (`int*&`) pour modifier ce pointeur ; cette technique est optionnelle aujourd'hui.

| Paramètre | Appel | Effet possible sur l'objet original |
|---|---|---|
| `int x` | `f(nombre)` | Aucun via la copie |
| `int& x` | `f(nombre)` | Modification de l'objet |
| `int* x` | `f(&nombre)` | Modification par déréférencement |

Choisir une référence quand un objet doit être fourni ; un pointeur peut aussi exprimer une absence, si le contrat de la fonction le prévoit.

## 5. Pointeurs et `const`

```cpp
int valeur = 10;
const int* lecture = &valeur;
int* const fixe = &valeur;
```

- `const int*` : interdit de modifier l'entier à travers ce pointeur ; le pointeur peut changer de cible.
- `int* const` : interdit de changer la cible ; l'entier peut être modifié.
- `const int* const` : interdit les deux opérations à travers ce nom.

```cpp
// *lecture = 20; // Refusé par le compilateur.
*fixe = 20;       // Autorisé.
```

`const int*` n'affirme pas que l'objet ne peut jamais changer : il peut changer par un autre accès autorisé.

## 6. Tableaux et pointeurs

```cpp
int valeurs[4] = {10, 20, 30, 40};
int* p = valeurs;
```

Dans cette expression, le tableau est converti en pointeur vers son premier élément. On pourrait écrire `int* p = &valeurs[0];`.

```cpp
std::cout << p[2] << '\n';       // 30
std::cout << *(p + 2) << '\n';   // 30
```

Un tableau n'est pas un pointeur. Le tableau contient les éléments ; le pointeur contient une adresse. `sizeof(valeurs)` mesure le stockage de tout le tableau, tandis que `sizeof(p)` mesure celui du pointeur.

### Arithmétique des pointeurs

`p + 1` avance d'un élément du type pointé, et non d'un octet. Dans un même tableau, on peut déplacer un pointeur entre les éléments et former l'adresse juste après le dernier élément. Cette dernière adresse ne doit pas être déréférencée.

```cpp
for (int* courant = valeurs; courant != valeurs + 4; ++courant) {
    std::cout << *courant << ' ';
}
```

Ne forme pas de pointeur au-delà de cette plage, ni avant le premier élément. Les règles de calcul et de comparaison ne permettent pas de traiter des objets sans rapport comme un tableau unique.

## 7. Transmettre un tableau

```cpp
int somme(const int* valeurs, int taille) {
    int total = 0;
    for (int i = 0; i < taille; ++i) {
        total += valeurs[i];
    }
    return total;
}
```

**Contrat :** `taille >= 0` et, si elle est positive, `valeurs` désigne au moins `taille` entiers vivants. Pour ces exercices, les valeurs restent assez petites pour éviter un dépassement de la capacité de `int`.

La fonction ne reçoit pas automatiquement la taille. Dans un paramètre de fonction, `int valeurs[]` est ajusté en `int* valeurs`. Utiliser `sizeof` sur ce paramètre ne permet pas de retrouver le nombre d'éléments.

## 8. Renvoyer un pointeur sur un élément

```cpp
int* rechercher(int* valeurs, int taille, int cible) {
    for (int i = 0; i < taille; ++i) {
        if (valeurs[i] == cible) {
            return &valeurs[i];
        }
    }
    return nullptr;
}
```

Le pointeur renvoyé reste utilisable tant que l'élément du tableau existe et conserve une adresse valide. La fonction renvoie le premier élément correspondant. Elle ne crée pas un nouvel entier.

## Atelier Visual Studio — Observer les adresses

Intégrer l'exemple avec `a`, `b` et `p` dans un `main` avec `<iostream>`. Poser un point d'arrêt sur `*p = 15`, après l'initialisation du pointeur.

1. Lancer avec `F5`.
2. Ouvrir **Déboguer → Fenêtres → Espion → Espion 1**.
3. Ajouter `a`, `b`, `p`, `*p`, `&a`, `&b` et `&p`.
4. Avancer avec `F10` et expliquer ce qui change lors de `*p = 15`, puis `p = &b`.
5. Dans l'exercice d'échange, utiliser `F11` pour entrer dans ta fonction et comparer les adresses du paramètre et de l'objet pointé.

Ne demander `*p` dans Espion que lorsque `p` désigne un objet vivant. Retirer l'expression si le pointeur devient nul ou invalide. Une adresse affichée par l'IDE ne garantit pas la validité de l'objet.

En configuration x64 de MSVC, `sizeof(int*)` vaut 8 et `sizeof(int)` vaut 4. Pour `int valeurs[4]`, `sizeof(valeurs)` vaut donc 16. Ces valeurs caractérisent cette cible ; le code C++ général ne doit pas supposer ces tailles sur toutes les plateformes. Les adresses et leur format peuvent changer entre deux exécutions.

## Exercices pratiques

### Exercice 1 — Suivre un pointeur (30 min)

1. Déclarer `a = 10`, `b = 20` et `p = &a`.
2. Afficher `a`, `b`, `p` et `*p`.
3. Faire `*p += 5`, puis `p = &b`, puis `*p *= 2`.
4. Avant d'exécuter, prédire les valeurs finales.
5. Comparer `p == &b` et expliquer le résultat.

**Manipulation Visual Studio :** relever les valeurs dans **Espion 1** à chaque étape.

**Attendu :** `a = 15`, `b = 40`, comparaison vraie. Aucun accès à un pointeur non initialisé.

### Exercice 2 — Échange par pointeurs (30 min)

Écrire `void echanger(int* a, int* b)`. Si l'un des pointeurs est nul, ne rien faire. Sinon échanger les entiers désignés.

**Tests :** deux entiers différents ; deux valeurs égales ; `echanger(&x, &x)` ; `echanger(nullptr, &x)`.

**Attendu :** échange correct, objet inchangé dans les deux derniers cas. Comparer avec la version par références du jour 1.

### Exercice 3 — Fonctions sur tableaux (30 min)

Écrire `somme`, `maximum` et `inverser` avec pointeur et taille.

- `maximum` exige un tableau non vide : vérifier ce préalable dans l'appelant.
- `inverser` accepte aussi un tableau vide et un tableau d'un élément.
- Aucun accès hors limites ; échanger les éléments symétriques.

**Test :** `{4, -2, 9, 1}` donne somme `12`, maximum `9` et inversion `{1, 9, -2, 4}`. Tester aussi `{-8, -3, -10}` pour le maximum.

### Exercice 4 — Recherche et modification (30 min)

Utiliser un tableau `{10, 25, 30, 25, 50}`. Écrire la recherche ci-dessus, puis demander une valeur à chercher.

- Si elle existe, afficher son indice (`resultat - valeurs`) et proposer sa nouvelle valeur.
- Sinon afficher « Valeur absente ».
- Afficher le tableau après l'opération.

**Tests :** `25` doit sélectionner l'indice `1` ; `99` est absent. Ne jamais calculer l'indice à partir de `nullptr`.

## Challenge — Rotation d'un tableau sur place

Implémenter :

```cpp
void rotationDroite(int* valeurs, int taille, int decalage);
```

**Contrat :** taille et décalage non négatifs ; tableau valide si la taille est positive. Une taille nulle ne produit aucun effet.

La fonction déplace circulairement les valeurs vers la droite. Ne pas utiliser de second tableau, de `std::vector` ou d'allocation dynamique.

**Tests :**

- `{1,2,3,4,5}`, décalage `2` → `{4,5,1,2,3}`.
- Même tableau, décalage `0` ou `5` → tableau inchangé.
- Décalage `7` → même effet que `2`.
- Tableau vide ou d'un élément → aucun accès invalide.

**Indice :** traiter le cas vide avant `decalage % taille`. Une solution simple répète des rotations d'un élément. Une solution plus efficace utilise trois inversions de portions du tableau.

**Bonus :** expliquer pourquoi la solution par inversions effectue un nombre d'opérations proportionnel à la taille, même pour un grand décalage.

## Documentation Microsoft

- [Fenêtres Espion](https://learn.microsoft.com/fr-fr/visualstudio/debugger/watch-and-quickwatch-windows?view=vs-2022)
- 
