"""Calculator example with interfaces (`IEcoCalculatorX` + `IEcoCalculatorY`)."""

import sys

from eco_python2acom.runtime.system import EcoSystem
from eco_python2acom.types.core import Void
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr
from eco_python2acom.types.utils import byref, cast
from examples.client.calculator.interfaces import (
    IEcoCalculatorX,
    IEcoCalculatorY,
    IID_IEcoCalculatorX,
    IID_IEcoCalculatorY,
)
from examples.client.console import console


def show(name: str, a: int, oper: str, b: int, result: int) -> None:
    """Print one operation result in a neat single-line format.

    Args:
        name: Operation name.
        a: First operand.
        oper: Operator symbol.
        b: Second operand.
        result: Operation result.
    """
    console.print(
        f"  [bold magenta]{name:<14}[/bold magenta] "
        f"[cyan]{a:>4}[/cyan] [yellow]{oper}[/yellow] [cyan]{b:<4}[/cyan] "
        f"= [green]{result}[/green]"
    )


def main() -> int:
    """Run calculator example."""
    CID_EcoCalculator = UGUID("4828F655-2E45-40E7-8121-EBD220DC360E")
    console.header("Eco Calculator Example")
    console.info(f"CID:                 {CID_EcoCalculator}")
    console.info(f"IEcoCalculatorX IID: {IID_IEcoCalculatorX}")
    console.info(f"IEcoCalculatorY IID: {IID_IEcoCalculatorY}\n")

    try:
        console.info("Initializing EcoSystem...")

        with EcoSystem(user_lib_dir="data") as eco:
            console.success("EcoSystem initialized\n")

            ppv_x = Ptr[Void]()
            result = eco.bus.obj.QueryComponent(
                byref(CID_EcoCalculator), None, byref(IID_IEcoCalculatorX), byref(ppv_x)
            )
            if result != 0 or not bool(ppv_x):
                console.error(f"Failed to query component (code = {result})\n")
                return -2
            calc_x = cast(ppv_x, Ptr[IEcoCalculatorX])
            console.success(f"Got {calc_x}")

            result = calc_x.obj.Addition(a=10, b=20)
            show("Addition", 10, "+", 20, result)
            result = calc_x.obj.Subtraction(a=50, b=30)
            show("Subtraction", 50, "-", 30, result)
            console.print()

            ppv_y = Ptr[Void]()
            result = calc_x.obj.QueryInterface(byref(IID_IEcoCalculatorY), byref(ppv_y))
            if result != 0 or not bool(ppv_y):
                console.error(f"Failed to query interface (code = {result})\n")
                calc_x.Release()
                return -3
            calc_y = cast(ppv_y, Ptr[IEcoCalculatorY])
            console.success(f"Got {calc_y}")

            result = calc_y.obj.Multiplication(a=6, b=7)
            show("Multiplication", 6, "*", 7, result)
            result = calc_y.obj.Division(a=100, b=10)
            show("Division", 100, "/", 10, result)
            console.print()

            calc_y.obj.Release()
            console.success(f"Released {calc_y}")
            calc_x.obj.Release()
            console.success(f"Released {calc_x}")

        console.info("Released EcoSystem")
        return 0

    except Exception as err:
        console.error(str(err))
        return -1


if __name__ == "__main__":
    sys.exit(main())
