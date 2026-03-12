# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build & Test Commands

### CMake (preferred)
```bash
mkdir build && cd build
cmake ..
make -j$(sysctl -n hw.ncpu)   # build all (library, tests, example)
ctest                           # run all tests
ctest -R test3                  # run a single test
```

### Make
```bash
cd lib && make              # build static library → lib/lib/StopPow.a, headers → lib/include/
cd test && make test        # build and run all 9 tests (test0-test8)
cd test && make test0.out && ./test0.out   # build and run a single test
cd lib && make clean        # clean build artifacts
```

### Dependencies
- C++17, GSL (`brew install gsl` on macOS arm64, installed at `/opt/homebrew`)
- macOS Makefiles use `/opt/homebrew/include` and `/opt/homebrew/lib` for GSL paths

## Architecture

All code lives in `src/` under the `StopPow` namespace. The class hierarchy:

```
StopPow (base) ─── dEdx, Eout, Ein, Thickness, Range (GSL ODE integration)
├── StopPow_SRIM          ← tabulated data, interpolation
├── StopPow_BetheBloch    ← cold matter, analytic
├── StopPow_AZ            ← cold matter, empirical fits (protons only)
├── StopPow_Plasma (base) ← adds multi-species plasma field parameters
│   ├── StopPow_LP        ← Li-Petrasso (fully ionized)
│   ├── StopPow_BPS       ← Brown-Preston-Singleton (fully ionized)
│   └── StopPow_Grabowski ← MD fit (fully ionized)
├── StopPow_PartialIoniz (base) ← adds Zbar (ionization state)
│   ├── StopPow_Zimmerman ← partially ionized
│   ├── StopPow_Mehlhorn  ← partially ionized
│   └── StopPow_Fit       ← adjustable model for fitting
└── Spectrum, PlotGen, Fit, AtomicData ← utilities
```

**Key design**: Every model implements `dEdx_MeV_um()` and `dEdx_MeV_mgcm2()` (pure virtual). The base class provides `Eout`, `Ein`, `Thickness`, `Range` via GSL ODE solvers that call `dEdx()`. Two modes: `MODE_LENGTH` (um) and `MODE_RHOR` (mg/cm^2).

**Units**: Energy in MeV, length in um, temperature in keV, density in 1/cm^3, mass in AMU.

## Known Issues

- **test5 and test6 fail** with current GSL 2.8 — pre-existing numerical precision differences in the expected values, not regressions
- `Fit.cpp` uses `gsl_multifit_fdfsolver_jac()` (GSL 2.x API); older GSL used `solver->J` directly
- Linux/Windows Makefiles still reference `-std=c++11` (only Darwin paths were updated to C++17)

## Bindings

- `python_swig/`: SWIG-generated Python bindings (`setup.py`, `StopPow.i`)
