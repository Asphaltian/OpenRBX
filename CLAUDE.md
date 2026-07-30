# OpenRBX

Decompilation of Roblox as of 2007-12-20, using MSVC 8.0 SP1 (cl.exe 14.00.50727.762). Modeled after the [LEGO Racers decompilation](https://github.com/isledecomp/racers).

The client is in scope eventually. RBXGS, the game server, comes first because its binary ships an unstripped PDB, and the engine libraries under `common/` were shared with the client anyway: three of the six LTCG objects in `WebService.dll` are `Client\win` sources.

One target so far: **WEBSERVICE**, `WebService.dll`, an ATL Server ISAPI extension with 12,633 functions.

## Building

```
<path-to-msvc8>\Common7\Tools\vsvars32.bat
cmake -B build -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build
```

Portable MSVC 8.0 SP1: https://github.com/Asphaltian/MSVC800-SP1, assembled from VS2005 Professional and KB926601. Use `vsvars32.bat`, not `VC\bin\vcvars32.bat`, which forwards to it via `%VS80COMNTOOLS%` and needs a registered installation. RTM (`.42`) will not reproduce the original codegen. `CMakeLists.txt` gates all decomp-specific flags behind `MSVC_FOR_DECOMP`, true only for cl 14.x.

## Ground Truth

`WebService.pdb` ships alongside the binary and is not stripped. Check whether it already answers a question before working one out from the disassembly. It carries symbol names, namespaces, class hierarchies, source file paths, line numbers, per-object compiler command lines and the toolchain version.

Both files come out of the RBXGS installer on archive.org, extracted with [lessmsi](https://github.com/activescott/lessmsi) into `SourceDir/`. See the README for the URL and hashes. `.github/workflows/rbxgsbin.yml` does the same fetch for CI.

Loading the PDB into Ghidra is worth doing before writing anything. Reading the decompiler's pseudocode is usually faster than inferring structure from raw bytes, and it recovers control flow that a byte diff will not show you.

## Recovering Facts

Never guess a path, a name, an offset or an enum value. Each of these is recorded:

`reccmp-cvdump -m -s -l` gives modules, symbols and line info. The `src =` line in each module is the original directory and filename with its real casing, which is the layout to reproduce. Note that the LINES section lowercases every path it stores, so it cannot answer casing; only `src =` can, and only for `.cpp` files.

`reccmp-cvdump -t` writes the TYPES section. It is the authority for member names, offsets, class sizes and enum values. Ghidra's type browser resolves some names to the wrong namespace, so prefer the type records when they disagree.

The LINES range table maps an address to the file it came from. Inline and template code is attributed to the header that defines it, so this is how to find a class's real header and how to tell whether a function belongs in the `.cpp` or inline in the `.h`.

Jump tables settle enum values without guessing: read the byte index table and the jump table out of the image and the case-to-value mapping falls out.

Prove layouts rather than assuming them. `DECOMP_SIZE_ASSERT(T, size)` fails the build on a mismatch; assert a deliberately wrong size once to confirm the assert is live.

## Build Layout

Each original static library gets its own OBJECT library, because each was compiled with different flags. `CMakeLists.txt` carries the full set. The differences that bite:

- App, RbxView and AppDraw were built `/GS-`. Network and RenderLib were not. Stack cookies show up as spurious mismatches if this is wrong.
- App is the only library with `/fp:fast`, and the only one with `/Ob2 /Oi /Oy` together.
- RenderLib is the only one with `/Wp64`.
- Six translation units are the only objects compiled `/GL`, so the link ran `/LTCG`. Nothing else in the binary is LTCG. Three of the six are `Client\win` sources, not RBXGS ones.

Within a library, App uses subdirectories (`util`, `v8world`, `v8tree`, `v8kernel`, `v8datamodel`, `v8xml`, `gui`, `humanoid`, `reflection`, `script`, `security`, `tool`) with headers under `include/`. AppDraw, RbxView, RenderLib and the WebService project are flat, and each library's headers live under `include/<libname>/`.

`util/decomp.cpp` exists so the DLL target has a source file while everything else is empty. Keep it out of the LTCG list; it is ours rather than theirs.

`OPENRBX_KEEP_UNREFERENCED` links with `/OPT:NOREF` so reccmp can see functions nothing calls yet. The original used `/OPT:REF`. Turn it off once enough of the DLL is reachable.

## Third Party

Everything is vendored under `3rdparty/`. RakNet is the exception that is reconstructed rather than vendored, because the 3.0 release RBXGS used was never archived; the closest archived drop still differs in nine files.

Two dependencies are easy to miss because they were compiled into someone else's `.lib` instead of being linked. Lua 5.1.1 went into app.lib as 25 translation units, and zlib, libpng and IJG jpeg arrive inside G3D as `g3d/zlib`, `g3d/png` and `g3d/ijg`.

## MSVC 8.0 Codegen Patterns

Each of these cost a round trip. Keep the sources clean and leave the reasoning here.

A `const float` at namespace scope is emitted as a real global and referenced by name, where the binary reads an anonymous constant out of the float pool. Write the literal at each use.

A dense switch lowers to descending `sub eax,N / je`. An if/else chain emits repeated `cmp` against each value. They are not interchangeable.

Locals are pushed in declaration order, so getting that order wrong leaves an extra value on the x87 stack and shifts every later `st(n)` operand. It reads as a large diff for a one-line cause.

Building a return value through a constructor evaluates every argument before storing anything. Assigning members one at a time interleaves the arithmetic with the stores. Chained assignment (`a = b = c = value`) keeps the value in a register across the stores and writes them right to left; separate statements reload it each time.

How you spell the return path decides register assignment. `if (cond) return x; return y;` makes `x` the fall-through value; assigning to one named result and returning it keeps `y` in place.

The binary does not store a vftable in the constructor of an abstract class, and ours does. Do not delete the virtuals to force the number.

A virtual destructor with no standalone function in the PDB was inlined everywhere. Define it without an annotation.

boost's `shared_ptr::operator<` compares ownership, not the pointer, so it loads offset 4 rather than 0.

Calls into vendored libraries read low: reccmp cannot name the original's call targets in G3D, Boost or the CRT, so every such call counts as a difference even when the streams are identical. `LIBRARY:` is the wrong fix, since that is for code we never build and G3D is in our binary.

## Gotchas

The original link used `/OPT:ICF` and folded heavily. Many distinct source functions compiled to the same three-byte body and share one address, which is also why Ghidra's PDB source line importer throws several hundred `IllegalArgumentException` warnings when the symbols load. Those warnings are expected and harmless. Run `tools/check_folded.py` before concluding that a function at a shared address is genuinely unmatched.

`tools/` holds only what came from racers. Analysis scripts written along the way live in the session scratchpad.
