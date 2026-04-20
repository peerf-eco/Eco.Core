"""Console output helpers for examples."""

from typing import Optional

from rich.console import Console
from rich.panel import Panel
from rich.text import Text


class EcoConsole:
    """Styled console output for EcoOS examples."""

    def __init__(self) -> None:
        self._console = Console()

    def print(self, *args, **kwargs) -> None:
        """Pass-through to the underlying Rich console."""
        self._console.print(*args, **kwargs)

    def header(self, title: str, subtitle: Optional[str] = None) -> None:
        """Print a prominent header panel."""
        text = Text(title, style="bold cyan")
        if subtitle:
            text.append("\n")
            text.append(subtitle, style="dim")
        self._console.print(Panel(text, border_style="blue", padding=(0, 2)))

    def success(self, msg: str) -> None:
        """Print a success message with green check."""
        self._console.print(f"[green]✓[/green] {msg}")

    def error(self, msg: str) -> None:
        """Print an error message with red cross."""
        self._console.print(f"[red]✗[/red] {msg}")

    def info(self, msg: str) -> None:
        """Print an info message with cyan arrow."""
        self._console.print(f"[cyan]>[/cyan] {msg}")


console = EcoConsole()
