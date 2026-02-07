"""Console output helpers for examples.

Provides a pre-configured ``logging.Logger`` with colored output
via rich (if available) or plain formatting as a fallback.

Also exposes ``console`` and helper functions for rich output when
the library is installed.
"""

from __future__ import annotations

import logging
from typing import Optional

# Logger for all examples
logger = logging.getLogger("eco.example")
logger.setLevel(logging.DEBUG)

try:
    from rich.console import Console
    from rich.logging import RichHandler

    console = Console()
    _handler: logging.Handler = RichHandler(
        show_time=False,
        show_path=False,
        markup=True,
        rich_tracebacks=True,
        console=console,
    )
    _handler.setLevel(logging.DEBUG)
    _RICH_AVAILABLE = True

except ImportError:
    _handler = logging.StreamHandler()
    _handler.setLevel(logging.DEBUG)
    _handler.setFormatter(logging.Formatter("[%(levelname)s] %(message)s"))
    _RICH_AVAILABLE = False

logger.addHandler(_handler)
logger.propagate = False


def print_header(title: str, subtitle: Optional[str] = None) -> None:
    """Print a prominent header with optional subtitle."""
    if _RICH_AVAILABLE:
        from rich.panel import Panel
        from rich.text import Text

        text = Text(title, style="bold cyan")
        if subtitle:
            text.append("\n")
            text.append(subtitle, style="dim")
        console.print(Panel(text, border_style="blue", padding=(0, 2)))
    else:
        print(f"\n=== {title} ===")
        if subtitle:
            print(subtitle)
        print()


def print_section(title: str, style: str = "bold yellow") -> None:
    """Print a section divider."""
    if _RICH_AVAILABLE:
        console.print(f"\n[{style}]>>> {title}[/{style}]\n")
    else:
        print(f"\n>>> {title}\n")


def print_success(msg: str) -> None:
    """Print a success message."""
    if _RICH_AVAILABLE:
        console.print(f"[green]✓[/green] {msg}")
    else:
        print(f"OK: {msg}")


def print_error(msg: str) -> None:
    """Print an error message."""
    if _RICH_AVAILABLE:
        console.print(f"[red]✗[/red] {msg}")
    else:
        print(f"ERROR: {msg}")


def print_info(msg: str) -> None:
    """Print an info message."""
    if _RICH_AVAILABLE:
        console.print(f"[cyan]→[/cyan] {msg}")
    else:
        print(f"INFO: {msg}")
