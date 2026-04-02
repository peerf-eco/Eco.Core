from collections.abc import Callable

from rich.table import Table

from eco_python2acom.types.core import Int16, Int32


def get_operation_table(
    title: str,
    op_symbol: str,
    pairs: list[tuple[int, int]],
    op_func: Callable[[Int16, Int16], Int16 | Int32],
) -> Table:
    """Get a table of binary operations.

    Args:
        title: Table title.
        op_symbol: Operator symbol.
        pairs: List of (a, b) operand pairs.
        op_func: Callable that takes (a, b) and returns the result.

    Returns:
        Table representing the operation results.
    """
    table = Table(
        title=title,
        show_header=True,
        header_style="bold magenta",
        border_style="blue",
    )
    table.add_column("a", justify="right", style="cyan")
    table.add_column(op_symbol, justify="center", style="yellow")
    table.add_column("b", justify="left", style="cyan")
    table.add_column("=", justify="center")
    table.add_column("result", justify="right", style="green")

    for a, b in pairs:
        result = op_func(Int16(a), Int16(b))
        table.add_row(str(a), op_symbol, str(b), "=", str(result.value))

    return table
