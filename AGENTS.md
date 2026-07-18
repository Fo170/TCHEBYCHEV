# AGENTS.md - TCHEBYCHEV Project

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

  Windows/
    TCHEBYCHEV.pro  - Configuration Windows (MSVC/MinGW)
    TCHEBYCHEV.exe  - Binaire compilé (release)

  linux/
    TCHEBYCHEV.pro  - Configuration Linux (GCC)
```

## Build Instructions

**Important :** Compilez toujours depuis le sous-dossier correspondant à votre plateforme.
Les artéfacts de compilation restent dans ce sous-dossier, la racine reste propre.

### Linux
```bash
cd linux
qmake TCHEBYCHEV.pro
make
```

### Windows (MinGW)
```bash
cd Windows
qmake TCHEBYCHEV.pro
mingw32-make
```

### Windows (MSVC)
```bash
cd Windows
qmake TCHEBYCHEV.pro
nmake
```

## Dependencies

- Qt 5.15+ (Core, Gui, Widgets, Charts)
- C++17 compiler
- Math library (libm on Linux, built-in on MSVC)

## Key Types and Conventions

- `Ldbl` = `long double` (precision etendue)
- `PI` = 3.14159265358979323846L
- `PI_2` = PI/2
- Macros `pw2(x)`, `pw3(x)` pour carre/cube
- Macro `_(x)` pour dereferencement de pointeur

## Data Flow

1. User input: manual points, file import, or mathematical function
2. `TCHBYCHV` class computes coefficients Ak using Chebyshev polynomials Tk(v)
3. Output: coefficients, verification table, error analysis
4. Chart: scatter input points + line series of fitted curve
5. Export: results can be saved to text file
