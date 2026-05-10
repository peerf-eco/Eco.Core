# Eco.ACOM2Python

C-side bridge that lets EcoOS components call Python implementations via `CPython` embedding.

## Build & Run (Windows, MSVC + CPython)

### 1. Prerequisites

| Tool           | Notes                                                                                                                  |
|----------------|------------------------------------------------------------------------------------------------------------------------|
| `Visual Studio`| Toolset is not set in the project files.                                                                               |
| `Python 3.x`   | Install from [python.org](https://www.python.org/downloads/). Pick the architecture (32-bit or 64-bit) that matches the `Platform` you intend to build (`Win32` ↔ 32-bit Python, `x64` ↔ 64-bit Python). For `Debug` configurations also tick **Download debug binaries** in the installer's options. |

### 2. Environment variables (User scope)

| Variable           | Used by         | Notes                                                                  |
|--------------------|-----------------|------------------------------------------------------------------------|
| `PYTHON_HOME`      | build & runtime | Path to `Python` installation (3.6 or newer).                          |
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
| `python3.dll`                                         | `$(PYTHON_HOME)\`                                                       |
| `53884AFC93C448ECAA929C8D3A562281.dll` (Eco.List1)    | `Eco.List1\BuildFiles\Windows\<Platform>\<Configuration>\`              |
