# eco-python2acom

**Python bridge for ACOM component technology (EcoOS)**

[![Python 3.11+](https://img.shields.io/badge/python-3.11+-blue.svg)](https://www.python.org/downloads/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Code style: black](https://img.shields.io/badge/code%20style-black-000000.svg)](https://github.com/psf/black)

## Overview

`eco-python2acom` is a Python library for interacting with ACOM (Analogous Component Object Model) components from EcoOS. It provides a Pythonic interface for:

- Loading and registering ACOM component DLLs (automatic scan)
- Creating component instances via the interface bus
- Calling component methods through virtual tables
- Managing component lifecycles with reference counting

## Features

- **Declarative Interface Definitions**: Use `@interface` and `@method` decorators to define ACOM interfaces with full IDE autocomplete
- **Automatic DLL Discovery**: EcoSystem auto-scans the working directory for component DLLs on init
- **Direct Interface Instantiation**: Interface classes are directly instantiable from `VoidPtr` — no wrapper needed
- **Type-Safe**: Full ctypes integration with Python type hints
- **Rich Console Output**: Optional colorful output in examples via [rich](https://github.com/Textualize/rich)
- **Context Manager Support**: Automatic resource cleanup with `with EcoSystem()`

## Installation

```bash
# Using poetry
poetry add eco-python2acom

# With rich support for examples (optional)
poetry add eco-python2acom[rich]

# Using pip
pip install eco-python2acom
```

## Quick Start

### 1. Set Environment Variable

```bash
# Windows
set ECO_FRAMEWORK_RT=C:\Eco\Framework\RT
```

### 2. Place Component DLL

Put your component DLL (e.g. calculator) in the working directory. EcoSystem auto-scans it on init. DLL filenames must match the EcoOS pattern: 32 hex characters + `.dll` (the CID as hex).

### 3. Define Interface

```python
from eco_python2acom.interfaces.base import IEcoUnknown
from eco_python2acom.interfaces.decorators import interface, method
from eco_python2acom.core.types import Int16, Int32

@interface(iid="93221116-2248-4742-AE06-82819447843D")
class IEcoCalculatorX(IEcoUnknown):
    """Calculator interface X."""

    @method
    def Addition(self, a: Int16, b: Int16) -> Int32:
        """Add two numbers."""
        ...

    @method
    def Subtraction(self, a: Int16, b: Int16) -> Int16:
        """Subtract two numbers."""
        ...
```

### 4. Use Component

```python
from eco_python2acom.core.guid import UGUID
from eco_python2acom.core.types import ByRef, VoidPtr
from eco_python2acom.runtime.system import EcoSystem

CID_Calculator = "4828F655-2E45-40E7-8121-EBD220DC360E"

with EcoSystem() as eco:
    cid = UGUID(CID_Calculator)
    ppv = VoidPtr()

    eco.bus.QueryComponent(
        ByRef(cid),
        None,
        ByRef(IEcoCalculatorX._iid_),
        ByRef(ppv),
    )

    calc = IEcoCalculatorX(ppv)
    result = calc.Addition(10, 20)  # 30
    print(f"10 + 20 = {result}")

    calc.Release()
```

## Package Structure

```
eco_python2acom/
├── core/                    # Core utilities
│   ├── types.py             # ctypes aliases (Int16, Int32, VoidPtr, etc.)
│   ├── guid.py              # UGUID structure
│   └── errors.py            # Error codes and exceptions
├── interfaces/              # Interface definitions
│   ├── base.py              # IEcoUnknown, IEcoComponentFactory
│   ├── decorators.py        # @interface, @method decorators
│   ├── guids/               # CID, IID, GID constants
│   │   ├── cid.py           # Component IDs
│   │   ├── iid.py           # Interface IDs
│   │   └── gid.py           # Generation IDs
│   └── system/              # System interfaces
│       ├── system.py        # IEcoSystem1
│       ├── interface_bus.py # IEcoInterfaceBus1
│       ├── memory_manager.py# IEcoMemoryManager1
│       └── file_manager.py  # IEcoFileSystemManagement1
└── runtime/                 # Runtime initialization
    ├── system.py            # EcoSystem (main entry point)
    ├── loader.py            # DllLoader
    └── helpers.py           # GUID conversion, path utilities
```

## Architecture

### EcoSystem

The main entry point that:

1. Loads system components from `ECO_FRAMEWORK_RT/{GID}/` (InterfaceBus, MemoryManager, FileSystemManagement)
2. Initializes the InterfaceBus and memory heap
3. Auto-scans `user_dll_path` (default: CWD) for component DLLs and registers them
4. Exposes the `bus` property for component querying

Usage:

```python
with EcoSystem() as eco:
    ppv = VoidPtr()
    eco.bus.QueryComponent(ByRef(cid), None, ByRef(iid), ByRef(ppv))
    obj = IMyInterface(ppv)
    obj.SomeMethod(...)
    obj.Release()
```

### Interface Decorators

Define interfaces declaratively — classes are directly instantiable from `VoidPtr`:

```python
@interface(iid="...")
class IMyInterface(IEcoUnknown):
    @method
    def MyMethod(self, arg: Int32) -> Int16:
        ...
```

The decorator automatically generates:

- VTbl ctypes structure (including inherited IEcoUnknown methods)
- Interface ctypes structure
- `__init__(ptr)`, method dispatchers, `__repr__`

### Direct Instantiation

No wrapper class needed — pass a `VoidPtr` to the interface constructor:

```python
ppv = VoidPtr()
eco.bus.QueryComponent(ByRef(cid), None, ByRef(IID_IFoo), ByRef(ppv))
foo = IFoo(ppv)       # Direct instantiation
foo.SomeMethod(42)    # IDE autocomplete works
foo.Release()
```

## Examples

Run the calculator example (requires ECO_FRAMEWORK_RT and calculator DLL in CWD):

```bash
python -m examples.example_calculator
```

With rich installed, the example prints colored tables and panels.

## Development

```bash
# Clone repository
git clone https://github.com/user/eco-python2acom.git
cd eco-python2acom

# Install dependencies
poetry install

# Run linters
poetry run black .
poetry run isort .
poetry run ruff check .
poetry run mypy eco_python2acom/

# Run tests
poetry run pytest
```

## Requirements

- Python 3.11+
- Windows (primary platform)
- EcoOS runtime DLLs in `ECO_FRAMEWORK_RT`
