"""`Eco.NewProject` pattern example — client host.

The host:
    1. Boots `EcoSystem` and reaches the Python bridge through the bus.
    2. Registers `Eco.NewProject` (server-side template) with the bridge.
    3. Queries `INewProject`.
    4. Calls `MyFunction`.
    5. Releases interface pointer.
"""

import sys
from pathlib import Path

from eco_python2acom.guids.iid import IID_IEcoACOM2Python
from eco_python2acom.interfaces.acom2python import IEcoACOM2Python
from eco_python2acom.runtime.system import EcoSystem
from eco_python2acom.types.core import CString, Void
from eco_python2acom.types.pointer import Ptr
from eco_python2acom.types.utils import byref, cast
from examples.client.console import console
from examples.server.pattern.interfaces import IID_INewProject, INewProject
from examples.server.pattern.project import CID_EcoNewProject

SERVER_PATH = str(
    Path(__file__).resolve().parent.parent.parent / "server" / "pattern" / "project.py"
)


def main() -> int:
    """Run the `Eco.NewProject` pattern example."""
    console.header("Eco NewProject Example")
    console.info(f"CID:         {CID_EcoNewProject}")
    console.info(f"INewProject: {IID_INewProject}\n")

    try:
        console.info("Initializing EcoSystem...")

        with EcoSystem(lib_dir="data") as eco:
            console.success("EcoSystem initialized\n")

            # --------------------------- Bridge ---------------------------
            bridge_ptr = Ptr[Void]()
            result = eco.bus.obj.QueryInterface(byref(IID_IEcoACOM2Python), byref(bridge_ptr))
            if result != 0 or not bool(bridge_ptr):
                console.error(f"Failed to get bridge (code = {result})")
                return -2
            bridge = cast(bridge_ptr, Ptr[IEcoACOM2Python])
            console.success(f"Got {bridge}")

            result = bridge.obj.RegisterComponent(SERVER_PATH.encode(), byref(CID_EcoNewProject))
            if result != 0:
                console.error(f"Failed to register component (code = {result})")
                bridge.obj.Release()
                return -3

            # ------------------------- Component --------------------------
            project_ptr = Ptr[Void]()
            result = bridge.obj.QueryComponent(
                byref(CID_EcoNewProject), None, byref(IID_INewProject), byref(project_ptr)
            )
            if result != 0 or not bool(project_ptr):
                console.error(f"Failed to query component (code = {result})")
                bridge.obj.Release()
                return -4
            project = cast(project_ptr, Ptr[INewProject])
            console.success(f"Got {project}\n")

            # -------------------------- MyFunction ------------------------
            source = b"Hello, world!"
            copy = CString()
            result = project.obj.MyFunction(source, byref(copy))
            if result != 0:
                console.error(f"MyFunction failed (code = {result})")
                project.obj.Release()
                bridge.obj.UnRegisterComponent(byref(CID_EcoNewProject))
                bridge.obj.Release()
                return -5

            console.info(f"Input:  {source.decode()}")
            console.success(f"Copied: {copy.value.decode()}\n")

            # -------------------------- Cleanup ---------------------------
            project.obj.Release()
            console.success(f"Released {project}")
            bridge.obj.UnRegisterComponent(byref(CID_EcoNewProject))
            bridge.obj.Release()

        console.info("Released EcoSystem")
        return 0

    except Exception as err:
        console.error(str(err))
        return -1


if __name__ == "__main__":
    sys.exit(main())
