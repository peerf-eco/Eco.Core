# Java2ACOM Bridge
## Usage (Windows)
- Build `Java2ACOM.dll`
  - Go to `cd AssemblyFiles/Windows`
  - Change `MICROSOFT_VISUAL_STUDIO_PATH` and `MICROSOFT_SDK_PATH` in `build.bat` if needed
  - Run `build.bat`
- Build and run unit test
  - Open project in **IntelliJ IDEA**
  - Add configuration `Application`
    - Main class: `Eco.System.EcoMain`
    - Add VM option: `-Djava.library.path=BuildFiles`
    - Working directory: `path/to/Eco.Java2ACOM`
  - Run configuration
