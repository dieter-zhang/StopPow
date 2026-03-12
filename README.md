# StopPow

A C++ library for calculating charged-particle stopping power in various materials, from cold matter to hot plasmas.

StopPow implements multiple physics models as an object-oriented class hierarchy. All models share a common interface for computing stopping power (dE/dx), energy downshift/upshift, material thickness, and particle range.

## Physics Models

### Cold Matter

| Class | Description | Reference |
|-------|-------------|-----------|
| `StopPow_SRIM` | Tabulated SRIM stopping power data with interpolation | [SRIM](http://www.srim.org/) |
| `StopPow_BetheBloch` | Bethe-Bloch theory with optional shell corrections | Bethe (1930), Bloch (1933) |
| `StopPow_AZ` | Andersen-Ziegler empirical fits for protons in elements (Z=1-92) | Andersen & Ziegler (1977) |

### Fully Ionized Plasma

| Class | Description | Reference |
|-------|-------------|-----------|
| `StopPow_LP` | Li-Petrasso model with collective effects and quantum corrections | Phys. Rev. Lett. **70**, 3059 (1993) |
| `StopPow_BPS` | Brown-Preston-Singleton model (short-range, long-range, quantum) | Phys. Reports **410**, 237 (2005) |
| `StopPow_Grabowski` | Fit to molecular dynamics stopping power | Phys. Rev. Lett. **111**, 215002 (2013) |

### Partially Ionized Plasma

| Class | Description | Reference |
|-------|-------------|-----------|
| `StopPow_Zimmerman` | Zimmerman model with free/bound electron and ion contributions | UCRL-JC-105616 (1990) |
| `StopPow_Mehlhorn` | Mehlhorn model using Li-Petrasso for free electrons | J. Appl. Phys. (1981) |
| `StopPow_Fit` | Adjustable model for fitting experimental data; selectable free-electron model | -- |

## Dependencies

- **C++17** compiler (clang++, g++)
- **GSL** (GNU Scientific Library) -- `brew install gsl` on macOS
- **GNU Make** or **CMake** (>= 3.14)

Optional (for bindings):
- SWIG 2.x, Python headers (Python bindings)
- SWIG 2.x, Java JDK (Java bindings)

## Building

### With Make

```bash
# Build static library (output in lib/lib/StopPow.a, headers in lib/include/)
cd lib && make

# Build and run tests
cd test && make test

# Build C++ example (copy lib/lib/StopPow.a to examples/c++ first)
cd examples/c++ && make
```

### With CMake

```bash
mkdir build && cd build
cmake ..
make
ctest    # run tests
```

## C++ Library API

All models inherit from `StopPow::StopPow` and share these methods:

| Method | Description |
|--------|-------------|
| `double dEdx(double E)` | Stopping power at energy E |
| `double Eout(double E, double x)` | Exit energy after traversing thickness x |
| `double Ein(double E, double x)` | Incident energy given exit energy and thickness |
| `double Thickness(double E1, double E2)` | Material thickness for energy change E1 -> E2 |
| `double Range(double E)` | Total range (distance to stop) at energy E |
| `double get_Emin()` / `double get_Emax()` | Valid energy bounds |
| `void set_mode(int mode)` | Set calculation mode (`MODE_LENGTH` or `MODE_RHOR`) |

### Usage Examples

**SRIM (tabulated cold-matter data):**
```cpp
#include "StopPow_SRIM.h"

StopPow::StopPow_SRIM s("SRIM/Hydrogen in Aluminum.txt");
double dedx = s.dEdx(14.7);           // MeV/um
double Ef   = s.Eout(14.7, 100.0);    // exit energy after 100 um
double Ei   = s.Ein(14.7, 100.0);     // incident energy
double t    = s.Thickness(14.7, 14.0); // thickness for 14.7 -> 14.0 MeV
double R    = s.Range(14.7);           // total range
```

**Li-Petrasso (plasma):**
```cpp
#include "StopPow_LP.h"

// Test particle: proton (mt=1, Zt=1)
// Field: hydrogen plasma at 1 keV, 1e24 /cm3
std::vector<double> mf  = {1, 1./1823.};  // ion mass, electron mass (AMU)
std::vector<double> Zf  = {1, -1};         // charges
std::vector<double> Tf  = {1, 1};          // temperatures (keV)
std::vector<double> nf  = {1e24, 1e24};    // densities (1/cm3)

StopPow::StopPow_LP s(1, 1, mf, Zf, Tf, nf);
double dedx = s.dEdx(3.0);
```

Or let electrons be added automatically:
```cpp
StopPow::StopPow_LP s(1, 1, mf_ions, Zf_ions, Tf_ions, nf_ions, Te);
```

**Bethe-Bloch:**
```cpp
#include "StopPow_BetheBloch.h"

// Protons in carbon (6.022e22 atoms/cm3)
std::vector<double> mf = {12};
std::vector<double> Zf = {6};
std::vector<double> nf = {6.022e22};

StopPow::StopPow_BetheBloch s(1, 1, mf, Zf, nf);
```

**Andersen-Ziegler (protons in any element):**
```cpp
#include "StopPow_AZ.h"

StopPow::StopPow_AZ s(13);        // protons in aluminum (Z=13)
StopPow::StopPow_AZ s(13, 2.7);   // with density in g/cc (enables MODE_RHOR)
```

### Calculation Modes

- `MODE_LENGTH` (default): lengths in um, stopping power in MeV/um
- `MODE_RHOR`: lengths in mg/cm^2, stopping power in MeV/(mg/cm^2)

```cpp
s.set_mode(StopPow::StopPow::MODE_RHOR);
```

## Input/Output Formats

- **Energy**: MeV
- **Length**: um (MODE_LENGTH) or mg/cm^2 (MODE_RHOR)
- **Temperature**: keV (plasma models)
- **Density**: 1/cm^3 (plasma models)
- **Mass**: AMU

**SRIM data files**: Standard SRIM text output format. Place files in a `SRIM/` directory relative to the working directory. Example files are included in the `test/SRIM/` directory.

## Python Bindings

```bash
cd python_swig
swig -python -c++ StopPow.i
python setup.py build_ext --inplace
```

## Java Bindings

```bash
cd java_swig
make
```

## Directory Structure

```
StopPow/
  src/            C++ source files and headers
  lib/            Makefile to build static library
  test/           Test suite (9 test programs)
  examples/c++/   C++ usage example
  python_swig/    Python SWIG bindings
  java_swig/      Java SWIG/JNI bindings
  StopPowGUI/     Java GUI application (NetBeans project)
  doc/            Doxygen-generated documentation
```

## License

GNU General Public License v2. See [LICENSE](LICENSE).

## Authors

Alex Zylstra (MIT / PSFC)
