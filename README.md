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

## Prerequisites

Install the required dependencies before building:

```bash
# macOS (Homebrew)
brew install gsl cmake

# Ubuntu / Debian
sudo apt-get install libgsl-dev cmake build-essential

# Fedora / RHEL
sudo dnf install gsl-devel cmake gcc-c++
```

Requirements:
- **C++17** compiler (clang++ or g++)
- **GSL** (GNU Scientific Library)
- **CMake** (>= 3.14) or **GNU Make**

For Python bindings, also install:
- **SWIG** (`brew install swig` on macOS, `apt-get install swig` on Ubuntu)
- **Python 3** with development headers

## Building

### Option 1: CMake (recommended)

```bash
git clone https://github.com/dieter-zhang/StopPow.git
cd StopPow
mkdir build && cd build
cmake ..
make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu)
```

This builds:
- `libStopPow.a` -- the static library
- `StopPow_example` -- the example executable
- `test0` through `test8` -- test executables

### Option 2: Make

```bash
cd lib && make        # builds lib/lib/StopPow.a and copies headers to lib/include/
```

## Running the Example

The included example (`examples/c++/main.cpp`) demonstrates three models: SRIM (proton in solid aluminum), Li-Petrasso (proton in hydrogen plasma), and Bethe-Bloch (proton in diamond).

### After building with CMake

```bash
# Must run from examples/c++ so it can find the SRIM data files
cd examples/c++
../../build/StopPow_example
```

### After building with Make

```bash
# First copy the library to the example directory
cp lib/lib/StopPow.a examples/c++/
cd examples/c++
make
./StopPow
```

### Expected output

```
dEdx(10 MeV) = -0.00918155 MeV/um          # SRIM: proton in solid aluminum
dEdx(10 MeV) = -0.0339818 MeV/(mg/cm2)
Eout(10 MeV, 100um) = 9.04531
Ein(10 MeV, 100um) = 10.8888
Thickness(10 MeV, 9 MeV) = 104.561
Thickness(10 MeV, 1 MeV) = 610.41
Range(10 MeV) = 624.797
-----------
dEdx(10 MeV) = -0.0144201 MeV/um          # Li-Petrasso: proton in H plasma
dEdx(10 MeV) = -0.0861648 MeV/(mg/cm2)
Eout(10 MeV, 100um) = 8.46346
Ein(10 MeV, 100um) = 11.3668
Thickness(10 MeV, 9 MeV) = 66.5674
Thickness(10 MeV, 1 MeV) = 423.19
Range(10 MeV) = 471.962
-----------
dEdx(10 MeV) = -0.0141242 MeV/um          # Bethe-Bloch: proton in diamond
dEdx(10 MeV) = -0.0399827 MeV/(mg/cm2)
Eout(10 MeV, 100um) = 8.49478
Ein(10 MeV, 100um) = 11.3399
Thickness(10 MeV, 9 MeV) = 67.9095
Thickness(10 MeV, 1 MeV) = 388.745
Range(10 MeV) = 393.028
-----------
```

## Running the Tests

### With CMake

```bash
cd build
ctest                    # run all 9 tests
ctest -R test0           # run a single test
ctest --output-on-failure  # show output for failing tests
```

### With Make

```bash
cd test
make test               # build and run all tests
make test0.out && ./test0.out           # build and run a single test
make test0.out && ./test0.out --verbose # run with verbose output
```

### Test descriptions

| Test | What it tests |
|------|---------------|
| test0 | Computational fidelity: model outputs vs. published/reference values |
| test1 | Computational correctness: dEdx, Eout, Ein, Thickness, Range under normal and abnormal conditions |
| test2 | Plot generation utilities |
| test3 | Atomic data retrieval |
| test4 | dEdx computation speed benchmarks |
| test5 | Numerical accuracy of SRIM-based Eout, Ein, Thickness, Range |
| test6 | Fitting utilities (Gaussian, rhoR, forward fit) |
| test7 | StopPow_Fit model |
| test8 | SRIM file loading for various materials |

## Writing Your Own C++ Program

Create a file `my_calc.cpp`:

```cpp
#include <iostream>
#include "StopPow_SRIM.h"
#include "StopPow_LP.h"
#include "StopPow_AZ.h"

int main() {
    // --- SRIM: load tabulated data ---
    StopPow::StopPow_SRIM srim("path/to/SRIM/Hydrogen in Aluminum.txt");
    std::cout << "SRIM dEdx at 14.7 MeV: " << srim.dEdx(14.7) << " MeV/um" << std::endl;
    std::cout << "Exit energy after 100 um: " << srim.Eout(14.7, 100.0) << " MeV" << std::endl;
    std::cout << "Incident energy for 100 um: " << srim.Ein(14.7, 100.0) << " MeV" << std::endl;
    std::cout << "Thickness 14.7 -> 14.0 MeV: " << srim.Thickness(14.7, 14.0) << " um" << std::endl;
    std::cout << "Range at 14.7 MeV: " << srim.Range(14.7) << " um" << std::endl;

    // Switch to areal density mode
    srim.set_mode(StopPow::StopPow::MODE_RHOR);
    std::cout << "SRIM dEdx (rhoR mode): " << srim.dEdx(14.7) << " MeV/(mg/cm2)" << std::endl;

    // --- Andersen-Ziegler: proton in any element by atomic number ---
    StopPow::StopPow_AZ az(13);         // aluminum, Z=13
    std::cout << "A-Z dEdx at 10 MeV: " << az.dEdx(10.0) << " MeV/um" << std::endl;

    StopPow::StopPow_AZ az_rho(13, 2.7); // with density (g/cc) for rhoR mode
    az_rho.set_mode(StopPow::StopPow::MODE_RHOR);
    std::cout << "A-Z dEdx (rhoR): " << az_rho.dEdx(10.0) << " MeV/(mg/cm2)" << std::endl;

    // --- Li-Petrasso: proton in fully ionized hydrogen plasma ---
    // Field species: hydrogen ions + electrons
    std::vector<double> mf = {1.0, 1.0/1800.0};   // mass (AMU)
    std::vector<double> Zf = {1.0, -1.0};           // charge (e)
    std::vector<double> Tf = {1.0, 1.0};             // temperature (keV)
    std::vector<double> nf = {1e24, 1e24};           // density (1/cm^3)

    StopPow::StopPow_LP lp(1, 1, mf, Zf, Tf, nf);  // test particle: proton
    std::cout << "L-P dEdx at 3 MeV: " << lp.dEdx(3.0) << " MeV/um" << std::endl;
    std::cout << "L-P Range at 10 MeV: " << lp.Range(10.0) << " um" << std::endl;

    return 0;
}
```

Compile and run:

```bash
# Using the CMake build
clang++ -std=c++17 -Isrc -o my_calc my_calc.cpp -Lbuild -lStopPow \
    $(gsl-config --libs)

./my_calc
```

## Python Usage

### Building the Python Module

```bash
cd python_swig
swig -python -c++ StopPow.i
python3 setup.py build_ext --inplace
```

This produces `_StopPow.so` and `StopPow.py`. You can also use the makefile which places output in `dist/`:

```bash
cd python_swig
make
# module is now in python_swig/dist/
```

### Python API

The Python bindings mirror the C++ API. Use `DoubleVector` (SWIG wrapper for `std::vector<double>`) to pass arrays to constructors.

```python
from StopPow import *

# --- SRIM: proton in solid aluminum ---
srim = StopPow_SRIM("path/to/Hydrogen in Aluminum.txt")

srim.set_mode(srim.MODE_LENGTH)
print(srim.dEdx(10.0))             # -0.00918 MeV/um
print(srim.Eout(10.0, 100.0))      # 9.045 MeV (exit energy after 100 um)
print(srim.Ein(10.0, 100.0))       # 10.889 MeV (incident energy)
print(srim.Thickness(10.0, 9.0))   # 104.6 um
print(srim.Range(10.0))            # 624.8 um

srim.set_mode(srim.MODE_RHOR)
print(srim.dEdx(10.0))             # -0.03398 MeV/(mg/cm2)


# --- Li-Petrasso: proton in hydrogen plasma ---
# Field particles: ions + electrons
mf = DoubleVector(2)    # mass (AMU)
mf[0] = 1.0             # hydrogen ion
mf[1] = 1.0 / 1800.0    # electron

Zf = DoubleVector(2)    # charge (units of e)
Zf[0] = 1.0
Zf[1] = -1.0

Tf = DoubleVector(2)    # temperature (keV)
Tf[0] = 1.0
Tf[1] = 1.0

nf = DoubleVector(2)    # density (1/cm^3)
nf[0] = 1e24
nf[1] = 1e24

# test particle: proton (mass=1 AMU, charge=1 e)
lp = StopPow_LP(1, 1, mf, Zf, Tf, nf)
print(lp.dEdx(10.0))               # -0.01442 MeV/um
print(lp.Eout(10.0, 100.0))        # 8.463 MeV
print(lp.Range(10.0))              # 471.96 um


# --- Bethe-Bloch: proton in diamond ---
mf2 = DoubleVector(1)
mf2[0] = 12.0           # carbon mass (AMU)

Zf2 = DoubleVector(1)
Zf2[0] = 6.0            # carbon atomic number

nf2 = DoubleVector(1)
nf2[0] = 1.76e23        # number density (1/cm^3)

bb = StopPow_BetheBloch(1, 1, mf2, Zf2, nf2)
print(bb.dEdx(10.0))               # -0.01412 MeV/um


# --- Andersen-Ziegler: proton in any element by Z ---
az = StopPow_AZ(13)                # aluminum (Z=13)
print(az.dEdx(10.0))               # stopping power in MeV/um

az_rho = StopPow_AZ(13, 2.7)       # with density (g/cc), enables MODE_RHOR
az_rho.set_mode(az_rho.MODE_RHOR)
print(az_rho.dEdx(10.0))           # stopping power in MeV/(mg/cm2)
```

### Running the Python Example

```bash
cd examples/python
# Ensure the built module is on the Python path:
PYTHONPATH=../../python_swig/dist python3 main.py
```

### C++ to Python Reference

| C++ | Python |
|-----|--------|
| `std::vector<double>` | `DoubleVector` |
| `std::vector<int>` | `IntVector` |
| `StopPow::StopPow_LP(...)` | `StopPow_LP(...)` (no namespace) |
| `std::invalid_argument` | Python `Exception` |
| `#include "StopPow_LP.h"` | `from StopPow import *` (all models) |

## C++ API Reference

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
| `std::string get_type()` | Model type descriptor |
| `std::string get_info()` | Model information string |

### Calculation Modes

- `MODE_LENGTH` (default): lengths in **um**, stopping power in **MeV/um**
- `MODE_RHOR`: lengths in **mg/cm^2**, stopping power in **MeV/(mg/cm^2)**

### Units

| Quantity | Unit |
|----------|------|
| Energy | MeV |
| Length | um (MODE_LENGTH) or mg/cm^2 (MODE_RHOR) |
| Temperature | keV (plasma models) |
| Number density | 1/cm^3 (plasma models) |
| Mass | AMU |
| Charge | units of e |

### SRIM Data Files

StopPow_SRIM reads standard SRIM text output files. Example data files are included in `test/SRIM/` and `examples/c++/data/`. The file path is relative to the current working directory.

## Directory Structure

```
StopPow/
  src/            C++ source files and headers
  lib/            Makefile to build static library
  test/           Test suite (test0-test8) with SRIM data and reference CSVs
  examples/
    c++/          C++ usage example with sample SRIM data
    python/       Python usage example
  python_swig/    SWIG Python bindings (StopPow.i, setup.py)
  doc/            Doxygen documentation config
```

## License

GNU General Public License v2. See [LICENSE](LICENSE).

## Authors

Alex Zylstra (MIT / PSFC)
