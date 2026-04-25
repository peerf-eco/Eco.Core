# Java2ACOM Bridge
## Usage (Windows)
- Build `Java2ACOM.dll`
  - Go to `cd AssemblyFiles/Windows`
  - Change `MICROSOFT_VISUAL_STUDIO_PATH` and `MICROSOFT_SDK_PATH` in `build.bat` if needed
  - Install `libffi`
    - Download [libffi-3.5.2-x86-32bit-msvc-binaries.zip](https://github.com/libffi/libffi/releases/download/v3.5.2/libffi-3.5.2-x86-32bit-msvc-binaries.zip)
    - Extract `ffi.h` and `ffitarget.h` into `%MICROSOFT_VISUAL_STUDIO_PATH%\VC\include`
    - Extract `libffi-8.lib` into `%MICROSOFT_VISUAL_STUDIO_PATH%\VC\lib`
    - Extract `libffi-8.dll` into working directory
  - Run `build.bat`
- Build and run unit test
  - Open project in **IntelliJ IDEA**
  - Build - Build Artifacts - Eco.Java2ACOM.jar
  - Add configuration `Application`
    - Module: `UnitTestFiles`
    - Main class: `Eco.System.EcoMain`
    - Add VM option: `-Djava.library.path=BuildFiles`
    - Working directory: `path/to/Eco.Java2ACOM`
  - Run configuration
