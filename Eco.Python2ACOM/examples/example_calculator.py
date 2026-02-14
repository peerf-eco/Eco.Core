#!/usr/bin/env python3
"""Example: Using Calculator component through Python2ACOM bridge.

This example demonstrates:
1. Defining interfaces with @interface and @method decorators
2. Inheritance from IEcoUnknown (QueryInterface, AddRef, Release)
3. Using EcoSystem to bootstrap the runtime
4. Using bus.QueryComponent to create typed component instances
5. Direct interface instantiation for full IDE autocomplete

Prerequisites:
    - ECO_FRAMEWORK_RT environment variable set
    - Calculator component DLL in the working directory
      (or specify user_dll_path when creating EcoSystem)
"""

from __future__ import annotations

import sys

from eco_python2acom.core.errors import EcoError
from eco_python2acom.core.guid import UGUID
from eco_python2acom.core.types import ByRef, VoidPtr
from eco_python2acom.runtime.system import EcoSystem
from examples.calculator_interfaces import CID_EcoCalculator, IEcoCalculatorX, IEcoCalculatorY
from examples.console import (
    console,
    logger,
    print_error,
    print_header,
    print_info,
    print_section,
    print_success,
)

# Rich availability check for table output
try:
    from rich.table import Table

    _RICH_AVAILABLE = True
except ImportError:
    _RICH_AVAILABLE = False


def _build_op_table(calc_x: IEcoCalculatorX, op_name: str, op_symbol: str, op_func) -> None:
    """Build and print a table of operation results (IEcoCalculatorX)."""
    if _RICH_AVAILABLE and console:
        table = Table(
            title=f"IEcoCalculatorX — {op_name}",
            show_header=True,
            header_style="bold magenta",
            border_style="blue",
        )
        table.add_column("a", justify="right", style="cyan")
        table.add_column(op_symbol, justify="center", style="yellow")
        table.add_column("b", justify="left", style="cyan")
        table.add_column("=", justify="center")
        table.add_column("result", justify="right", style="green")

        pairs = (
            [
                (10, 20),
                (100, 200),
                (-50, 100),
                (32767, 1),
            ]
            if op_name == "Addition"
            else [(50, 30), (100, 200), (0, -100), (-50, -30)]
        )

        for a, b in pairs:
            result = op_func(a, b)
            table.add_row(str(a), op_symbol, str(b), "=", str(result))

        console.print(table)
    else:
        print(f"  {op_name}:")
        pairs = (
            [
                (10, 20),
                (100, 200),
                (-50, 100),
                (32767, 1),
            ]
            if op_name == "Addition"
            else [(50, 30), (100, 200), (0, -100), (-50, -30)]
        )
        for a, b in pairs:
            result = op_func(a, b)
            print(f"    {a:>6} {op_symbol} {b:<6} = {result}")


def _build_op_table_y(calc_y: IEcoCalculatorY, op_name: str, op_symbol: str, op_func) -> None:
    """Build and print a table of operation results (IEcoCalculatorY)."""
    if _RICH_AVAILABLE and console:
        table = Table(
            title=f"IEcoCalculatorY — {op_name}",
            show_header=True,
            header_style="bold magenta",
            border_style="blue",
        )
        table.add_column("a", justify="right", style="cyan")
        table.add_column(op_symbol, justify="center", style="yellow")
        table.add_column("b", justify="left", style="cyan")
        table.add_column("=", justify="center")
        table.add_column("result", justify="right", style="green")

        pairs = (
            [(6, 7), (100, 100), (-10, 5), (256, 256)]
            if op_name == "Multiplication"
            else [
                (100, 10),
                (42, 7),
                (-100, 5),
                (1000, 3),
            ]
        )

        for a, b in pairs:
            result = op_func(a, b)
            table.add_row(str(a), op_symbol, str(b), "=", str(result))

        console.print(table)
    else:
        print(f"  {op_name}:")
        pairs = (
            [(6, 7), (100, 100), (-10, 5), (256, 256)]
            if op_name == "Multiplication"
            else [
                (100, 10),
                (42, 7),
                (-100, 5),
                (1000, 3),
            ]
        )
        for a, b in pairs:
            result = op_func(a, b)
            print(f"    {a:>6} {op_symbol} {b:<6} = {result}")


def demo_calculator_x(calc_x: IEcoCalculatorX) -> None:
    """Demonstrate IEcoCalculatorX operations (Addition, Subtraction)."""
    print_section("IEcoCalculatorX — Addition & Subtraction", "bold cyan")
    _build_op_table(calc_x, "Addition", "+", calc_x.Addition)
    console.print() if _RICH_AVAILABLE and console else print()
    _build_op_table(calc_x, "Subtraction", "-", calc_x.Subtraction)


def demo_calculator_y(calc_y: IEcoCalculatorY) -> None:
    """Demonstrate IEcoCalculatorY operations (Multiplication, Division)."""
    print_section("IEcoCalculatorY — Multiplication & Division", "bold cyan")
    _build_op_table_y(calc_y, "Multiplication", "*", calc_y.Multiplication)
    console.print() if _RICH_AVAILABLE and console else print()
    _build_op_table_y(calc_y, "Division", "/", calc_y.Division)


def main() -> int:
    """Main example function."""
    print_header(
        "Eco.Python2ACOM — Calculator Demo",
        "ACOM component via Python bridge",
    )
    console.print() if _RICH_AVAILABLE and console else print()

    # Interface info (compact)
    logger.info("IEcoCalculatorX IID: %s", IEcoCalculatorX._iid_.to_string())
    logger.info("IEcoCalculatorY IID: %s", IEcoCalculatorY._iid_.to_string())
    logger.info("CID_EcoCalculator:   %s", CID_EcoCalculator)
    console.print() if _RICH_AVAILABLE and console else print()

    try:
        print_info("Initializing EcoSystem runtime...")

        with EcoSystem() as eco:
            print_success("EcoSystem initialized")
            console.print() if _RICH_AVAILABLE and console else print()

            cid = UGUID(CID_EcoCalculator)

            # --- IEcoCalculatorX via bus.QueryComponent ---
            print_info("Querying IEcoCalculatorX from bus...")
            ppv_x = VoidPtr()
            result = eco.bus.QueryComponent(
                ByRef(cid),
                None,
                ByRef(IEcoCalculatorX._iid_),
                ByRef(ppv_x),
            )
            if result.value != 0 or not ppv_x.value:
                print_error(f"QueryComponent failed for IEcoCalculatorX (code = {result.value})")
                return 1

            print_success("Got IEcoCalculatorX instance")
            calc_x = IEcoCalculatorX(ppv_x)

            demo_calculator_x(calc_x)
            console.print() if _RICH_AVAILABLE and console else print()

            # --- QueryInterface -> IEcoCalculatorY ---
            print_section("QueryInterface Demo", "bold yellow")
            print_info("Querying IEcoCalculatorY via QueryInterface...")
            ppv_y = VoidPtr()
            result = calc_x.QueryInterface(
                ByRef(IEcoCalculatorY._iid_),
                ByRef(ppv_y),
            )

            if result.value == 0 and ppv_y.value:
                calc_y = IEcoCalculatorY(ppv_y)
                print_success("Got IEcoCalculatorY instance")
                console.print() if _RICH_AVAILABLE and console else print()

                demo_calculator_y(calc_y)

                calc_y.Release()
                print_success("Released IEcoCalculatorY")
            else:
                print_error(f"QueryInterface failed (code = {result.value})")

            # Cleanup
            print_section("Cleanup", "bold yellow")
            calc_x.Release()
            print_success("Released IEcoCalculatorX")

        console.print() if _RICH_AVAILABLE and console else print()
        print_header("Done", "EcoSystem released successfully")
        return 0

    except FileNotFoundError as err:
        print_error(str(err))
        print_info("Set ECO_FRAMEWORK_RT and ensure calculator DLL is in CWD")
        return 1

    except EcoError as err:
        print_error(str(err))
        return 1

    except Exception as err:
        logger.exception("Unexpected error: %s", err)
        return 1


if __name__ == "__main__":
    sys.exit(main())
