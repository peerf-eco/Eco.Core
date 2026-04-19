# eco-python2acom

**Python bridge for ACOM component technology**

[![Python 3.11+](https://img.shields.io/badge/python-3.11+-blue.svg)](https://www.python.org/downloads/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## Overview

`eco-python2acom` is a Python library for interacting with ACOM (_Adaptive Component Object Model_) components from EcoOS. It provides a Pythonic interface for:

- Loading and registering ACOM components (automatic scan)
- Creating component instances via the interface bus
- Calling component methods through virtual tables
- Managing component lifecycles with reference counting

## Installation

```bash
# Using poetry
poetry add eco-python2acom

# Using pip
pip install eco-python2acom
```
