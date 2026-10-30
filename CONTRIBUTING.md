# Contributing

Pull requests are welcome!

Please keep them small. Reviewing decompiled code means reading the original assembly side by side with it, and that gets painful fast as the diff grows. One class per pull request is a good rule. Some classes are too tangled up with each other for that to work, but once you're past ten files or so it's too big to review properly.

Right now the only goal is accuracy to the original binaries. A change is good if Visual C++ 8.0 SP1 compiles it into exactly what the original `WebService.dll` or `Roblox.exe` contains, so please hold off on modernizing code or fixing bugs for now.

Decompiling takes a while to get good at. You should be comfortable with C++ and x86 assembly before you start.

## Overview

* [`WebService`](/WebService) - the translation units specific to RBXGS
* [`Client`](/Client) - the engine's static libraries, shared by RBXGS and the client, plus the libraries Roblox kept in its own tree
* [`util`](/util) - headers the decompilation needs
* [`3rdparty`](/3rdparty) - the SDKs the original built against from outside its tree

## Annotations

Every decompiled function gets a reccmp marker with its address in the original binary:

```cpp
// FUNCTION: WEBSERVICE 0x10001040
void CWebService::Something()
{
}
```

`FUNCTION:` means the function is a 100% match, so use `STUB:` until it is one. Globals use `GLOBAL:`, vtables use `VTABLE:` and compiler-generated code uses `SYNTHETIC:`.

A function that is in the client too gets a second marker with its address in `Roblox.exe`. Code that only the client has gets the `ROBLOX` marker on its own:

```cpp
// FUNCTION: WEBSERVICE 0x100d1180
// FUNCTION: ROBLOX 0x0058cde0
void RelativePanel::init(const Layout& layout)
{
}
```

The two binaries are scored separately, so a function can be `FUNCTION:` in one and `STUB:` in the other.

* Write addresses with all eight hex digits in lowercase. CI checks this with `tools/reccmp_addr_padding.py`.
* Order the functions in a translation unit by ascending address.
* Don't forget the colon! Every marker needs one except `// SIZE`, and reccmp silently skips a marker without it.

Functions the linker merged into one share an address. Mark them `FOLDED`, they don't have to follow the address order:

```cpp
// FUNCTION: WEBSERVICE 0x100a77d0 FOLDED
```

`reccmp/webservice-function-size.csv` lists the address and size of all 12,633 functions in `WebService.dll`, and `reccmp/roblox-function-size.csv` does the same for the client. Check them if you want to know where a function ends.

## Code Style

### Formatting

Run [clang-format](https://clang.llvm.org/docs/ClangFormat.html) and [clang-tidy](https://clang.llvm.org/extra/clang-tidy/) with the configuration files in the repository root. Most editors can [do this for you](https://clang.llvm.org/docs/ClangFormat.html#vim-integration). CI uses clang-format 22.

### Naming

`WebService.pdb` was never stripped, so most names are already known. Take class names, member names, namespaces, struct layouts and enum values from it. `reccmp-cvdump -t` prints the type records with member offsets and enum values.

The client has no PDB of its own, but a lot of its code is still described in the RBXGS one. Keep in mind that a class can be in the type records even when RBXGS never used it, so look there first. The client's RTTI names its other classes.

Only when there really is no name, use the placeholders from the [LEGO Island decompilation](https://github.com/isledecomp/isle):

* Unknown functions: `FUN_XXXXXXXX` (eight hex digits, lowercase)
* Unknown global variables: `g_unk0xXXXXXXXX`
* Unknown member variables: `m_unk0xXX`
* Unknown parameters: `p_unk0xXX`

Please put each file at the path the PDB records for it too.
