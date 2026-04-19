"""Calculator example with interfaces (IEcoCalculatorX + IEcoCalculatorY)."""

from __future__ import annotations

import sys

from eco_python2acom.runtime.system import EcoSystem
from eco_python2acom.types.core import Void
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr
from eco_python2acom.types.utils import byref
from examples.calculator.interfaces import IEcoCalculatorX, IEcoCalculatorY
from examples.calculator.utils import get_operation_table
from examples.console import console, print_error, print_header, print_info, print_success

# CID_EcoCalculator = UGUID.from_string("AE202E54-3CE5-4550-8996-03BD70C62565")  # Calculator B
# CID_EcoCalculator = UGUID.from_string("4828F655-2E45-40E7-8121-EBD220DC360E")  # Calculator C
# CID_EcoCalculator = UGUID.from_string("3A8E4467-7E82-475C-B4A3-719ED8397E61")  # Calculator D
CID_EcoCalculator = UGUID.from_string("872FEF1D-E331-4B87-AD44-D1E7C232C2F0")  # Calculator E


def main() -> int:
    """Run calculator example."""
    print_header("Calculator Example")
    print_info(f"CID: {CID_EcoCalculator.to_string()}")
    print_info(f"IEcoCalculatorX IID: {IEcoCalculatorX._iid_.to_string()}")
    print_info(f"IEcoCalculatorY IID: {IEcoCalculatorY._iid_.to_string()}")
    console.print()

    try:
        print_info("Initializing EcoSystem...")

        with EcoSystem(user_dll_path="data") as eco:
            print_success("EcoSystem initialized")
            console.print()

            ppv_x = Ptr[Void]()
            result = eco.bus.QueryComponent(
                byref(CID_EcoCalculator), None, byref(IEcoCalculatorX._iid_), byref(ppv_x)
            )
            if result.value != 0 or not ppv_x.value:
                print_error(f"QueryComponent(IEcoCalculatorX) failed (code={result.value})")
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
                print_error(f"QueryInterface(IEcoCalculatorY) failed (code={result.value})")
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
