# eco-python2acom

Python bridge for `ACOM` component technology (`EcoOS`).

Provides declarative decorators for defining and consuming `EcoOS` interfaces from Python via `ctypes`.

---

## What is ACOM?

`ACOM` (Advanced Component Object Model) is a component architecture used in `EcoOS` — a research unikernel operating system. It follows a COM-like model: components expose typed interfaces identified by GUIDs, communicate through vtable pointers, and manage lifetime via reference counting (`AddRef` / `Release`).

## What does this library do?

`eco-python2acom` lets you interact with `EcoOS` shared libraries directly from Python:

- **Define interfaces and data layouts** declaratively using `@interface`, `@model`, `@union`
- **Implement ACOM components in Python** using `@component`, `@view`, `@factory`
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
    eco.bus.obj.QueryComponent(byref(cid), None, byref(iid), byref(ppv))
    calc = cast(ppv, Ptr[IEcoCalculatorX])
    result = calc.obj.Addition(10, 20)
    calc.Release()
```

## Package structure

| Package | Description |
|---|---|
| `eco_python2acom.decorators` | `@model`, `@union`, `@stub`, `@interface`, `@view`, `@component`, `@factory` decorators |
| `eco_python2acom.types` | Primitive types, `Ptr`, `Array`, `UGUID`, errors, utilities (`sizeof`, `cast`, `pointer`, `byref`, `addressof`, `offsetof`) |
| `eco_python2acom.guids` | IID, CID, GID constants |
| `eco_python2acom.interfaces` | Built-in EcoOS interface definitions (`IEcoUnknown`, `IEcoInterfaceBus1`, `IEcoMemoryManager1`, `IEcoFileManager1`, etc.) |
| `eco_python2acom.runtime` | `EcoSystem` bootstrap and library loader |
