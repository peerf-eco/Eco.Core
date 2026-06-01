"""Console output helpers for client examples."""

from typing import Optional

from rich.console import Console, Group, RenderableType
from rich.live import Live
from rich.panel import Panel
from rich.text import Text


class EcoConsole:
    """Styled console output for EcoOS client examples."""

    def __init__(self) -> None:
        """Create the styled console and its single reusable live region."""
        self._console = Console()
        self._live = Live(console=self._console, auto_refresh=False)

    def print(self, *args, **kwargs) -> None:
        """Print through the underlying `Rich` console.

        Args:
            *args: Positional arguments.
            **kwargs: Keyword arguments.
        """
        self._console.print(*args, **kwargs)

    def live(self, *renderables: RenderableType, start: bool = False, stop: bool = False) -> None:
        """Drive a single in-place `Live` region on the underlying console.

        Args:
            *renderables: Frame content.
            start: Open (or restart) the region before drawing this frame.
            stop: Close the region after drawing this frame.
        """
        frame: Optional[RenderableType] = None
        if renderables:
            parts = [
                Text.from_markup(render) if isinstance(render, str) else render
                for render in renderables
            ]
            frame = next(iter(parts)) if len(parts) == 1 else Group(*parts)

        if start:
            self._live.start()
        if frame is not None:
            self._live.update(frame, refresh=True)
        if stop:
            self._live.stop()

    def header(self, title: str, subtitle: Optional[str] = None) -> None:
        """Print a prominent bordered header panel.

        Args:
            title: Heading text, drawn in bold cyan.
            subtitle: Optional second line drawn dim beneath the title.
        """
        text = Text(title, style="bold cyan")
        if subtitle:
            text.append("\n")
            text.append(subtitle, style="dim")
        self._console.print(Panel(text, border_style="blue", padding=(0, 2)))

    def success(self, msg: str) -> None:
        """Print a success line prefixed with a green check.

        Args:
            msg: Message text.
        """
        self._console.print(f"[green]✓[/green] {msg}")

    def error(self, msg: str) -> None:
        """Print an error line prefixed with a red cross.

        Args:
            msg: Message text.
        """
        self._console.print(f"[red]✗[/red] {msg}")

    def info(self, msg: str) -> None:
        """Print an info line prefixed with a cyan arrow.

        Args:
            msg: Message text.
        """
        self._console.print(f"[cyan]>[/cyan] {msg}")


console = EcoConsole()
