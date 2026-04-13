"""Calculator example with interfaces (IEcoCalculatorX + IEcoCalculatorY)."""

import sys

from eco_python2acom.runtime.system import EcoSystem
from eco_python2acom.types.core import Void
from eco_python2acom.types.pointer import Ptr
from eco_python2acom.types.utils import byref
from examples.calculator.interfaces import (
    CID_EcoCalculator,
    IEcoCalculatorX,
    IEcoCalculatorY,
    IID_IEcoCalculatorX,
    IID_IEcoCalculatorY,
)
from examples.calculator.utils import get_operation_table
from examples.console import console, print_error, print_header, print_info, print_success


def main() -> int:
    """Run calculator example."""
    print_header("Calculator Example")
    print_info(f"CID: {CID_EcoCalculator}")
    print_info(f"IEcoCalculatorX IID: {IID_IEcoCalculatorX}")
    print_info(f"IEcoCalculatorY IID: {IID_IEcoCalculatorY}")
    console.print()

    try:
        print_info("Initializing EcoSystem...")

        with EcoSystem(user_lib_dir="data") as eco:
            print_success("EcoSystem initialized")
            console.print()

            ppv_x = Ptr[Void]()
            result = eco.bus.QueryComponent(
                byref(CID_EcoCalculator), None, byref(IEcoCalculatorX._iid_), byref(ppv_x)
            )
            if result.value != 0 or not ppv_x.value:
                print_error(f"Failed to query component (code = {result.value})")
                return -2
            calc_x = IEcoCalculatorX(ppv_x)
            print_success(f"Got {calc_x}")
            console.print(
                get_operation_table(
                    "Addition", "+", [(10, 20), (100, 200), (-50, 100)], calc_x.Addition
                )
            )
            console.print(
                get_operation_table(
                    "Subtraction", "-", [(50, 30), (100, 200), (-50, -30)], calc_x.Subtraction
                )
            )
            console.print()

            ppv_y = Ptr[Void]()
            result = calc_x.QueryInterface(byref(IEcoCalculatorY._iid_), byref(ppv_y))
            if result.value != 0 or not ppv_y.value:
                print_error(f"Failed to query interface (code={result.value})")
                calc_x.Release()
                return -3
            calc_y = IEcoCalculatorY(ppv_y)
            print_success(f"Got {calc_y}")
            console.print(
                get_operation_table(
                    "Multiplication", "*", [(6, 7), (100, 100), (-10, 5)], calc_y.Multiplication
                )
            )
            console.print(
                get_operation_table(
                    "Division", "/", [(100, 10), (42, 7), (-100, 5)], calc_y.Division
                )
            )
            console.print()

            calc_y.Release()
            print_success(f"Released {calc_y}")
            calc_x.Release()
            print_success(f"Released {calc_x}")

        print_info("Released EcoSystem")
        return 0

    except Exception as err:
        print_error(str(err))
        return -1


if __name__ == "__main__":
    sys.exit(main())
