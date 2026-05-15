# Eco.Python2ACOM

Python bridge for ACOM component technology (EcoOS).

[![Python](https://img.shields.io/badge/python-3.11+-3776AB?logo=python&logoColor=white)](https://www.python.org/)
[![ctypes](https://img.shields.io/badge/ctypes-FFD43B?logo=python&logoColor=black)](https://docs.python.org/3/library/ctypes.html)
[![Poetry](https://img.shields.io/badge/poetry-2.0+-60A5FA?logo=poetry&logoColor=white)](https://python-poetry.org/)
[![MkDocs Material](https://img.shields.io/badge/docs-mkdocs--material-526CFE?logo=materialformkdocs&logoColor=white)](https://squidfunk.github.io/mkdocs-material/)

## Overview

`eco-python2acom` is the Python half of the bridge between Python and `ACOM`
(_Adapted Component Object Model_) — the component architecture used by
`EcoOS`. It lets a Python program both **consume** existing `EcoOS` components
written in C and **expose** new components implemented in Python so that
native C code can use them.

The package is built around `ctypes` and a small set of declarative decorators
that hide vtable plumbing and pointer arithmetic behind regular-looking Python
classes.

## Features

- **Declarative interface and layout definitions** — `@interface`, `@model`,
  `@union`, `@stub` for describing `ACOM` interfaces and C-compatible structs.
- **Server-side ACOM components in Python** — `@component`, `@view`,
  `@factory` with full support for inclusion, containment and aggregation.
- **Runtime bootstrap** — `EcoSystem` brings up `InterfaceBus`,
  `MemoryManager`, `FileSystemManagement` in a single context manager.
- **Loader** — `EcoLibLoader` resolves library files in user
  directories and pulls out their factory export.

## Architecture

![Architecture](docs/images/client-flow.png)

## Installation

### Prerequisites

- Python **3.11+**
- `EcoOS` runtime libraries built and reachable via the `ECO_FRAMEWORK_RT`
  environment variable.
- For server-side scenarios — a working build of
  [`Eco.ACOM2Python`](../Eco.ACOM2Python/) embedded in the system.

### Editable install

For local development (recommended while iterating on examples):

```bash
poetry install
```

or with `pip`:

```bash
pip install -e .
```

## Documentation

Full reference is generated with `MkDocs Material` + `mkdocstrings` and lives
under [`docs/`](docs/). To preview locally:

```bash
poetry run mkdocs serve
```

Topical entry points:

| Topic | Page |
|---|---|
| Decorators (`@model`, `@interface`, `@view`, `@component`, `@factory`) | [`docs/decorators/`](docs/decorators/) |
| Type primitives, `Ptr`, `Array`, `UGUID`, errors, utils | [`docs/types/`](docs/types/) |
| GUIDs (IID, CID, GID) | [`docs/guids/`](docs/guids/) |
| Runtime (`EcoSystem`, `EcoLibLoader`, logging) | [`docs/runtime/`](docs/runtime/) |
| Built-in EcoOS interfaces | [`docs/interfaces/`](docs/interfaces/) |

## Examples

Runnable scenarios live under [`examples/`](examples/), split into client and
server flows:

**Client side** ([`examples/client/calculator/`](examples/client/calculator/)) — a host program that
boots `EcoSystem`, queries components by CID through the interface bus and
calls their methods from Python as if they were regular native components.

**Server side** ([`examples/server/calculator/`](examples/server/calculator/))
— four calculator components illustrating the main ACOM composition models:

| File | Demonstrates |
|---|---|
| `simple.py` | Standalone component implementing `IEcoCalculatorX` + `IEcoCalculatorY` |
| `inner.py` | Aggregatable component (`IEcoCalculatorX`-only) with a non-delegating `IEcoUnknown` |
| `inclusion.py` | Containment — owns an included `IEcoCalculatorX`, delegates arithmetic to it |
| `outer.py` | Outer aggregator that wraps an inner aggregatable calculator |

## See also

- [`Eco.ACOM2Python`](../Eco.ACOM2Python/README.md) — the C-side bridge that
  loads Python modules, embeds `CPython`, and exposes Python factories to
  native EcoOS code.
