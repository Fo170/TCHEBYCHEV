# TCHEBYCHEV - Approximation de fonctions par la methode de Tchebychev

## Description

Ce programme calcule le developpement limite d'une fonction selon la methode de
Tchebychev. Il prend en entree une liste de points (x, y) ou une expression
mathematique, calcule les coefficients du polynome de Tchebychev, et affiche
la courbe d'approximation avec analyse de l'erreur.

## Fonctionnalites

- **Saisie des donnees** :
  - Saisie manuelle de points (x, y)
  - Import depuis fichier texte
  - Fonction mathematique (evaluee aux noeuds de Tchebychev)

- **Calcul** :
  - Coefficients Ak du developpement en polynomes de Tchebychev
  - Verification point par point avec l'erreur absolue et relative
  - Courbe d'approximation lisse sur une grille dense

- **Affichage graphique** :
  - Nuage de points (donnees d'entree) en rouge
  - Courbe d'approximation en bleu
  - Zoom avant/arriere
  - Auto-ranging automatique

- **Export** :
  - Sauvegarde des resultats dans un fichier texte

## Algorithme

v = 2 * (x - Xmin) / (Xmax - Xmin) - 1
o = Acos(v),  cos(k * o) = Tk(v)

T0(v) = 1
T1(v) = v
Tk(v) = 2 * v * T[k-1](v) - T[k-2](v)

f(x) = Somme[k=0 a n] { Ak * Tk(v) }

Ak = 2/N * Somme[j=0 a N-1] { yj * Tk(vj) }

ou vj = cos((2*j+1) * PI / (2*N))

## Auteur

Projet original : Olivier Fournet
Port QT : 2026
