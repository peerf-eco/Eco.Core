# Eco.ACOM2Python

C-side bridge that lets EcoOS components call Python implementations via `CPython` embedding.

## Build & Run (Windows, MSVC + libffi + CPython)

### 1. Prerequisites

| Tool           | Notes                                                                                                                  |
|----------------|------------------------------------------------------------------------------------------------------------------------|
| `Visual Studio`| Toolset is not set in the project files.                                                                               |
| `Python 3.x`   | Install from [python.org](https://www.python.org/downloads/). Pick the architecture (32-bit or 64-bit) that matches the `Platform` you intend to build (`Win32` ↔ 32-bit Python, `x64` ↔ 64-bit Python). For `Debug` configurations also tick **Download debug binaries** in the installer's options. |
| `libffi`       | Pre-built MSVC binaries on the [libffi releases page](https://github.com/libffi/libffi/releases): pick the 32-bit ZIP for `Win32`, the 64-bit ZIP for `x64`. |

### 2. Environment variables (User scope)

| Variable           | Used by         | Notes                                                                  |
|--------------------|-----------------|------------------------------------------------------------------------|
| `PYTHON_HOME`      | build & runtime | Path to `Python` installation.                                         |
| `PYTHON_VERSION`   | build           | Selects `python$(PYTHON_VERSION)[_d].lib` at link time.                |
| `LIBFFI_HOME`      | build & runtime | Path to `libffi` installation.                                         |
| `ECO_FRAMEWORK`    | build           | Eco component sources (interfaces and per-component build outputs).    |
| `ECO_FRAMEWORK_RT` | runtime         | Eco runtime libraries (`InterfaceBus1`, `MemoryManager1`, `FileSystemManagement1`, …). |

After setting them, restart `Visual Studio` so the values are picked up.

### 3. Install the Python runtime package

The Python module loaded by the bridge imports `eco_python2acom`. Install the package once into the same interpreter that `PYTHON_HOME` points at:

```cmd
pip install -e eco_python2acom
```

### 4. Build

Open `Eco.ACOM2Python\AssemblyFiles\Windows\VS_v100\EcoACOM2Python.sln` in Visual Studio, choose need configuration and click **Build Solution**. The bridge library and the unit-test executable land in `BuildFiles\Windows\<Platform>\<Configuration>\`.

### 5. Runtime layout next to `EcoACOM2PythonUnitTest.exe`

| File                                                  | Source                                                                  |
|-------------------------------------------------------|-------------------------------------------------------------------------|
| `219EDB626EF14B42BE16F93A566F1CC3.dll`                | bridge build output                                                     |
| `python$(PYTHON_VERSION)[_d].dll`                     | `$(PYTHON_HOME)\`                                                       |
| `libffi-8.dll`                                        | `$(LIBFFI_HOME)\`                                                       |
| `8039E233E9A34D43BAF7833001434A0B.dll` (Eco.TypeLib1) | `Eco.TypeLib1\BuildFiles\Windows\<Platform>\<Configuration>\`           |
| `53884AFC93C448ECAA929C8D3A562281.dll` (Eco.List1)    | `Eco.List1\BuildFiles\Windows\<Platform>\<Configuration>\`              |
| `9322111622484742AE0682819447843D.etl`                | `Eco.TypeLib1\BuildFiles\Windows\<Platform>\<Configuration>\`           |
| `BD6414C29096423EA90C04D77AFD1CAD.etl`                | `Eco.TypeLib1\BuildFiles\Windows\<Platform>\<Configuration>\`           |
