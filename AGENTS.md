# AGENTS.md — TCHEBYCHEV Project

## Project Overview

Approximation de fonctions par la methode de Tchebychev (developpement limite).
Programme QT avec interface graphique pour saisir des points, calculer les coefficients
de Tchebychev, afficher la courbe d'approximation et exporter les resultats.

## Project Structure

```
TCHEBYCHEV/
  Types.h           - Types de base (Ldbl, uint, PI, macros)
  TCHBYCHV.h        - Classe d'approximation de Tchebychev
  main.cpp          - Point d'entree QT
  mainwindow.h      - Interface graphique (header)
  mainwindow.cpp    - Interface graphique (implementation)

  img/
    TCHEBYCHEV.png  - Capture d'ecran de l'application

  Windows/
    TCHEBYCHEV.pro  - Configuration Windows (MSVC/MinGW)
    TCHEBYCHEV.exe  - Binaire compile (release)

  linux/
    TCHEBYCHEV.pro  - Configuration Linux (GCC)
```

## Build Instructions

**Important :** Compilez toujours depuis le sous-dossier correspondant a votre plateforme.
Les artefacts de compilation restent dans ce sous-dossier, la racine reste propre.

### Windows (MinGW)
```bash
cd Windows
qmake TCHEBYCHEV.pro
mingw32-make
windeployqt release\TCHEBYCHEV.exe
```

### Windows (MSVC)
```bash
cd Windows
qmake TCHEBYCHEV.pro
nmake
windeployqt release\TCHEBYCHEV.exe
```

### Linux
```bash
cd linux
qmake TCHEBYCHEV.pro
make
```

## Dependencies

- Qt 6.x (Core, Gui, Widgets, Charts)
- Compilateur C++17
- Bibliotheque math (libm sur Linux, integree sur MSVC)

## Key Types and Conventions

- `Ldbl` = `long double` (precision etendue 80-bit)
- `PI` = 3.14159265358979323846L
- `PI_2` = PI / 2
- Macros `pw2(x)` = `(x)*(x)`, `pw3(x)` = `(x)*(x)*(x)` pour carre/cube
- Macro `_(x)` pour dereferencement de pointeur `(*(x))`
- Commentaires et documentation en francais
- Pas d'espaces dans les noms de fichiers ou dossiers

## Data Flow

### Entree utilisateur
1. **Mode points manuels** : l'utilisateur remplit un QTableWidget avec des valeurs (x, y)
2. **Mode fichier** : QFileDialog -> QTextStream -> parse avec QRegularExpression -> remplit le tableau
3. **Mode fonction** : l'utilisateur entre une expression (ex: "4*sin(x)/(exp(x)+exp(-x))"), evaluee via functionFT()

### Calcul (TCHBYCHV + MainWindow::computeChebyshev)
1. L'utilisateur fixe N (nb points/nœuds), Xmin, Xmax
2. Les nœuds de Tchebychev sont calcules : vj = cos((2j+1) * PI/2N), xj = Xmin + (1+vj)*(Xmax-Xmin)/2
3. Les yj sont obtenus : evaluation de fonction aux nœuds (mode fonction) ou interpolation nearest-neighbor depuis les points saisis
4. Les coefficients Ak sont calcules : Ak = 2/N * Sum[j]{ yj * Tk(vj) }
   - Recurrence de Tk : T0=1, T1=v, Tk=2*v*T[k-1]-T[k-2]
5. Verification : pour chaque nœud, on reconstitue y_calc = Sum[k]{ Ak * Tk(vj) } et on calcule l'erreur
6. Courbe dense : on evalue le polynome sur 200+ points regulierement espaces pour un tracé lisse

### Affichage
- **Panneau gauche** : formulaire de saisie + tableau des points + resultats textuels
- **Panneau droit** : QChart avec QScatterSeries (rouge) pour les points d'entree, QLineSeries (bleu) pour la courbe d'approximation
- Auto-range : calcul des bornes x/y sur l'ensemble des points + marge 10%

### Export
- Les resultats (coefficients, table de verification, erreur max) sont exportes via QFileDialog -> QFile -> QTextStream

## Application Controls (Widget Hierarchy)

```
QMainWindow (windowTitle: "TCHEBYCHEV - Approximation de fonctions", 1200x800)
  QWidget (central)
    QHBoxLayout
      [Left panel - QVBoxLayout, stretch 2]
        QGroupBox "Saisie"
          QComboBox (3 modes: points manuels / fichier / fonction)
          QFormLayout
            QLineEdit "Nb points" (defaut: 18)
            QLineEdit "X min" (defaut: -3.14159)
            QLineEdit "X max" (defaut: 3.14159)
            QLineEdit "Fonction f(x)" (defaut: "4*sin(x)/(exp(x)+exp(-x))", desactive en mode points)
          QPushButton "Charger fichier..." (cache ou desactive en mode manuel/fonction)
          QTableWidget (colonnes X, Y)
          [Boutons] QPushButton "Calculer" | "Effacer" | "Exporter"
        QGroupBox "Resultats"
          QLabel "Erreur Absolue Max: --"
          QTextEdit (readOnly, police Courier New 9, monospace)
      [Right panel - QVBoxLayout, stretch 3]
        [Boutons graphique] QPushButton "Zoom +" | "Zoom -" | "Auto-range"
        QChartView
          QChart "Approximation de Tchebychev"
            QScatterSeries "Points saisis" (rouge, markerSize 8)
            QLineSeries "Approximation" (bleu)
            QValueAxis (X)
            QValueAxis (Y)
```

## Algorithm Details

### Coefficients Ak

Ak = (2/N) * Sum_{j=0}^{N-1} [ yj * Tk(vj) ]

Implementation dans `MainWindow::computeChebyshev()` (lignes 228-269 de `mainwindow.cpp`) :
```
Pour chaque nœud j:
  vj = cos((2j+1) * PI/2N)
  yj = f(xj) ou nearest-neighbor
  A0 += yj / 2
  A1 += yj * vj
  Recurrence Tk pour k>=2: Ak += yj * Tk

Apres la boucle: Ak *= 2/N pour tout k
```

### Verification

Pour chaque nœud j, on recalcule y_calc = Sum_{k} Ak * Tk(vj) et on affiche :
- X, Y_original, Y_tchebychev
- Erreur absolue = y_calc - y_orig
- Erreur relative = (y_calc - y_orig) / y_orig * 100%

L'erreur absolue maximale est affichee dans le label en haut du panneau Resultats.

### Courbe dense

200 points (ou N*4 si superieur) regulierement espaces sur [Xmin, Xmax] sont evalues avec les memes coefficients Ak pour produire une courbe de trace lisse.

## Limitations

- `functionFT()` est codée en dur dans `mainwindow.cpp` — l'expression entree par l'utilisateur dans le champ "Fonction f(x)" n'est pas parse; seuls les parametres (N, Xmin, Xmax) sont utilises. L'expression affichee sert d'indication.
- Interpolation nearest-neighbor en mode points (pas d'interpolation lineaire ou spline)
- N (nb nœuds) limite a 255
- La verification et la courbe dense utilisent le meme intervalle [Xmin, Xmax]

## Future Improvements

- Parser d'expression mathematique pour evaluer la fonction utilisateur en mode 3
- Interpolation lineaire entre les points saisis au lieu de nearest-neighbor
- Export PDF du graphique
- Drag & drop des points sur le graphique
- Theme sombre / clair
