# Contributing

## General Guidelines

Pull requests are welcome.

Keep them small. Reviewing decompiled code means reading the original assembly next to it, and that does not scale with the size of the diff. One class per pull request is the working rule. Some classes are entangled enough that this cannot hold, but past roughly ten files a submission is too large to review properly.

Accuracy to the original binary is the only current goal. The work is instruction matching, so the only question about a change is whether Visual C++ 8.0 SP1 emits what `WebService.dll` already contains. Modernisations and bug fixes are out of scope until that changes.

Decompilation takes time to become good at. Be comfortable with C++ and x86 assembly before starting.

## Overview

* [`WebService`](/WebService): Decompilation of the RBXGS-specific translation units.
* [`common`](/common): The engine static libraries the original link consumed, one directory per `.lib`, since each was built with its own flags. Those flags are set per target in [CMakeLists.txt](/CMakeLists.txt); one wrong flag produces mismatches that have nothing to do with the code in front of you.
* [`util`](/util): Utility headers supporting the decompilation.
* [`3rdparty`](/3rdparty): Vendored dependencies, apart from `RakNet30`. The 3.0 release RBXGS used was never archived, so it is recovered from the binary and annotated like the rest of the decompilation.

## File Layout

Files belong where the original kept them, and the PDB records where that was. `reccmp-cvdump -m -s -l WebService.pdb` prints a `src =` line for every module giving the original directory and filename, casing included.

## Annotations

Decompiled functions carry a reccmp marker giving their address in the original binary:

```cpp
// FUNCTION: WEBSERVICE 0x10001040
void CWebService::Something()
{
}
```

`FUNCTION:` claims a 100% match, so use `STUB:` until it is one. Globals use `GLOBAL:`, vtables `VTABLE:`, compiler-generated code `SYNTHETIC:`. Write addresses with all eight hex digits in lowercase, which `tools/reccmp_addr_padding.py` enforces in CI, and order functions within a translation unit by ascending address.

The colon is required on all of them except `// SIZE`. Leave it out and reccmp ignores the annotation silently, which on a `VTABLE` costs several percent on every constructor that stores the vtable.

Functions the linker merged share one address and are marked `FOLDED`, as in `// FUNCTION: WEBSERVICE 0x100a77d0 FOLDED`. They are exempt from ascending order.

`reccmp/webservice-function-size.csv` carries the address and size of all 12,633 functions, taken from the PDB. It is the quickest way to confirm that a function ends where you think it does.

## Code Style

### Formatting

[clang-format](https://clang.llvm.org/docs/ClangFormat.html) and [clang-tidy](https://clang.llvm.org/extra/clang-tidy/) run from the configuration files in the repository root, whose settings were measured against Roblox's own sources. Most editors have [integrations](https://clang.llvm.org/docs/ClangFormat.html#vim-integration). CI runs clang-format 22.

### Naming conventions

Placeholder names follow the [LEGO Island decompilation](https://github.com/isledecomp/isle):
- Unknown functions: `FUN_XXXXXXXX` (8 hex digits, lowercase)
- Unknown global variables: `g_unk0xXXXXXXXX`
- Unknown member variables: `m_unk0xXX`
- Unknown parameters: `p_unk0xXX`

They are for the cases where the original name is genuinely lost. It usually is not: the PDB was never stripped, so most of this codebase already has real names, namespaces and class hierarchies. Struct layouts and enum values are recorded too, and `reccmp-cvdump -t` prints the type records that give member offsets and enumerator values outright.
