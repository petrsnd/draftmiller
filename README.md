<<<<<<< HEAD
# draftmiller
C++ implementation of draft-miller-ssh-agent-00

## Build

This project uses a CMake build system.  Helper scripts are provided to bootstrap your project
files for IDEs that don't natively support CMake.

### macOs / Linux

On macOs and Linux, we use a Unix Makefiles build system.  A Bash script has been provided to
configure the build system via CMake, compile the code via make, and execute the unit tests.

```Bash
$ ./build_proxy.sh -f
```

### Windows

On Windows, we use a Visual Studio build system.  A PowerShell script has been provided to create
the necessary solution and project files via CMake.  Once the projects are generated you may use
Visual Studio as your IDE.  The only difference in your normal workflow will be that you need to
modify the appropriate CMakeLists.txt file rather than the generated project file to add new
classes or code files to the project.

```Powershell
> .\build_proxy.ps1
```
