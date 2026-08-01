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

Prove layouts; do not assume them. `DECOMP_SIZE_ASSERT(T, size)` fails the build on a mismatch; assert a deliberately wrong size once to confirm the assert is live.

## Annotations

Functions in a compilation unit are ordered by ascending address.

```cpp
// FUNCTION: WEBSERVICE 0x100a77d0         complete, compared by reccmp
// FUNCTION: WEBSERVICE 0x100a77d0 FOLDED  identical code the linker merged
// STUB: WEBSERVICE 0x100a7800             incomplete, skipped by reccmp
// LIBRARY: WEBSERVICE 0x10001000          CRT or third party, in library_msvc.h inside #ifdef 0
// SYNTHETIC: WEBSERVICE 0x10007040        compiler-generated, such as scalar deleting destructors
// GLOBAL: WEBSERVICE 0x102c7000           global variable
// VTABLE: WEBSERVICE 0x10056440           virtual function table
// SIZE 0xf8                               class size assertion
```

The colon is required on everything except `// SIZE`. reccmp silently ignores `// VTABLE WEBSERVICE 0x...`, and the vtable stores inside constructors then compare as `<OFFSET>` instead of the vftable symbol, costing five to ten percent.

`// FUNCTION:` asserts a 100% match. Anything less is `// STUB:`.

`FOLDED` marks siblings the linker merged onto one address. They share that address, are exempt from ascending order, and do not need the `STUB()` macro.

## Class Pattern

```cpp
// VTABLE: WEBSERVICE 0x10056440
// SIZE 0xf8
class Instance : public GuidItem<Instance> {
public:
	virtual ~Instance();      // vtable+0x00
	virtual void VTable0x04();

private:
	int worldIndex;                       // 0x28
	undefined m_unk0x2c[0x40 - 0x2c];     // 0x2c
};
```

`DECOMP_SIZE_ASSERT(Instance, 0xf8)` goes in the source. Member offset comments and `vtable+0xNN` comments are required. Size gaps use a subtraction so the length documents itself.

## Types and Names

`undefined`, `undefined2` and `undefined4` from `decomp.h` stand in for unproven types. Do not guess `int` or `float` until a 100% match proves it.

Unlike racers, the PDB is not stripped, so real names are usually available and placeholders are the exception. Where nothing is recorded, NCC rules apply: `FUN_XXXXXXXX`, `g_unk0xXXXXXXXX`, `m_unk0xXX`, `p_unk0xXX`, `VTable0xXX`, `c_` on enum constants. Rename a placeholder only when the evidence is unambiguous; competing plausible names mean keep it, because a misleading name is worse than a neutral one.

## Code Style

- Bit tests read `if (flags & c_flag)`, never `!= 0`.
- No redundant `this->`.
- Win32 calls use the un-suffixed macro (`CreateWindowEx`, not `CreateWindowExA`), which is what the source had even though the import is the `A` symbol.
- `NULL` for pointers, `TRUE`/`FALSE` for booleans, plain `0` only for scalars.
- No leading `const` on return-by-value; NCC rejects it.
- clang-format the files you touch. Reflowing does not change codegen.

## Matching a Function

1. Read the decompilation and check the byte budget. The gap to the next address bounds the body, so a body that cannot fit means this is a wrapper and the logic is in a callee.
2. Take calling conventions from the definition, not a call site. `__thiscall` means a member.
3. A `__thiscall` on a global means that global is an instance. Declare the class with `undefined m_unk0x00[size]`.
4. STUB every unknown callee, in ascending address order, with `STUB(0xADDRESS)` in the body so `/OPT:ICF` cannot fold them together. Empty destructors are exempt.
5. A stub is one store, and App builds `/Ob2`, so the caller inlines it and loses whatever stack the real call needed. A caller whose stub callee takes arguments cannot reach 100% until that callee is matched for real.
6. Write clean C++, not decompiler pseudocode. No `*(int*)(this + 4)`, no gotos, no raw float bit patterns.
7. Build, then `reccmp-reccmp --target WEBSERVICE --nolib --verbose 0xADDRESS`, and iterate to 100%.
8. Compare the vtable address itself, not only the functions. It is what exposes a missing or misordered virtual.
9. Re-verify functions that touch any class you changed, then run `reccmp-decomplint` and `tools/check_folded.py`.

Starting a new class, match the constructor, destructor and scalar deleting destructor before any method. Those three prove size, base chain, vtable layout and member order.

## Decompilation Principles

- A type is proven when a `// FUNCTION:` using it reaches 100%, and not before.
- A diff means the code is wrong. Find the cause: layout, types, a missing base. Register allocation is never the explanation.
- Claimed matches are not verified matches. Re-measure anything inherited or generated.
- Every annotation carries a real address. No placeholders.
- Casts plus pointer arithmetic mean the types are wrong. Find the real class.
- One root type per header. Nest one-owner helper types inside their owner.
- An unexplained address gap means a function sits in the wrong class, or one file mixes classes. Move whole classes; never split a class's methods across files to tidy address order.

## Build Layout

Each original static library gets its own OBJECT library, because each was compiled with different flags. `CMakeLists.txt` carries the full set. The differences that bite:

- App, RbxView and AppDraw were built `/GS-`. Network and RenderLib were not. Stack cookies show up as spurious mismatches if this is wrong.
- App is the only library with `/fp:fast`, and the only one with `/Ob2 /Oi /Oy` together.
- App was built `/Gy`. The original folded `Primitive::getFirstJoint` onto `Joint::getJointOwner` across two objects, which only COMDATs allow.
- RenderLib is the only one with `/Wp64`.
- Six translation units are the only objects compiled `/GL`, so the link ran `/LTCG`. Nothing else in the binary is LTCG. Three of the six are `Client\win` sources, not RBXGS ones.

Within a library, App uses subdirectories (`util`, `v8world`, `v8tree`, `v8kernel`, `v8datamodel`, `v8xml`, `gui`, `humanoid`, `reflection`, `script`, `security`, `tool`) with headers under `include/`. AppDraw, RbxView, RenderLib and the WebService project are flat, and each library's headers live under `include/<libname>/`.

`util/decomp.cpp` exists so the DLL target has a source file while everything else is empty. Keep it out of the LTCG list; it is ours, not theirs.

`OPENRBX_KEEP_UNREFERENCED` links with `/OPT:NOREF` so reccmp can see functions nothing calls yet. The original used `/OPT:REF`. Turn it off once enough of the DLL is reachable.

## Third Party

Everything is vendored under `3rdparty/`. RakNet is the exception that is reconstructed rather than vendored, because the 3.0 release RBXGS used was never archived; the closest archived drop still differs in nine files.

Two dependencies are easy to miss because they were compiled into someone else's `.lib` instead of being linked. Lua 5.1.1 went into app.lib as 25 translation units, and zlib, libpng and IJG jpeg arrive inside G3D as `g3d/zlib`, `g3d/png` and `g3d/ijg`.

## MSVC 8.0 Codegen Patterns

Each of these cost a round trip. Keep the sources clean and leave the reasoning here.

A `const float` at namespace scope is emitted as a real global and referenced by name, where the binary reads an anonymous constant out of the float pool. Write the literal at each use.

A dense switch lowers to descending `sub eax,N / je`. An if/else chain emits repeated `cmp` against each value. They are not interchangeable. Ghidra prints both as `if` chains when the cases are sparse, so the bytes are the only witness: `getJointKMultiplier` has six nested dispatches and every one of them is a switch, which took it from 43 to 90 percent.

Locals are pushed in declaration order, so getting that order wrong leaves an extra value on the x87 stack and shifts every later `st(n)` operand. It reads as a large diff for a one-line cause.

Building a return value through a constructor evaluates every argument before storing anything. Assigning members one at a time interleaves the arithmetic with the stores. Chained assignment (`a = b = c = value`) keeps the value in a register across the stores and writes them right to left; separate statements reload it each time.

How you spell the return path decides register assignment. `if (cond) return x; return y;` makes `x` the fall-through value; assigning to one named result and returning it keeps `y` in place.

Abstract bases carry `__declspec(novtable)`. Without it the constructor stores a vftable the binary never stores, and any derived destructor gains a base vftable store plus a whole SEH frame to unwind the base through, which reads as a fifty percent diff. `Edge` and `IPipelined` both need it. Do not delete the virtuals to force the number.

A virtual destructor with no standalone function in the PDB was inlined everywhere. Define it inline in the header, without an annotation. Out of line in the `.cpp` it cannot inline across translation units and every derived destructor calls it.

boost's `shared_ptr::operator<` compares ownership, not the pointer, so it loads offset 4, not 0.

A `ComputeProp` member records its owner's inheritance shape. MSVC sizes the pointer-to-member by how the class inherits, so `ComputeProp<float, Primitive>` is 16 bytes and `ComputeProp<float, Assembly>` is 24: `Assembly` also derives from `boost::noncopyable`, which makes the member pointer the multiple-inheritance form. A class size that is off by the difference means a base is missing.

Assigning a call's result to a local before comparing it changes the evaluation order. `f() != g()` runs `g()` first, per the right-operand rule; `x = f(); return x != g();` runs `f()` first. `Ball::hitTest` only matched with the call written inside the comparison.

Comparing two `bool`s is a byte compare. Writing one of them inline as a negation, `flag != !other`, promotes both to int and costs a `movzx`. Store the negation in its own `bool` first.

A recursive helper and the loop it compiles to are not interchangeable in the source. MSVC turns tail recursion into a loop at the definition, so both spellings match there, but a caller inlines exactly one level of the recursion and none of the loop. `onPrimitivesChanged` and `findNextRelative` both had to be written recursively before `setParent` and `EdgeIterator::begin` would match.

`x = a; if (cond) x = b;` and `x = cond ? b : a;` are not the same codegen. The if form folds into one load and a conditional reload; the ternary evaluates `a` into a scratch register first and copies it in the else branch, which is one instruction longer. Roblox wrote the ternary for the "pick the other primitive on this edge" idiom, so `heavyParent`, `findParent` and `findFirstChild` only reach 100% spelled that way.

Read a CRT call's name out of the publics, never infer it from what the surrounding code looks like it should do. `_floor` at 0x101e8ac4 spent a long stretch written as `sqrt` because a size clamp reads like one, and the wrong one still matched three quarters of the body.

A `__int64` multiply lowers to `_allmul`, and the conversion back to float tells you the signedness: a plain `fild` is signed, and a pair of `fild`s that splits the sign bit out and subtracts it is unsigned.

A comparison puts its right operand in the register: `a == b` on integers is `mov eax, b` then `cmp [a], eax`, and on floats `fld b` then `fcomp a`. So the operand the binary loads first is the one written second, which settles whether a member `operator==` was called as `x == y` or `y == x`.

Calls into vendored libraries used to read low, because reccmp only parses the recompiled PDB and had no name for the original's side. `reccmp/webservice-vendored.csv` hands it the 2,594 G3D, Boost, STL and ATL functions out of `WebService.pdb`, which reccmp then matches by mangled name; `--nolib` keeps them out of the score. Selection is by the scope a name is declared in, not the whole mangled string, or a Roblox method taking an `ATL::CStringT` counts as ATL's. Two cases are still unnamed: an address where the linker folded two vendored functions keeps one symbol in our PDB and may keep the other in theirs, and a function-local static mangles its enclosing scope's index, which the two builds number differently.

A function-local static never matches by its mangled name. reccmp reads ours out of the `S_LDATA32` nested in the enclosing `S_GPROC32`, which carries only the bare name, and labels it `<variable>___<enclosing function>`. `webservice-globals.csv` hands the original the same label. The magic-static guard beside it does match by symbol, and so does the `dynamic atexit destructor` the compiler emits when the static has one, but only once a `// SYNTHETIC:` nameref claims it. Those two kinds are the exception to the rule that vendored globals stay out of the table, because a singleton like `G3D::Vector3::zero` is read straight from Roblox code and is data, which reccmp cannot match a vendored function through. A vendored vftable is the third: `G3D::ReferenceCountedObject`'s is stored by every Roblox constructor and destructor below it, and we never declare the class, so no `// VTABLE:` annotation can name it.

## Gotchas

`.github/workflows/build.yml` runs seven checks and six of them are not `reccmp-reccmp`. `reccmp-decomplint` runs with `--warnfail`, so a warning fails the build: a nameref belongs in a header, never in a `.cpp`. `reccmp-vtable` compares every vtable that has a name on both sides, which is why a vendored vftable only goes into `webservice-globals.csv` one at a time, listed in `VENDORED_VTABLES` — naming them all pairs ATL and STL tables whose entries reccmp cannot resolve. `reccmp-datacmp` compares every named variable, and it matches statics by bare name, so a name has to be unique in both builds; `next` named RakNet's file-scope one and a function-local one our G3D keeps. Run all seven before committing, not just the accuracy number.


Never sweep timestamps across the working tree. `core.autocrlf` is true and `.gitattributes` marks `3rdparty/**` as `-text`, so git stores those files byte for byte from disk. Touching their mtimes forces a re-stat, and a `git add -A` then rewrites every one of them to whatever line ending the working copy happens to hold. That is what the CI workflow's backdate step does, which is fine on a throwaway runner and destructive in a clone.

`RakPeer` has no `// VTABLE:` annotation because our `RakPeerInterface` declares one virtual too many or too few: from around slot 0x5c the two vtables run one entry apart, so `RPC`, `Ping` and `GetIndexFromSystemAddress` cannot be told apart by slot order. Reconciling that virtual list is what unblocks annotating those overloads.

The original link used `/OPT:ICF` and folded heavily. Many distinct source functions compiled to the same three-byte body and share one address, which is also why Ghidra's PDB source line importer throws several hundred `IllegalArgumentException` warnings when the symbols load. Those warnings are expected and harmless. Run `tools/check_folded.py` before concluding that a function at a shared address is genuinely unmatched.

`tools/check_folded.py` diverges from racers there. MSVC 8 keeps a PDB symbol for every alias the linker folded away, so all the `FOLDED` annotations on one address resolve rather than just the survivor. The check asks whether they landed on a single recompiled address instead of counting how many resolved.

`tools/` otherwise holds only what came from racers. Analysis scripts written along the way live in the session scratchpad.
