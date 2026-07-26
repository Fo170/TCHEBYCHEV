# TCHEBYCHEV — Agent instructions

QT6/C++17 qmake app for Chebyshev polynomial approximation.

## Build

Compile from the platform subdirectory only (keeps root clean):
```bash
cd linux   # or Windows/
qmake TCHEBYCHEV.pro && make
# Windows: qmake TCHEBYCHEV.pro && (mingw32-make|nmake) && windeployqt release\TCHEBYCHEV.exe
```

Platform `.pro` files use `../` relative paths for sources; both add `QT += core gui widgets charts network`.

## Key files

| File | Role |
|---|---|
| `mainwindow.cpp` | All UI logic, Chebyshev computation (`computeChebyshev()`), chart, export |
| `TCHBYCHV.h` | Pure data class: nodes X[j], Y[j], coefficients aT[k] |
| `LangueManager` | Key=value lang files in `lang/*.txt`, emits `languageChanged()` -> `retranslateUi()` |
| `Project` | JSON save/load of input state (points, params, coefficients) |
| `UpdateChecker` | Fetches `version.json` via HTTP, compares with `QVersionNumber` |
| `AppConfig.hpp` | Defines `APP_NAME`, `APP_VERSION` ("2.0"), `UPDATE_CHECK_URL` |
| `Types.h` | `Ldbl` = `long double`, `PI`, `pw2`/`pw3` macros, `_(x)` = `*(x)` |

## Conventions

- French comments and UI strings (English alternative in `lang/anglais.txt`)
- `functionFT()` is hardcoded at line 601 — the "Fonction f(x)" field is display-only, NOT parsed
- Nearest-neighbor interpolation for user points (no linear/spline)
- N (node count) limited to 2–255
- `_()` macro dereferences a pointer: `_(p)` == `*(p)`
- `Ldbl` = `long double`; flags `-mfpmath=387 -ffloat-store` on GCC/MinGW for 80-bit precision
- MSVC `.pro` adds `/fp:precise /utf-8` (emoji support)

## Runtime

- Settings saved to `tchebychev.ini` beside the executable (language, window geometry)
- `LangueManager` loads `lang/<code>.txt` files from `applicationDirPath() + "/lang/"`
- Icons baked into binary via `resources.qrc` (`:/ico/app-*.png`)
- Project files are `.json` (menu Fichier → Ouvrir/Sauvegarder)
- `AGENTS.md` is gitignored

## Gotchas

- The `.pro` files are the ONLY build config (no CMake)
- Function mode evaluates `functionFT()` which is `4*sin(x)/(exp(x)+exp(-x))` — the user's expression text is never parsed
- `autoRange()` considers both `m_inputPoints` and `m_fittedPoints` for bounds with 10% margin
- `onExportText()` writes `m_output->toPlainText()` to a file — no formatting options
- Zoom is multiplicative: `zoom(1.5)` / `zoom(0.666)`; `onResetView` calls `autoRange()`
