<div align="center">
  <img src="https://github.com/user-attachments/assets/e9886f32-5ba3-444b-804e-1de4a37eb1b6" width="80px" alt="OpenRBX logo">

  [Discord] | [Contributing]
</div>

[Discord]: https://discord.gg/2JcBQFxVPv
[Contributing]: CONTRIBUTING.md

# OpenRBX

OpenRBX is a source port of Roblox as it was released in December 2007. It is built from the code recovered by [RBXDecomp](https://github.com/Asphaltian/RBXDecomp), and the goal is a modern, cross-platform codebase.

## Getting OpenRBX

### Compiling from source

For now OpenRBX still builds only for 32-bit Windows with the compiler the original used. This will change as the port progresses.

| Platform | Toolchain | Notes |
|----------|-----------|-------|
| Windows (x86) | Visual C++ 8.0 SP1 | Builds the client and the RBXGS web service. |

#### Prerequisites

* [Git](https://git-scm.com/downloads)
* Visual C++ 8.0 SP1. [MSVC800-SP1](https://github.com/Asphaltian/MSVC800-SP1) is a portable copy that doesn't need installing.
* [CMake](https://cmake.org/)

#### Building

Open a command prompt and run `Common7\Tools\vsvars32.bat` from the toolchain folder, then:

```bat
:: Clone the repo
git clone https://github.com/Asphaltian/OpenRBX.git
cd OpenRBX

:: Configure and build
cmake -B build -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build
```

Once the build completes, the client (`Roblox.exe`) and the RBXGS web service (`WebService.dll`) are in the `build` folder.

## Contributing

If you would like to contribute, please see the [contributing guide](CONTRIBUTING.md).

If you want to report bugs or request new features, see [issues](https://github.com/Asphaltian/OpenRBX/issues).

## License

The OpenRBX source code is licensed under the [GNU General Public License v3.0](LICENSE).

This project includes third-party components that are separately licensed.
Those components are not covered by the license above and remain subject to their original licenses.
