# Jour 3 — Pile, tas et allocation dynamique : cours détaillé

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
} // nombre n'existe plus.
// p est pendant : ne pas accéder à *p.
```

De même, renvoyer l'adresse d'une variable locale d'une fonction produit un pointeur qui ne permet plus d'accéder à un objet vivant après le retour.

### 1.1 Trois notions à ne pas confondre

| Notion | Question | Exemple |
|---|---|---|
| Nom et portée | Où puis-je écrire ce nom ? | `temporaire` est visible dans son bloc |
| Objet et durée de vie | Cet objet existe-t-il encore ? | L'entier local disparaît à la sortie du bloc |
| Pointeur et validité | Puis-je accéder à sa cible maintenant ? | Un pointeur peut rester dans la portée alors que sa cible est détruite |

Dans le premier exemple, `nombre` existe pendant l'exécution de la fonction et `temporaire` seulement pendant le bloc intérieur. Lorsqu'un bloc se termine, les objets locaux ordinaires sont détruits dans l'ordre inverse de leur construction. Un entier n'a pas de travail de nettoyage visible ; un `std::string`, par exemple, peut devoir libérer une ressource.

### 1.2 Un programme complet pour observer les blocs

```cpp
#include <iostream>

int main() {
    int exterieur = 10;

    {
        int interieur = 20;
        int* observateur = &exterieur;

        *observateur += interieur;
        std::cout << "Dans le bloc : " << exterieur << '\n';
    }

    std::cout << "Apres le bloc : " << exterieur << '\n';
    return 0;
}
```

Résultat : `30` dans les deux affichages. L'objet `exterieur` vit plus longtemps que `observateur`. La destruction du pointeur local ne détruit pas l'entier extérieur. Les noms `interieur` et `observateur` ne sont plus disponibles après le bloc.

Dans Visual Studio, poser un point d'arrêt sur `*observateur += interieur`, avancer avec `F10`, puis regarder **Variables locales** avant et après la sortie du bloc.

### 1.3 Pourquoi une adresse locale ne peut pas être conservée

Si une fonction crée un entier local puis renvoie son adresse, l'appelant reçoit une adresse vers un objet dont la vie vient de se terminer. L'adresse peut sembler inchangée et les octets peuvent encore contenir l'ancienne valeur : aucun de ces faits n'autorise la lecture.

Pour renvoyer un simple résultat numérique, utiliser une valeur :

```cpp
int creerNombre() {
    int nombre = 42;
    return nombre;
}
```

L'appelant reçoit le résultat `42`, pas un accès à l'objet local détruit. L'allocation dynamique n'est pas nécessaire pour renvoyer un entier.

**Question flash :** un pointeur prolonge-t-il automatiquement la durée de vie d'une variable locale ? **Non.** Il désigne l'objet ; il ne le conserve pas vivant.

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

### 2.1 La pile pendant les appels de fonctions

Lorsqu'une fonction en appelle une autre, l'implémentation conserve les informations nécessaires pour revenir à l'appelante. On parle souvent de cadre d'appel : il comprend les informations de retour et généralement une partie des données locales.

```cpp
#include <iostream>

int doublerValeur(int valeur) {
    int resultat = valeur * 2;
    return resultat;
}

int main() {
    int nombre = 21;
    int resultat = doublerValeur(nombre);
    std::cout << resultat << '\n';
    return 0;
}
```

Pendant `doublerValeur`, les objets de `main` existent encore. Le paramètre `valeur` est une copie de `nombre`. Le `resultat` local de la fonction et celui de `main` sont deux objets différents, même s'ils portent le même nom.

Poser un point d'arrêt sur `return resultat` et ouvrir **Pile des appels**. On doit retrouver l'appel de `doublerValeur` depuis `main`. Les noms exacts des cadres supplémentaires dépendent du runtime et des symboles de débogage.

Les gros tableaux locaux et la récursion profonde peuvent épuiser la pile du thread. Ne pas expérimenter avec des tailles gigantesques : la taille de pile est limitée et dépend de la configuration du programme.

### 2.2 Le tas et la durée de vie indépendante du bloc

Une allocation dynamique est utile lorsque la taille n'est connue qu'à l'exécution ou lorsqu'un objet doit survivre au bloc qui le crée. Elle a aussi un coût : recherche de stockage, gestion de l'allocation, puis libération. L'absence de variable locale massive ne signifie pas une mémoire illimitée : le tas dépend des ressources et des limites du processus.

```cpp
int* creerEntierDynamique() {
    int* local = new int{42};
    return local;
}
```

À la sortie de cette fonction, le pointeur local est détruit, mais l'entier dynamique reste vivant. Une copie de son adresse a été renvoyée. Le contrat doit préciser que l'appelant devient propriétaire et doit libérer l'entier. En C++ moderne, on exprime plutôt ce transfert avec un propriétaire automatique tel que `std::unique_ptr`.

### 2.3 Où se trouvent le pointeur et la cible ?

Pour `int* p = new int{42};`, il y a **deux objets** :

| Objet | Contenu | Durée de stockage | Fin de vie |
|---|---|---|---|
| `p` | Une adresse | Automatique dans cet exemple local | Sortie du bloc |
| L'entier alloué | `42` | Dynamique | `delete p`, tant que `p` contient sa bonne adresse |

`&p` est l'adresse du pointeur lui-même. `p` contient l'adresse de l'entier alloué. `*p` permet d'accéder à cet entier. Le fait qu'un pointeur soit local ne dit donc pas où se trouve sa cible.

**À retenir :** utiliser la pile pour des objets locaux ordinaires et des propriétaires automatiques ; utiliser les allocations dynamiques lorsque nécessaire, en organisant leur propriété.

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

### 3.1 Décomposer `new int{42}`

L'expression réalise, dans le cas ordinaire, les étapes suivantes :

1. Obtenir assez de stockage pour un entier, correctement aligné.
2. Créer et initialiser l'entier avec `42` dans ce stockage.
3. Produire un pointeur de type `int*` vers cet objet.
4. Affecter cette adresse à la variable pointeur.

L'objet dynamique n'a pas besoin d'un nom de variable propre : on y accède ici à travers le pointeur. `new` est un opérateur C++, pas simplement une déclaration de variable.

| Expression | État initial de l'entier |
|---|---|
| `new int{42}` | Valeur `42` |
| `new int{}` | Valeur `0` |
| `new int` | Valeur indéterminée : affecter avant lecture |

### 3.2 Décomposer `delete p`

`delete p` détruit l'objet alloué et rend son stockage disponible pour une utilisation ultérieure. Il ne modifie pas automatiquement la valeur de `p`. Le pointeur contient donc encore une adresse qui ne permet plus d'accéder à l'ancien objet.

L'instruction suivante, `p = nullptr`, modifie le pointeur. Elle ne libère rien par elle-même. L'ordre est essentiel : mettre l'unique pointeur propriétaire à `nullptr` **avant** la libération perdrait l'adresse nécessaire.

`delete nullptr` et `delete[] nullptr` sont autorisés et n'ont pas d'effet. Cela ne rend pas autorisée une seconde libération via un pointeur encore non nul et pendant.

### 3.3 Exemple complet et trace

```cpp
#include <iostream>

int main() {
    int* proprietaire = new int{42};
    int* observateur = proprietaire;

    *observateur += 8;
    std::cout << "Valeur : " << *proprietaire << '\n';

    delete proprietaire;
    proprietaire = nullptr;
    observateur = nullptr;
    return 0;
}
```

Résultat : `Valeur : 50`.

| Étape | Objet dynamique | Propriétaire | Observateur |
|---|---|---|---|
| Après `new` | Vivant, valeur 42 | Le désigne | Pas encore créé |
| Après copie du pointeur | Toujours le même objet | Le désigne | Le désigne aussi |
| Après `*observateur += 8` | Vivant, valeur 50 | Le désigne | Le désigne |
| Après `delete` | Détruit | Pendant | Pendant |
| Après les deux affectations | Détruit | Nul | Nul |

Copier un pointeur ne copie pas son objet. Deux pointeurs peuvent désigner le même entier, mais un seul doit porter la responsabilité de la libération dans ce programme.

### 3.4 Échec d'allocation et exceptions

Le `new` ordinaire peut lever `std::bad_alloc` si le stockage ne peut pas être obtenu. Une exception interrompt le chemin normal ; elle peut être interceptée avec `try` et `catch`. Il ne faut donc pas supposer que le code suivant `new` sera toujours atteint.

Dans cette séance, les tailles sont petites et validées. La gestion complète des exceptions n'est pas un objectif obligatoire, mais elle explique pourquoi la gestion automatique est préférable : un propriétaire RAII se détruit aussi lorsqu'une exception fait quitter sa portée.

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

### 4.1 Une fuite ne fait pas forcément planter

Si une fonction alloue à chaque appel et ne libère jamais, les objets oubliés s'accumulent. Le programme peut donner les bons résultats tout en consommant de plus en plus de mémoire. La fin du processus permet normalement au système de récupérer son espace mémoire, mais cela ne corrige pas les fuites pendant son fonctionnement.

Un retour anticipé peut créer une fuite :

```cpp
// Exemple a analyser : ne pas conserver cette version.
void traitement(bool abandonner) {
    int* p = new int{42};
    if (abandonner) {
        return; // delete n'est pas execute.
    }
    delete p;
}
```

Solution manuelle : éviter un retour qui oublie le nettoyage, ou libérer avant chaque sortie. Solution plus robuste : confier l'allocation à un propriétaire automatique.

### 4.2 Utilisation après libération

Après destruction, il est incorrect de lire **ou** écrire l'ancien objet. Un allocateur peut réutiliser sa zone pour un autre objet ; une ancienne écriture pourrait alors modifier des données sans rapport.

La règle s'applique à tous les alias. Mettre un seul pointeur à zéro ne sécurise pas les autres. Lorsqu'un groupe d'objets est détruit ou remplacé, identifier les pointeurs qui le désignaient et cesser de les utiliser.

### 4.3 Double libération et libération d'un objet local

Deux pointeurs égaux ne représentent pas deux allocations. Libérer par chacun produirait une double libération. De même, l'adresse d'une variable locale n'est pas issue de `new` : la transmettre à `delete` est incorrect.

Avant un `delete`, se demander : **cette adresse provient-elle de l'allocation correspondante, est-ce le début de cette allocation, et suis-je son propriétaire ?** Un pointeur vers un élément intérieur d'un tableau ne doit pas servir à libérer ce tableau.

### 4.4 Hors limites et comportement indéfini

Pour trois éléments, seuls les indices 0, 1 et 2 sont valides. Une lecture à l'indice 3 n'est pas « une quatrième valeur vide ». Elle ne désigne aucun élément de ce tableau.

Un comportement indéfini signifie que le langage ne garantit plus le résultat de l'exécution concernée. Il ne faut pas conclure « c'est bon, ça ne plante pas ». Corriger l'erreur même si elle semble sans effet.

### 4.5 Méthode de relecture d'un programme

1. Repérer toutes les allocations.
2. Pour chacune, nommer son propriétaire.
3. Repérer la libération correspondante.
4. Vérifier les retours anticipés et les branches.
5. Vérifier qu'aucun observateur n'est utilisé après destruction.
6. Vérifier chaque indice et chaque taille.

Cette relecture complète les outils : elle explique le problème et sa correction.

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

### 5.1 Pourquoi `int valeurs[taille]` ne convient pas ici

Si `taille` est saisie pendant l'exécution, elle n'est pas une expression constante à la compilation. Un tableau intégré déclaré avec cette taille n'est pas du C++ standard ; MSVC ne fournit pas cette construction pour le cours. Utiliser une allocation dynamique ou `std::vector`.

Avec `new int[taille]{}`, les éléments sont contigus et initialisés à zéro. Si le tableau contient quatre entiers, il comporte exactement les éléments d'indices 0 à 3. `valeurs + taille` forme l'adresse juste après la fin : elle peut servir de borne de parcours, pas être déréférencée.

### 5.2 Programme complet : taille saisie et nettoyage

```cpp
#include <iostream>

int main() {
    int taille = 0;
    std::cout << "Taille entre 1 et 100 : ";
    if (!(std::cin >> taille) || taille < 1 || taille > 100) {
        std::cout << "Taille invalide\n";
        return 1;
    }

    int* valeurs = new int[taille]{};

    for (int i = 0; i < taille; ++i) {
        valeurs[i] = (i + 1) * 10;
    }
    for (int i = 0; i < taille; ++i) {
        std::cout << valeurs[i] << ' ';
    }
    std::cout << '\n';

    delete[] valeurs;
    valeurs = nullptr;
    return 0;
}
```

Pour une taille de 4 : `10 20 30 40`. La validation se fait avant l'allocation. Aucune sortie intermédiaire ne perd le tableau dans cet exemple.

### 5.3 Pourquoi `sizeof` ne retrouve pas la taille

```cpp
int fixe[4] = {};
int* dynamique = new int[4]{};
// sizeof(fixe) mesure le tableau entier.
// sizeof(dynamique) mesure uniquement la variable pointeur.
delete[] dynamique;
```

Un pointeur brut n'est pas accompagné, dans son interface C++, d'un nombre d'éléments consultable. Les détails internes d'un allocateur ne constituent pas un moyen portable de retrouver la taille. Conserver un couple cohérent **pointeur + taille**.

### 5.4 Pourquoi `delete[]` est nécessaire

Le langage exige que la forme de libération corresponde à la forme d'allocation. C'est vrai aussi pour les tableaux d'entiers, même si leur destruction est simple. Pour des tableaux de structures avec des membres gérant des ressources, chaque élément doit également être correctement détruit.

`delete[]` ne vide pas un tableau pour pouvoir le réutiliser : il détruit le tableau. Pour conserver le tableau et remettre ses éléments à zéro, parcourir les éléments et leur affecter zéro.

### 5.5 Agrandir sans perdre les données

Pour passer de `{10, 20, 30}` à quatre emplacements :

| Étape | Ancien tableau | Nouveau tableau |
|---|---|---|
| Allocation | `{10,20,30}` vivant | `{0,0,0,0}` vivant |
| Copie | Toujours vivant | `{10,20,30,0}` |
| Libération de l'ancien | Détruit | Toujours vivant |
| Mise à jour du propriétaire | Ancienne adresse abandonnée | Adresse conservée |

La copie doit avoir lieu **avant** la destruction de l'ancien tableau. Ne pas écrire `valeurs = new int[...]` directement sur le seul pointeur propriétaire sans avoir sauvegardé et traité l'ancienne allocation.

Le tableau agrandi n'est pas forcément situé à la même adresse. Tous les pointeurs vers l'ancien tableau doivent être considérés comme invalides après sa destruction. Pour retrouver un élément, utiliser son indice dans le nouveau tableau si cet indice reste pertinent.

## 6. Gestion automatique : RAII

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

### 6.1 Pourquoi un objet propriétaire change le problème

Un pointeur brut ne libère pas sa cible à sa destruction. Un objet propriétaire comme `std::unique_ptr` a, lui, un destructeur qui effectue le nettoyage. Une sortie de bloc normale, un `return` ou une exception qui déroule la pile déclenche la destruction des propriétaires locaux construits.

Ce mécanisme ne repose pas sur l'appel manuel d'un « ramasse-miettes ». La fin de vie des propriétaires locaux ordinaires dépend de la sortie de leur portée.

### 6.2 `std::vector` pas à pas

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> valeurs(3, 0);
    valeurs[0] = 10;
    valeurs[1] = 20;
    valeurs[2] = 30;
    valeurs.push_back(40);

    std::cout << "Taille : " << valeurs.size() << '\n';
    for (int valeur : valeurs) {
        std::cout << valeur << ' ';
    }
    std::cout << '\n';
    return 0;
}
```

Résultat : taille `4`, puis `10 20 30 40`. À la fin de `main`, le vecteur détruit ses éléments et libère le stockage qu'il possède.

- `size()` : nombre d'éléments réellement présents.
- `capacity()` : nombre d'éléments pouvant tenir dans le stockage actuel sans réallocation.
- `push_back()` : ajoute un élément, en augmentant la taille.
- `reserve()` : demande une capacité suffisante, sans ajouter d'éléments.
- `resize()` : change le nombre d'éléments.

Ne pas écrire `valeurs[3] = 40` sur un vecteur de taille 3, même s'il a une capacité supérieure. Seuls les indices strictement inférieurs à `size()` sont utilisables avec `[]`.

La stratégie précise d'augmentation de capacité dépend de l'implémentation. Ne pas supposer que le vecteur double toujours sa capacité. Pour transmettre un vecteur en lecture, utiliser `const std::vector<int>&` ; pour le modifier, `std::vector<int>&`.

### 6.3 `std::unique_ptr` pas à pas

```cpp
#include <iostream>
#include <memory>

void exemple(bool abandonner) {
    auto nombre = std::make_unique<int>(42);
    if (abandonner) {
        return; // Liberation automatique.
    }
    std::cout << *nombre << '\n';
} // Liberation automatique si la fonction arrive ici.

int main() {
    exemple(false);
    exemple(true);
    return 0;
}
```

Résultat : un seul affichage de `42`, et les deux allocations sont prises en charge par leurs propriétaires. `auto` laisse le compilateur déduire ici le type `std::unique_ptr<int>`.

`nombre.get()` fournit un pointeur brut observateur. Il ne transfère pas la propriété et ne prolonge pas la vie de l'entier. Ne pas appeler `delete` sur ce pointeur. Copier le propriétaire est interdit ; un transfert explicite peut utiliser `std::move`, notion facultative aujourd'hui.

### 6.4 Quel outil choisir ?

| Besoin | Choix habituel |
|---|---|
| Une simple valeur locale | Variable ordinaire |
| Tableau local intégré de taille constante pour l'apprentissage | `int valeurs[5]` |
| Tableau dont la taille est connue à l'exécution ou évolue | `std::vector` |
| Propriété exclusive d'un objet dynamique | `std::unique_ptr` |
| Observer un objet existant sans le posséder | Référence ou pointeur, selon le contrat |

Les allocations manuelles de ce cours servent à comprendre les mécanismes et les erreurs. Dans les programmes courants, exprimer la propriété avec les outils automatiques réduit les chemins de nettoyage à vérifier.

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

Identifier trois erreurs et proposer une correction : borne valide, `delete[]`, absence d'accès après destruction. Générer la correction et la tester avec AddressSanitizer. Utiliser ensuite une exécution Debug séparée avec le rapport CRT pour contrôler les fuites.

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

### Aide au challenge : taille et capacité

Au départ, deux emplacements sont alloués, mais aucun ne contient encore un élément de la collection : `taille = 0`, `capacite = 2`.

| Après l'ajout de… | Taille | Capacité attendue dans ce challenge |
|---|---:|---:|
| 1 | 1 | 2 |
| 2 | 2 | 2 |
| 3 | 3 | 4 |
| 5 | 5 | 8 |
| 9 | 9 | 16 |
| 10 | 10 | 16 |

Conserver l'invariant `0 <= taille <= capacite`. Avant d'écrire à l'indice `taille`, s'assurer que `taille < capacite`. Écrire la nouvelle valeur puis incrémenter la taille.

Quand le tableau est plein : préparer la nouvelle allocation, copier les éléments utilisés, libérer l'ancienne allocation, remplacer l'adresse et mettre à jour la capacité. Avec des entiers, la copie elle-même ne lève pas d'exception ; si l'allocation échoue, ne pas avoir déjà détruit l'ancienne collection.

Ne pas afficher les emplacements entre `taille` et `capacite - 1` comme des éléments de la collection. Pour supprimer le dernier élément de cette collection d'entiers, décrémenter la taille suffit si elle est positive ; l'ancien emplacement n'est alors plus un élément utilisé.

## Documentation Microsoft

- [AddressSanitizer avec MSVC](https://learn.microsoft.com/fr-fr/cpp/sanitizers/asan?view=msvc-170)
- [Options incompatibles avec AddressSanitizer](https://learn.microsoft.com/en-us/cpp/sanitizers/asan-known-issues?view=msvc-170)
- [Détection des fuites avec la CRT](https://learn.microsoft.com/fr-fr/cpp/c-runtime-library/find-memory-leaks-using-the-crt-library?view=msvc-170)
