#!/usr/bin/env python3
"""Example: Using Eco.Calculator component through Python2ACOM bridge.

This example demonstrates:
1. How interfaces are defined using @interface and @method decorators
2. How EcoSystem initializes the runtime (InterfaceBus, MemoryManager)
3. How to load a component DLL and register it
4. How to create component instances and call their methods
5. How QueryInterface works to get different interfaces from a component

Prerequisites:
    - ECO_FRAMEWORK_RT environment variable set to runtime path
    - Calculator component DLL

Run:
    python -m examples.example_calculator
"""

from __future__ import annotations

import sys
from pathlib import Path

from eco_python2acom.client.component import ComponentWrapper
from eco_python2acom.core.errors import EcoError
from eco_python2acom.core.guid import UGUID
from eco_python2acom.runtime.system import EcoSystem
from examples.calculator_interfaces import CID_EcoCalculator, IEcoCalculatorX, IEcoCalculatorY


def print_header(text: str) -> None:
    """Print a formatted header."""
    print(f"\n{'=' * 70}")
    print(f"{text:^70}")
    print(f"{'=' * 70}\n")


def print_ok(text: str) -> None:
    """Print success message."""
    print(f"[OK] {text}")


def print_err(text: str) -> None:
    """Print error message."""
    print(f"[ERROR] {text}")


def print_info(text: str) -> None:
    """Print info message."""
    print(f"> {text}")


def print_result(operation: str, result: int) -> None:
    """Print calculation result."""
    print(f"  {operation} = {result}")


def demo_calculator_x(calc_x: ComponentWrapper) -> None:
    """Demonstrate IEcoCalculatorX operations (Addition, Subtraction).

    Args:
        calc_x: ComponentWrapper for IEcoCalculatorX interface.
    """
    print_info("Testing IEcoCalculatorX (Addition, Subtraction)")
    print()

    # Addition tests
    for a, b in [(10, 20), (100, 200), (-50, 100), (32767, 1)]:
        result = calc_x.Addition(a, b)
        print_result(f"{a:>6} + {b:<6}", result)

    print()

    # Subtraction tests
    for a, b in [(50, 30), (100, 200), (0, -100), (-50, -30)]:
        result = calc_x.Subtraction(a, b)
        print_result(f"{a:>6} - {b:<6}", result)


def demo_calculator_y(calc_y: ComponentWrapper) -> None:
    """Demonstrate IEcoCalculatorY operations (Multiplication, Division).

    Args:
        calc_y: ComponentWrapper for IEcoCalculatorY interface.
    """
    print_info("Testing IEcoCalculatorY (Multiplication, Division)")
    print()

    # Multiplication tests
    for a, b in [(6, 7), (100, 100), (-10, 5), (256, 256)]:
        result = calc_y.Multiplication(a, b)
        print_result(f"{a:>6} * {b:<6}", result)

    print()

    # Division tests
    for a, b in [(100, 10), (42, 7), (-100, 5), (1000, 3)]:
        result = calc_y.Division(a, b)
        print_result(f"{a:>6} / {b:<6}", result)


def main() -> int:
    """Main example function.

    Returns:
        Exit code (0 for success, 1 for failure).
    """

    print_header("Eco.Python2ACOM Example: Calculator Component")

    # Show interface information
    print_info(f"IEcoCalculatorX IID: {IEcoCalculatorX._iid_.to_string()}")
    print_info(f"IEcoCalculatorY IID: {IEcoCalculatorY._iid_.to_string()}")
    print_info(f"CID_EcoCalculator: {CID_EcoCalculator}")
    print()

    try:
        # Step 1: Find the DLL
        dll_path = Path(
            r"C:\Users\Sergei\Desktop\Diploma Files\eco_components\Eco.CalculatorC\BuildFiles\Windows\amd64\DynamicRelease\4828F6552E4540E78121EBD220DC360E.dll"
        )
        print_ok(f"Found DLL: {dll_path}")

        # Step 2: Initialize EcoSystem
        # EcoSystem creates:
        # - InterfaceBus for component registration and querying
        # - MemoryManager for dynamic memory allocation
        print_info("Initializing EcoSystem runtime...")

        with EcoSystem() as eco:
            print_ok("EcoSystem initialized")

            # Step 3: Load and register the calculator component
            # This:
            # - Loads the DLL
            # - Gets the component factory via GetIEcoComponentFactoryPtr()
            # - Registers the factory with InterfaceBus
            print_info(f"Loading component: {dll_path.name}")
            cid = UGUID.from_string(CID_EcoCalculator)
            eco.load_component(cid, dll_path)
            print_ok("Component loaded and registered")

            # Step 4: Query component instance with IEcoCalculatorX interface
            # This:
            # - Calls InterfaceBus.QueryComponent(CID, IID)
            # - Creates a new component instance
            # - Returns the requested interface
            print_info("Creating calculator instance (IEcoCalculatorX)...")
            calc_x = eco.query_component(cid, IEcoCalculatorX._iid_, IEcoCalculatorX)
            print_ok(f"Got IEcoCalculatorX: {calc_x}")

            # Step 5: Demo IEcoCalculatorX
            print_header("IEcoCalculatorX Operations")
            demo_calculator_x(calc_x)

            # Step 6: QueryInterface to get IEcoCalculatorY
            # This demonstrates that a single component can implement
            # multiple interfaces, and you can switch between them
            print_header("QueryInterface Demo")
            print_info("Querying IEcoCalculatorY from IEcoCalculatorX...")
            calc_y = calc_x.query_interface(IEcoCalculatorY)
            print_ok(f"Got IEcoCalculatorY: {calc_y}")

            # Step 7: Demo IEcoCalculatorY
            print_header("IEcoCalculatorY Operations")
            demo_calculator_y(calc_y)

            # Step 8: Release interfaces
            print_header("Cleanup")
            calc_y.release()
            print_ok("Released IEcoCalculatorY")

            calc_x.release()
            print_ok("Released IEcoCalculatorX")

        # EcoSystem context manager automatically calls release()
        print_ok("EcoSystem released")

        print_header("Test Completed Successfully!")
        return 0

    except FileNotFoundError as err:
        print_err(f"File not found: {err}")
        return 1

    except EcoError as err:
        print_err(f"ACOM Error: {err}")
        print_info("Make sure ECO_FRAMEWORK_RT is set correctly")
        return 1

    except Exception as err:
        print_err(f"Unexpected error: {err}")
        import traceback

        traceback.print_exc()
        return 1


if __name__ == "__main__":
    sys.exit(main())
