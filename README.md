# OpenRBX

A decompilation of Roblox as it shipped in December 2007. The aim is to rebuild the original binaries instruction for instruction, and to end up with a tree worth building on afterwards.

RBXGS, the game server, comes first, because its `WebService.dll` shipped with an unstripped PDB. The client follows. Most of the work carries across either way: the engine libraries under `common/` were shared between the two, and three of the six link-time code generation objects inside `WebService.dll` came from the client tree, not the server's.

Conventions and tooling follow the [LEGO Island decompilation](https://github.com/isledecomp/isle).

> **Note:** The code here reproduces the original release and does not try to improve on it. 32-bit Windows only.

## Status

<p align="center"><a href="https://asphaltian.github.io/OpenRBX/WEBSERVICEPROGRESS.HTML"><img src="https://asphaltian.github.io/OpenRBX/WEBSERVICEPROGRESS.SVG" width="50%"></a></p>

Still early. `WebService.dll` contains 12,633 functions.

## Building

Visual C++ 8.0 SP1 compiled the original and is the only toolchain that reproduces its output, so contributions are measured against it.

#### Prerequisites

- Visual C++ 8.0 SP1. [MSVC800-SP1](https://github.com/Asphaltian/MSVC800-SP1) is a portable copy that requires no installation. The revision matters: the original was built with `.762`, while RTM `.42` generates different code.
- [CMake](https://cmake.org/), either standalone or from the "Desktop development with C++" workload of a current Visual Studio.

#### Compiling

Open a command prompt and run `Common7\Tools\vsvars32.bat` from the toolchain to set the environment. `VC\bin\vcvars32.bat` will not do, as it defers to `%VS80COMNTOOLS%` and expects a registered installation.

Then configure and build:

```
cmake -B build -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build
```

`RelWithDebInfo` keeps the debug symbols that further decompilation work depends on, and NMake Makefiles is the generator that copes with a toolchain of this age. The result is `build\WebService.dll`.

### Verification

To compare a build against the original, install [reccmp](https://github.com/isledecomp/reccmp):

```
pip install -r tools/requirements.txt
```

Point it at your copy of the original through `reccmp-user.yml` in the project root:
```yaml
targets:
  WEBSERVICE:
    path: path/to/WebService.dll
```

Then:
```
reccmp-reccmp --target WEBSERVICE -S WEBSERVICEPROGRESS.SVG
```

## Project Structure

- `WebService/` - The RBXGS-specific translation units
- `common/` - Engine static libraries, one directory per `.lib` the original link consumed
- `3rdparty/` - Vendored dependencies
- `util/` - Headers supporting the decompilation itself
- `cmake/` - CMake modules
- `tools/` - Python tooling and its requirements
- `reccmp/` - Data sources for reccmp
- `assets/` - Icons for the progress report

## Target Binary

| Binary | Size | SHA256 |
|--------|------|--------|
| WebService.dll | 3,461,120 bytes | `5e9c6b82605fc8429c5fc5f682f2d7f98b55b73d993dfba58437365754410e2f` |

Version 0.3.676.0, dated December 20, 2007.

## Contributing

Contributions are welcome. The conventions in short:
- Annotate decompiled code with reccmp markers (`FUNCTION:`, `STUB:`, `GLOBAL:`)
- Order functions within a translation unit by ascending address
- Place each file at the path `WebService.pdb` records for it
- Follow the clang-format configuration
- Take names from the PDB where it has them, and use NCC placeholders (`FUN_XXXXXXXX`, `g_unk0xXXXXXXXX`) only where it does not
- Keep pull requests small and focused

[CONTRIBUTING.md](CONTRIBUTING.md) covers each of these properly.
