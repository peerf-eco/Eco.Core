"""Logging tools for `eco_python2acom`.

A single pre-configured `eco_logger` is exposed for the whole package. Format:

    `<YYYY-MM-DD HH:MM:SS> [LEVEL] <scope>::<func>() ---> <message>`

ANSI colors are applied to console output when stdout is a TTY.
"""

import logging
import logging.handlers
import sys
from contextlib import ContextDecorator
from types import TracebackType
from typing import Optional

# -----------------------------------------------------------------------------
# ANSI color codes
# -----------------------------------------------------------------------------


class AnsiColor:
    """ANSI escape codes used to decorate console output by log level."""

    RESET = "\033[0m"
    DIM = "\033[2m"
    CYAN = "\033[36m"
    YELLOW = "\033[33m"
    RED = "\033[31m"
    GREEN = "\033[32m"


# -----------------------------------------------------------------------------
# Defaults
# -----------------------------------------------------------------------------


class Defaults:
    """Default values used for logging."""

    LOGGER_NAME = "eco_python2acom"
    LEVEL = logging.WARNING
    ROTATION_MAX_BYTES = 10 * 1024 * 1024  # 10 MB per file
    ROTATION_BACKUP_COUNT = 5  # Keep last 5 rotated files
    LEVEL_COLORS = {
        logging.DEBUG: AnsiColor.DIM,
        logging.INFO: AnsiColor.CYAN,
        logging.WARNING: AnsiColor.YELLOW,
        logging.ERROR: AnsiColor.RED,
        logging.CRITICAL: AnsiColor.RED,
    }


# -----------------------------------------------------------------------------
# Formatter
# -----------------------------------------------------------------------------


class EcoFormatter(logging.Formatter):
    """Logging formatter for `eco_python2acom`.

    Renders each record as `<YYYY-MM-DD HH:MM:SS> [LEVEL] <scope>::<func>() ---> <message>`,
    optionally wrapping the line in ANSI color codes keyed by log level.

    Notes:
        - The `<scope>` is the logger name with the `eco_python2acom.` prefix stripped.
        - The root package logger is rendered as `core`.
        - The `<func>` is the name of the function that issued the log call.
    """

    def __init__(self, use_color: bool) -> None:
        """Initialize the formatter.

        Args:
            use_color: When `True`, wrap each line in ANSI color escapes.
        """
        super().__init__()
        self._use_color = use_color

    def format(self, record: logging.LogRecord) -> str:
        """Render a single log record.

        Args:
            record: The log record produced by the logging machinery.

        Returns:
            The formatted log line, with ANSI color escapes applied when
            color output is enabled.
        """
        timestamp = self.formatTime(record, datefmt="%Y-%m-%d %H:%M:%S")

        level = f"{record.levelname:<5}"
        scope = record.name
        if scope.startswith(Defaults.LOGGER_NAME + "."):
            scope = scope[len(Defaults.LOGGER_NAME) + 1 :]
        elif scope == Defaults.LOGGER_NAME:
            scope = "core"

        message = record.getMessage()
        line = f"{timestamp} [{level}] {scope}::{record.funcName}() ---> {message}"

        if self._use_color:
            color = Defaults.LEVEL_COLORS.get(record.levelno, "")
            if color:
                line = f"{color}{line}{AnsiColor.RESET}"

        return line


# -----------------------------------------------------------------------------
# Setup
# -----------------------------------------------------------------------------


eco_logger: logging.Logger = logging.getLogger(Defaults.LOGGER_NAME)
eco_logger.propagate = False
eco_logger.addHandler(logging.NullHandler())


def configure(
    name: Optional[str] = None,
    level: int = Defaults.LEVEL,
    use_color: Optional[bool] = None,
    log_file: Optional[str] = None,
    max_bytes: int = Defaults.ROTATION_MAX_BYTES,
    backup_count: int = Defaults.ROTATION_BACKUP_COUNT,
) -> logging.Logger:
    """Configure a logger using the `eco_python2acom` style.

    Without arguments, configures the package-wide `eco_logger`. Pass `name`
    to create or reconfigure a separate logger. Safe to call multiple times —
    handlers are replaced on each call. Output goes either to the console
    **or** to a rotating file.

    Args:
        name: Logger name. If `None`, the package-wide logger is configured.
        level: Verbosity threshold; messages below it are dropped.
        use_color: Force-enable or force-disable ANSI colors on console.
            If `None`, colors are enabled when stdout is a TTY.
        log_file: Path to a log file with size-based rotation.
        max_bytes: Per-file size limit before rotation kicks in.
        backup_count: Number of rotated files to keep alongside the live one.

    Returns:
        The configured logger.
    """
    target = eco_logger if name is None else logging.getLogger(name)
    target.propagate = False
    target.setLevel(level)

    for handler in list(target.handlers):
        target.removeHandler(handler)

    if log_file:
        file_handler = logging.handlers.RotatingFileHandler(
            filename=log_file,
            mode="a",
            encoding="utf-8",
            maxBytes=max_bytes,
            backupCount=backup_count,
        )
        file_handler.setFormatter(EcoFormatter(use_color=False))
        target.addHandler(file_handler)
    else:
        if use_color is None:
            use_color = sys.stdout.isatty()
        console_handler = logging.StreamHandler(sys.stdout)
        console_handler.setFormatter(EcoFormatter(use_color=use_color))
        target.addHandler(console_handler)

    return target


# -----------------------------------------------------------------------------
# Helpers
# -----------------------------------------------------------------------------


class at_level(ContextDecorator):
    """Temporarily change a logger's level — usable as both `with` block and `@decorator`.

    Restores the previous level when the block / function exits, even on exception.

    Example:
        ```python
        with at_level(logging.DEBUG):
            eco.bus.obj.QueryComponent(...)

        @at_level(logging.DEBUG)
        def test():
            ...
        ```
    """

    def __init__(self, level: int, target: Optional[logging.Logger] = None) -> None:
        """Initialize the level scope.

        Args:
            level: Verbosity threshold to apply inside the block / function.
            target: Logger to reconfigure. Defaults to the package-wide `eco_logger`.
        """
        self._level = level
        self._target = target if target is not None else eco_logger
        self._previous: Optional[int] = None

    def __enter__(self) -> logging.Logger:
        self._previous = self._target.level
        self._target.setLevel(self._level)
        return self._target

    def __exit__(
        self,
        exc_type: Optional[type[BaseException]],
        exc_val: Optional[BaseException],
        exc_tb: Optional[TracebackType],
    ) -> None:
        if self._previous is not None:
            self._target.setLevel(self._previous)


__all__ = ["eco_logger", "configure", "at_level", "EcoFormatter", "AnsiColor", "Defaults"]
