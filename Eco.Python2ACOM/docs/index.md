# eco-python2acom

Python bridge for `ACOM` component technology (`EcoOS`).

Provides declarative decorators for defining and consuming `EcoOS` interfaces from Python via `ctypes`.

---

## What is ACOM?

`ACOM` (Advanced Component Object Model) is a component architecture used in `EcoOS` — a research unikernel operating system. It follows a COM-like model: components expose typed interfaces identified by GUIDs, communicate through vtable pointers, and manage lifetime via reference counting (`AddRef` / `Release`).

## What does this library do?

`eco-python2acom` lets you interact with `EcoOS` shared libraries directly from Python:

- **Define interfaces** declaratively using `@interface`, `@model`, `@union`
- **Bootstrap the runtime** (`InterfaceBus`, `MemoryManager`, `FileSystemManagement`) with a single `EcoSystem` context manager
- **Load components** by CID and query interfaces by IID, exactly as in C

## Quick start

```python
from eco_python2acom.runtime.system import EcoSystem
from eco_python2acom.types.pointer import Ptr
from eco_python2acom.types.core import Void
from eco_python2acom.types.utils import byref, cast

with EcoSystem(runtime_path="/path/to/rt", user_lib_dir="/path/to/components") as eco:
    ppv = Ptr[Void]()
    eco.bus.QueryComponent(byref(cid), None, byref(iid), byref(ppv))
    calc = cast(ppv, Ptr[IEcoCalculatorX])
    result = calc.Addition(10, 20)
    calc.Release()
```

## Package structure

| Package | Description |
|---|---|
| `eco_python2acom.decorators` | `@model`, `@union`, `@interface` decorators |
| `eco_python2acom.types` | Primitive types, `Ptr`, `Array`, `UGUID`, errors |
| `eco_python2acom.guids` | IID, CID, GID constants |
| `eco_python2acom.interfaces` | Built-in EcoOS interface definitions |
| `eco_python2acom.runtime` | `EcoSystem` bootstrap and library loader |
