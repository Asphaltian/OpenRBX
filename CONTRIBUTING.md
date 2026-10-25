# Contributing

Pull requests are welcome!

Please keep them small. Reviewing decompiled code means reading the original assembly side by side with it, and that gets painful fast as the diff grows. One class per pull request is a good rule. Some classes are too tangled up with each other for that to work, but once you're past ten files or so it's too big to review properly.

Right now the only goal is accuracy to the original binary. A change is good if Visual C++ 8.0 SP1 compiles it into exactly what `WebService.dll` already contains, so please hold off on modernizing code or fixing bugs for now.

Decompiling takes a while to get good at. You should be comfortable with C++ and x86 assembly before you start.

## Overview

* [`WebService`](/WebService) - the translation units specific to RBXGS
* [`Client`](/Client) - the engine's static libraries
* [`util`](/util) - headers the decompilation needs
* [`3rdparty`](/3rdparty) - vendored dependencies

## Annotations

Every decompiled function gets a reccmp marker with its address in the original binary:

```cpp
// FUNCTION: WEBSERVICE 0x10001040
void CWebService::Something()
{
}
```

`FUNCTION:` means the function is a 100% match, so use `STUB:` until it is one. Globals use `GLOBAL:`, vtables use `VTABLE:` and compiler-generated code uses `SYNTHETIC:`.

* Write addresses with all eight hex digits in lowercase. CI checks this with `tools/reccmp_addr_padding.py`.
* Order the functions in a translation unit by ascending address.
* Don't forget the colon! Every marker needs one except `// SIZE`, and reccmp silently skips a marker without it.

Functions the linker merged into one share an address. Mark them `FOLDED`, they don't have to follow the address order:

```cpp
// FUNCTION: WEBSERVICE 0x100a77d0 FOLDED
```

`reccmp/webservice-function-size.csv` lists the address and size of all 12,633 functions. Check it if you want to know where a function ends.

## Code Style

### Formatting

Run [clang-format](https://clang.llvm.org/docs/ClangFormat.html) and [clang-tidy](https://clang.llvm.org/extra/clang-tidy/) with the configuration files in the repository root. Most editors can [do this for you](https://clang.llvm.org/docs/ClangFormat.html#vim-integration). CI uses clang-format 22.

### Naming

`WebService.pdb` was never stripped, so most names are already known. Take class names, member names, namespaces, struct layouts and enum values from it. `reccmp-cvdump -t` prints the type records with member offsets and enum values.

Only when the PDB really doesn't have a name, use the placeholders from the [LEGO Island decompilation](https://github.com/isledecomp/isle):

* Unknown functions: `FUN_XXXXXXXX` (eight hex digits, lowercase)
* Unknown global variables: `g_unk0xXXXXXXXX`
* Unknown member variables: `m_unk0xXX`
* Unknown parameters: `p_unk0xXX`

Please put each file at the path the PDB records for it, too.
