# Logging

A single pre-configured `eco_logger` is exposed for the whole package. Output format:

```
<YYYY-MM-DD HH:MM:SS> [LEVEL] <scope>::<func>() ---> <message>
```

Where `<scope>` is the per-class logger automatically injected into every `@component` and `@factory` (so each component logs under its own class name), and `<func>` is the name of the function that issued the log call.

Example output:

```
2026-05-13 10:23:12 [DEBUG] CEcoCalculator::Alloc() ---> Instance at <0x7f8a3c1d4280> ---> OK
2026-05-13 10:23:12 [DEBUG] CEcoCalculator::AddRef() ---> Refs = 2
2026-05-13 10:23:12 [DEBUG] CEcoCalculator::Release() ---> Refs = 0 ---> Destroyed
```

## Enabling output

Logging is silent by default — `eco_logger` ships with a `NullHandler` so no output appears until the user opts in:

```python
import logging
from eco_python2acom.runtime.logging import configure

configure(level=logging.DEBUG)
```

Add a file sink with size-based rotation:

```python
configure(level=logging.DEBUG, log_file="eco.log", max_bytes=5_000_000, backup_count=10)
```

Console output uses ANSI colors when stdout is a TTY; file output is always plain text.

## Configuration

::: eco_python2acom.runtime.logging.configure

## Helpers

::: eco_python2acom.runtime.logging.at_level

## Building blocks

::: eco_python2acom.runtime.logging.EcoFormatter

::: eco_python2acom.runtime.logging.AnsiColor

::: eco_python2acom.runtime.logging.Defaults
