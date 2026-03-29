"""Console and logging helpers for examples.

This module provides a single place for all example output: a configured
logger with rich formatting and helper functions for headers, sections,
messages.

Module-level attributes:
    logger: Pre-configured ``logging.Logger`` (name ``eco.example``).
    console: Rich ``Console`` instance for direct output.

Helper functions:
    print_header: Prominent panel with title and optional subtitle.
    print_section: Section divider with styled title.
    print_success: Green check and message.
    print_error: Red cross and message.
    print_info: Cyan arrow and message.
"""

from __future__ import annotations

import logging
from typing import Optional

from rich.console import Console
from rich.logging import RichHandler
from rich.panel import Panel
from rich.text import Text

# =============================================================================
# Logger and Console
# =============================================================================

logger = logging.getLogger("Eco")
logger.setLevel(logging.DEBUG)

console = Console()
handler = RichHandler(
    show_time=False,
    show_path=False,
    markup=True,
    rich_tracebacks=True,
    console=console,
)
handler.setLevel(logging.DEBUG)
logger.addHandler(handler)
logger.propagate = False


# =============================================================================
# Headers and sections
# =============================================================================


def print_header(title: str, subtitle: Optional[str] = None) -> None:
    """Print a prominent header panel.

    Args:
        title: Main title text (bold cyan).
        subtitle: Optional subtitle (dim), shown below title.
    """
    text = Text(title, style="bold cyan")
    if subtitle:
        text.append("\n")
        text.append(subtitle, style="dim")
    console.print(Panel(text, border_style="blue", padding=(0, 2)))


def print_section(title: str, style: str = "bold yellow") -> None:
    """Print a section divider.

    Args:
        title: Section title.
        style: Rich style string (default: bold yellow).
    """
    console.print(f"\n[{style}]>>> {title}[/{style}]\n")


# =============================================================================
# Status messages
# =============================================================================


def print_success(msg: str) -> None:
    """Print a success message with green check.

    Args:
        msg: Message text.
    """
    console.print(f"[green]✓[/green] {msg}")


def print_error(msg: str) -> None:
    """Print an error message with red cross.

    Args:
        msg: Message text.
    """
    console.print(f"[red]✗[/red] {msg}")


def print_info(msg: str) -> None:
    """Print an info message with cyan arrow.

    Args:
        msg: Message text.
    """
    console.print(f"[cyan]→[/cyan] {msg}")
