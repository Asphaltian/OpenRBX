# OpenRBX

Decompilation of Roblox as of 2007-12-20 using MSVC 8.0 SP1 (cl.exe 14.00.50727.762). Modeled after the [LEGO Racers decompilation](https://github.com/isledecomp/racers).

One target: **WEBSERVICE**, `WebService.dll`, an ATL Server ISAPI extension with 12,633 functions.

RBXGS, the game server, comes first because its binary ships an unstripped PDB, and the engine libraries under `common/` were shared with the client anyway. Three of the six LTCG objects in `WebService.dll` are `Client\win` sources. The client is in scope eventually.

## Building

```
<path-to-msvc8>\Common7\Tools\vsvars32.bat
cmake -B build -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build
```

Portable MSVC 8.0 SP1: https://github.com/Asphaltian/MSVC800-SP1, assembled from VS2005 Professional and KB926601. Use `vsvars32.bat`, not `VC\bin\vcvars32.bat`, which forwards to it via `%VS80COMNTOOLS%` and needs a registered installation. RTM (`.42`) will not reproduce the original codegen. `CMakeLists.txt` gates all decomp-specific flags behind `MSVC_FOR_DECOMP`, true only for cl 14.x.

Delete `build/` before trusting a green result on anything that moves an include. The object file that breaks is the one that did not need recompiling.

## Checks

CI runs eight checks and seven of them are not `reccmp-reccmp`. Run all of them before committing, not just the accuracy number.

```bash
reccmp-reccmp   --target WEBSERVICE --nolib                       # score
reccmp-reccmp   --target WEBSERVICE --nolib --verbose 0xADDRESS   # one function
reccmp-reccmp   --target WEBSERVICE --nolib --json out.json       # what is left
reccmp-decomplint WebService common --module WEBSERVICE --warnfail
reccmp-vtable   --target WEBSERVICE
reccmp-datacmp  --target WEBSERVICE
python tools/check_folded.py --target WEBSERVICE
python tools/reccmp_addr_padding.py
```

- **decomplint** runs with `--warnfail`, so a warning fails the build. It covers `WebService common` only, so nothing under `3rdparty` is linted and a bad marker there fails silently. Point it at `3rdparty/RakNet30` by hand after touching RakNet.
- **vtable** compares every vtable that has a name on both sides. A marker asserts a layout; a data source only resolves a store. Do not add a `// VTABLE:` marker to a class whose slots cannot all be claimed yet.
- **datacmp** matches statics by bare name, so a name has to be unique in both builds.
- **check_folded** asks whether every annotation on a shared address landed on a single recompiled address. MSVC 8 keeps a PDB symbol for every alias the linker folded away, so all `FOLDED` annotations resolve rather than just the survivor.
- **ncc** (`tools/ncc/ncc.py`) runs on its own Linux job and needs libclang plus the `clang` python bindings. It counts findings and still exits 0, so the count is the verdict. The bindings ship a libclang of their own next to `clang/cindex.py`, so pointing `--clang-lib` at that runs the same check locally on Windows; there is no reason to leave it to CI. A recovered name that trips the style is a style-file change and never a rename, which is why the patterns already carry specific alternatives rather than loosened ones.
- **Current MSVC** builds `app` and `network` with a modern cl plus clang-tidy. It reproduces locally out of an `amd64_x86` vcvars shell with `cmake -GNinja -DENABLE_CLANG_TIDY=ON`. Run it whenever a class or template lands: it is the only check that sees C++ conformance MSVC 8 lets through.

`--json` reports an effective match with its raw ratio and a separate `effective` flag, where the console prints it as a pass. A work list built from the ratio alone therefore lists finished functions as outstanding; filter on the flag.

`reccmp-reccmp` prints one row per recompiled address. Where our link folded two functions the original kept apart, only one annotation appears and the other is absent rather than listed as a miss. Probe the address with `--verbose` before reading absence as failure.

`--verbose` compares raw `[esp+N]` operands, so one missing local reprints the whole body as a mismatch at a constant offset. Add the frame difference back before reading a diff: `getPrimitivesTouchingExtents` looked forty percent wrong and was byte for byte right apart from a condition nothing else showed. What the shift cannot hide is a call, so read the original-only lines for callee names first; the two there named the predicate the source was missing.

## Ground Truth

`WebService.pdb` ships alongside the binary and is not stripped. Check whether it answers a question before working the answer out from disassembly. It carries symbol names, namespaces, class hierarchies, source paths, line numbers, per-object compiler command lines and the toolchain version.

Both files come out of the RBXGS installer on archive.org, extracted with [lessmsi](https://github.com/activescott/lessmsi) into `SourceDir/`. See the README for the URL and hashes. `.github/workflows/rbxgsbin.yml` does the same fetch for CI.

Load the PDB into Ghidra before writing anything. Reading the decompiler's pseudocode is faster than inferring structure from raw bytes and it recovers control flow a byte diff will not show.

### Feeding Ghidra Back

Ghidra's PDB loader takes function names and forgets everything else, so every local in
every function reads `local_XX` and every argument `param_N` until something puts the
recorded names there. Push them once and the decompiler output is readable for good;
re-deriving them from disassembly per function is the single most wasteful habit
available. The plugin answers its MCP tool names as HTTP paths on its own port, which is
what makes a bulk push scriptable when in-editor scripting is unavailable.

MSVC records `S_BPREL32` against its own virtual frame base, and that base does not agree
with Ghidra's entry-relative stack offsets. Do not assume the difference: vote it per
function by counting how many recorded offsets land on slots Ghidra already has, and take
the delta that wins. Getting this wrong renames real slots to unrelated names silently.

`reccmp-ghidra-import` pushes the other direction, our matched entities into the project,
and the CMake integration only creates its target once the project path and file name
cache variables are set. It drives Ghidra headless, so the GUI has to release the project
lock first.

### Querying the PDB

Load every cvdump section into one SQLite database and query that. Read the MSF container directly when a question needs a stream cvdump does not print. Extract our own `build/WebService.pdb` the same way and diff the two class by class for a work list of size, offset and signature mismatches.

Grepping cvdump text answers one section at a time and silently drops the rest. Every question worth asking spans several: a function's address is in SYMBOLS, its file and line span in LINES, its owning object in SECTION CONTRIBUTIONS, that object's flags in MODULES, its parameter types in TYPES, and its constness in the `LF_MODIFIER` its `This type` points at.

Four shapes in cvdump's output cost real data before the extractor was right, and each failed silently:

- The contribution table's `Imod` is hex. Read as decimal it maps every address to the wrong object.
- 61,440 type records carry four-digit ids, so a `0x[0-9a-f]{8}` pattern drops a third of the type table. Ids are referenced upper case and defined lower case.
- A basic-typed argument reaches an `LF_ARGLIST` as `T_INT4(0074)`, not `0x....`, so a hex-only parser turns `getPrimitive(int)` into `getPrimitive(void)`.
- An overloaded name reaches a field list as `LF_METHOD` pointing at an `LF_METHODLIST`, so a parser that reads only `LF_ONEMETHOD` loses every constructor and overload set.

Stream 4 is `/LinkInfo` here, not the IPI, so a 2007 PDB carries no `LF_UDT_SRC_LINE` and a class's declaring header has to come from LINES attribution. What the container does carry is the attribute provider's own output: thirteen `/src/files/*...webservice.inj` streams hold 68KB of injected source, reachable through the named stream table in stream 1.

### What Each Section Proves

Never guess a path, a name, an offset or an enum value. Each of these is recorded.

- **`reccmp-cvdump -m`** gives each module's `src =` line: the original directory and filename with its real casing. This is the layout to reproduce. LINES lowercases every path it stores, so only `src =` answers casing, and only for `.cpp` files.
- **`reccmp-cvdump -t`** writes TYPES, the authority for member names, offsets, class sizes, enum values and typedefs (`LF_NESTTYPE`). Ghidra's type browser resolves some names to the wrong namespace, so prefer the type records when they disagree.
- **Method constness** resolves through the `This type` `LF_POINTER` to either the class or an `LF_MODIFIER const`. It costs no instruction but it is recorded, and it changes what MSVC's alias analysis may assume.
- **LINES** maps an address to the file it came from and places a definition even when every call is inlined away. A header's records cover its own out-of-line copy; an inlined expansion is attributed to the **call site**, not to the callee, so a caller whose records never leave its `.cpp` still inlines header code freely. MSVC 8 has no `S_INLINESITE`, and a probe build settles this in three minutes when a helper's home is in doubt.
- **`reccmp-cvdump -m -seccontrib`** maps any address to the object that contributed it. It is the only way to place a namespace-scope global, which reaches the PDB as an `S_PUB32` with no module.
- **`S_BPREL32`** names every stack local and gives its frame offset. Declaration order cannot be read back from it, because MSVC lays locals out by size. Anchor the offsets on a local the disassembly already identifies and the rest of the frame reads straight off, which settles in one query what a spelling sweep cannot settle at all. Slots are shared between locals whose lifetimes do not overlap, so one offset carrying several names is normal and says nothing about scope.
- **Parameter names are recoverable and positional.** The recorded entries with a positive offset, in offset order, are the argument list; a hidden sret slot carries no name and drops out of its own accord. Renaming to them is mechanical, but four things make a textual rename wrong and each fails silently: a slot MSVC shared between disjoint lifetimes carries several names and any of them is already correct; a variadic member function is `__cdecl`, so `this` arrives as a stack parameter; the recorded names are frequently a permutation of ours, so a swap applied one side at a time collapses both onto one name; and a constructor's `member(param)` initialiser, a member reached through `.` or `->`, and any spelling the body already uses must all be left alone.
- **A recorded parameter name that matches a member name means the original wrote `this->`.** The rename otherwise turns `member = value` into a self-assignment that still compiles, so `Team::setScore`, `InstanceHandle::linkTo` and `GlueJoint`'s constructor each dropped forty points silently. Only positive-offset entries are parameters, and a local sharing a parameter's slot after it dies is why a function can record more names than it has arguments.
- **Where a class is declared follows from the file its in-class inline functions are attributed to.** A class cannot be declared above a type it uses, so a comparator or nested helper whose records land in the `.cpp` pins its whole dependent chain there too, however natural the header looks.
- **`S_FRAMEPROC`** carries `inl_specified` (whether the original wrote the `inline` keyword) and `wasinlined` (whether the callee was ever expanded anywhere). `wasinlined` answers in one query what a spelling sweep costs a build each.
- **`S_FRAMEPROC` also carries `inlasm`,** set on the 93 functions that reached inline assembly. It is the only witness that a body no spelling reproduces is expanding an `__asm` helper rather than doing something clever.
- **The raw string table** holds 2,001 Roblox source paths, readable straight out of the file. LINES names only files that contributed line records, so a header full of declarations is missing from it; the string table is the authority on whether a header exists. Every path there is lowercased.
- **Jump tables** settle enum values: read the byte index table and the jump table out of the image and the case-to-value mapping falls out.

Prove layouts, do not assume them. `DECOMP_SIZE_ASSERT(T, size)` fails the build on a mismatch. Assert a deliberately wrong size once to confirm the assert is live.

## Annotations

Functions in a compilation unit are ordered by ascending address. Namerefs obey the same order, and getting it wrong is silent: a marker placed above a lower-addressed one drops every annotation after it in that file.

```cpp
// FUNCTION: WEBSERVICE 0x100a77d0         complete, compared by reccmp
// FUNCTION: WEBSERVICE 0x100a77d0 FOLDED  identical code the linker merged
// TEMPLATE: WEBSERVICE 0x10103660         template body, nameref on the next line
// STUB: WEBSERVICE 0x100a7800             incomplete, skipped by reccmp
// LIBRARY: WEBSERVICE 0x10001000          CRT or third party, in library_msvc.h inside #ifdef 0
// SYNTHETIC: WEBSERVICE 0x10007040        compiler-generated, such as scalar deleting destructors
// GLOBAL: WEBSERVICE 0x102c7000           global variable
// VTABLE: WEBSERVICE 0x10056440           virtual function table
// SIZE 0xf8                               class size assertion
```

- **The colon is required** on everything except `// SIZE`. reccmp silently ignores `// VTABLE WEBSERVICE 0x...`, and the vtable stores inside constructors then compare as `<OFFSET>` instead of the vftable symbol, costing five to ten percent.
- **`// FUNCTION:` asserts a 100% match.** Anything less is `// STUB:`. This applies to `// SYNTHETIC:` too: if the claim will not hold, drop the nameref rather than assert it.
- **`FOLDED`** marks siblings the linker merged onto one address. They share that address, are exempt from ascending order, and do not need the `STUB()` macro.
- **A class with two bases needs the base-qualified vtable marker,** `// VTABLE: WEBSERVICE 0xADDR Base`, and the base has to be spelled the way the recompiled PDB does: `RBX::Notifier<RBX::World, struct RBX::AutoDestroy>`, with the space and the keyword. The second base's adjustor thunk carries no source line, so it goes in `webservice-synthetic.csv` or `reccmp-vtable` fails on slot zero.
- **A nameref belongs in a header, never in a `.cpp`.** decomplint fails on it under `--warnfail`.
- **An access label between a marker and its function unresolves the annotation.** reccmp pairs a header annotation with the declaration that follows it, so moving a method into its recorded access section by writing `private:` under the marker silently drops that function and takes others in the file with it: one label in `GUI.h` cost ten matches. Put the label above the marker.
- **Wrap long namerefs in `// clang-format off`.** clang-format reflows a `//` comment past the column limit, the name line loses its tail, and the annotation silently stops resolving.
- **A body written `{}` on one line** leaves decomplint's parser inside the function. The next marker is reported `unexpected_marker` and dropped. Split the braces.
- **A `// STUB:` is a claim in the other direction,** and it goes stale as its callees land. Twenty-nine v8datamodel bodies were already byte-exact behind one, which drops them from the denominator and reads as work outstanding. Flipping every `STUB` in a directory to `FUNCTION`, scoring once and reverting whatever misses costs one build and finds them all.

## Class Pattern

```cpp
// VTABLE: WEBSERVICE 0x10056440
// SIZE 0xf8
class Instance : public GuidItem<Instance> {
public:
	virtual ~Instance();      // vtable+0x00
	virtual void VTable0x04();

	// SYNTHETIC: WEBSERVICE 0x100078f0
	// RBX::Instance::`scalar deleting destructor'

private:
	int worldIndex;                       // 0x28
	undefined m_unk0x2c[0x40 - 0x2c];     // 0x2c
};
```

`DECOMP_SIZE_ASSERT(Instance, 0xf8)` goes in the source. Member offset comments and `vtable+0xNN` comments are required. Size gaps use a subtraction so the length documents itself.

Members lay out in declaration order regardless of access section, so an out-of-order member shifts every offset below it. Both `std::set` and `std::vector` carry a leading `_Myproxy` under `_SECURE_SCL`: a set is `_Myproxy, _Myhead, _Mysize` and a vector is `_Myproxy` then three pointers, which is why a vector's `_Myfirst` reads four bytes past where the type record puts the member.

## Types and Names

`undefined`, `undefined2` and `undefined4` from `decomp.h` stand in for unproven types. Do not guess `int` or `float` until a 100% match proves it.

Unlike racers, the PDB is not stripped, so real names are usually available and placeholders are the exception. Where nothing is recorded, NCC rules apply: `FUN_XXXXXXXX`, `g_unk0xXXXXXXXX`, `m_unk0xXX`, `p_unk0xXX`, `VTable0xXX`, `c_` on enum constants. `tools/ncc/ncc.style` also carries `prop_`, `event_` and an all-caps alternative, because those are the original's own conventions recovered from its symbols.

Rename a placeholder only when the evidence is unambiguous. Competing plausible names mean keep it, because a misleading name is worse than a neutral one.

## Code Style

- Bit tests read `if (flags & c_flag)`, never `!= 0`.
- No redundant `this->`.
- Win32 calls use the un-suffixed macro (`CreateWindowEx`, not `CreateWindowExA`), which is what the source had even though the import is the `A` symbol.
- `NULL` for pointers, `TRUE`/`FALSE` for booleans, plain `0` only for scalars.
- No leading `const` on return-by-value; NCC rejects it.
- clang-format the files you touch. Reflowing does not change codegen.

## Matching a Function

1. Read the decompilation and check the byte budget. The gap to the next address bounds the body, so a body that cannot fit means this is a wrapper and the logic is in a callee.
2. Read the line records before writing anything. Count the statements they imply, note which lines generate no code, and check `S_BPREL32` for the local set. Getting the statement count right first is worth more than any respelling afterwards.
3. Search TYPES for a helper before writing arithmetic out. Hand-inlining a helper the original called is the single most expensive mistake available.
4. Take calling conventions from the definition, not a call site. `__thiscall` means a member. A `__thiscall` on a global means that global is an instance. A member returning with a bare `ret` is `__cdecl`, which means `static`; the type records do not spell it and the return byte does.
5. STUB every unknown callee, in ascending address order, with `STUB(0xADDRESS)` in the body so `/OPT:ICF` cannot fold them together. Empty destructors are exempt.
6. A caller whose match depends on a call the stub loses is blocked on the callee's real body, not on an attribute. App builds `/Ob2`, so a one-store stub inlines and the caller loses the stack the real call needed. Write the body; `__declspec(noinline)` is not available as a shortcut.
7. Write clean C++, not decompiler pseudocode. No `*(int*)(this + 4)`, no gotos, no raw float bit patterns.
8. Build, then `reccmp-reccmp --target WEBSERVICE --nolib --verbose 0xADDRESS`, and iterate to 100%.
9. Compare the vtable address itself, not only the functions. It is what exposes a missing or misordered virtual.
10. Re-verify every function that touches a class you changed, then run the checks above.

Starting a new class, match the constructor, destructor and scalar deleting destructor before any method. Those three prove size, base chain, vtable layout and member order together.

## Decompilation Principles

- A type is proven when a `// FUNCTION:` using it reaches 100%, and not before.
- A diff means the code is wrong. Find the cause: layout, types, a missing base, an inlining decision. Register allocation is never the explanation.
- Fix the callee before sweeping the caller. A three-byte helper's frame changes every caller's stack layout.
- Claimed matches are not verified matches. Re-measure anything inherited or generated.
- Every annotation carries a real address. No placeholders.
- Casts plus pointer arithmetic mean the types are wrong. Find the real class.
- One root type per header. Nest one-owner helper types inside their owner.
- An unexplained address gap means a function sits in the wrong class, or one file mixes classes. Move whole classes; never split a class's methods across files to tidy address order.
- Measure with reccmp on the linked image, not by eye on an `/FA` listing. A listing's byte counts mislead because relocated `call` and `mov reg, [__imp_...]` bytes read short. Its mnemonic stream is still usable for cheap sweeps, since aligning it against the original's disassembly costs a compile rather than a link.

## MSVC 8.0 Codegen Patterns

Each of these cost a round trip. Keep the sources clean and leave the reasoning here.

### Float compares and x87

- **`a > b` and `!(a <= b)` are not the same compare.** The direct `>` emits the unordered-safe `test ah,5 / jp`; the negated form emits `test ah,1 / je`.
- **A memory-to-memory compare loads the right operand into st(0) whichever way it is spelled,** so the load says nothing about the operator and the flag test says everything: `>` emits `test ah,5 / jp`, `<` emits `test ah,0x41 / jne`. Read the operand order off the load and the direction off the test.
- **A compare whose loaded side is also stored uses `fcom` rather than `fcomp`,** which pins that side to st(0) regardless of the operator. Where one arm of a symmetric pair of tests loads the opposite side from its sibling, that is the reason, not a different spelling.
- **MSVC reorders the terms of an `&&` chain of float compares.** Write the source in the order the binary tests them.
- **A member compared against a parameter stays in memory** when written `member != value`, giving `fcom`. The reverse spelling loads both and needs `fucom`.
- **`std::swap(a, b)` loads `a` into st(0) first,** so argument order is recoverable from which member the binary loads first.
- **`std::min` and `std::max` return a reference,** visible as a `lea` of both candidates and a deref. Suppress `windows.h`'s `min` macro or the comparison compiles inline and nothing lines up. The two candidates land in declaration order.
- **Locals are pushed in declaration order.** Getting the order wrong leaves an extra value on the x87 stack and shifts every later `st(n)` operand, which reads as a large diff for a one-line cause.
- **A literal one ULP wrong reads as a naming problem.** When reccmp fails to resolve one side's operand, read the constant out of the image; a constant it renders on both sides is already proven equal.
- **`x = x * -1.0f` is not `x = -x`.** The multiply hoists the constant into an x87 register kept live across the enclosing loops and spends one `fmul st(n), st(0)` per flip; the unary negate emits `fchs`, which only reaches st(0) and so drags an `fxch` in with it. A run of sign flips at different stack depths is the multiply.
- **A scalar on the left of a vector binds G3D's free `operator*`, not the member overload,** and the two schedule differently. `(0.01f * percent) * v` loads the scalar once and multiplies both components from it; `percent * 0.01f * v` reassociates under `/fp:fast` and folds the scalar into each product separately. `GuiRoot::toPixelSize` sat at 94 on that one difference.
- **`RBX::Math::sign` returns `float`.** `G3D::sign` returns double, which loads a qword where the float one loads a dword.
- **Every float to int conversion goes through `G3D::iRound`,** which is `lrintf` in `g3dmath.h`, an `__inline` spelling `fld`/`fistp`. A C cast lowers to `call __ftol2_sse` and no spelling of it will ever match. The float overload loads a dword, so the argument has to reach it as a `float`: `float f = floor(x);` then `iRound(f)` gives `fstp dword`, reload, `fistp`. The `inlasm` flag names the thirteen App and RbxView functions that use it.

### Statement shape and evaluation order

- **Argument evaluation is right to left,** for operators as well as calls. The pair named in the second argument gets the callee-saved registers. Naming the left product in a local is what orders `(a * b) * c.inverse()` the way the original has it.
- **Naming each argument in a local restores left to right.** A recursive call whose arguments are computed inline evaluates them backwards, which decides which parameter lands in `esi` and flips every register below it. Two named locals in declaration order are the whole fix.
- **A helper the caller hands a temporary sinks its parameter copy into the branch that uses it.** Where the original stores unconditionally, the caller named the value; where it stores inside the `if`, the callee holds the test. Which side owns a null check is readable from that one store.
- **A comparison puts its right operand in the register.** `a == b` on integers is `mov eax, b` then `cmp [a], eax`. The operand the binary loads first is the one written second.
- **Assigning a call's result before comparing changes the order.** `f() != g()` runs `g()` first; `x = f(); return x != g();` runs `f()` first.
- **Which operand is written first decides evaluation order even where the operator is commutative in meaning.** `prim1 sum > prim0 sum` and `prim0 sum < prim1 sum` mean the same thing and evaluate in opposite orders.
- **Building a return value through a constructor evaluates every argument before storing anything.** Assigning members one at a time interleaves the arithmetic with the stores.
- **A vector expression batches its loads, then drains x, y, z; component assignments store as they go.** Routing the arithmetic through a named intermediate gets both: `Vector3 delta = v - t;` then one assignment per component batches the loads and still visits each component in turn. A component whose store is dead keeps its arithmetic in that slot, which is how a lone `fadd` lands between the x and z stores, and reading the member back rather than a float local is what stops the later store to it hoisting above the expression. `getIntendedMovementVector` sat between 90 and 95 across eleven spellings of those three statements.
- **Chained assignment (`a = b = c = value`)** keeps the value in a register across the stores and writes them right to left. Separate statements reload it each time.
- **MSVC lays a block out where its statement sits in the source.** A constant return written last ends up last, so a rare `return true` has to be moved below the hot path and the result tested afterwards.
- **How you spell the return path decides register assignment.** `if (cond) return x; return y;` makes `x` the fall-through value; assigning to one named result keeps `y` in place.
- **`x = a; if (cond) x = b;` and `x = cond ? b : a;` are not the same codegen.** The if form folds into one load and a conditional reload; the ternary evaluates `a` into a scratch register first and copies it in the else branch, one instruction longer. An if/else assigning two variables in each arm puts the else block at the tail.
- **A ternary materialises its arms into a 32-bit register.** A `bool` function ending `movzx eax, al` on one path and `mov eax, 1` or `xor eax, eax` on another returned a ternary; a plain `return false` writes the shorter `xor al, al`. `test al, al / setne al` on a call that already returns `bool` is the same tell.
- **`return p ? true : false` is not `return p != NULL`.** The comparison puts its right operand in a register, so the direct form spends an extra `xor eax, eax` where the ternary compares against the immediate.
- **Comparing two `bool`s is a byte compare.** Writing one inline as a negation promotes both to int and costs a `movzx`. Store the negation in its own `bool` first.
- **Multiply operand order is normalised; parenthesisation is not.** Commuting factors moves nothing. Regrouping a three-term sum moves several percent, in both directions, and one grouping cannot always serve two call sites.
- **`(&prim0)[index]` is not `index == 0 ? prim0 : prim1`.** The difference only shows once a caller passes a variable index.
- **Two statements on one source line are one line record,** and the record lengths say when that happened. A comma declaration is invisible to a statement count taken from the source.
- **Naming a call's argument decides when the object expression is loaded.** `p->f(a * b)` lets MSVC hoist the load of `p` above the arithmetic; naming the argument first moves it below, where the original has it. `PartInstance::legacyTraverseState` sat at 51.61 until its `CoordinateFrame` was named, and spelling the member as an accessor call moved nothing.
- **An implicit conversion and an explicit functional cast are not the same codegen.** `f(x)` through a converting constructor builds the temporary in the argument slot: `push ecx` reserves it and the store goes through a materialised `mov eax, esp`. `f(T(x))` builds a temporary and copies it, spending a whole-dword `mov eax, [esp+N]` and `push eax` even where `T` is one byte with a `const` member. `PartInstance::onCanAggregateChanged` sat at 63.64 on that difference alone.

### Naming locals

Where the line records show lines that generate no code, the original named something, and naming it back is worth more than any respelling. A reference declaration emits nothing, so a run of no-code lines in a header is the signature of aliases. A `float` copy does emit a load, but the scheduler hoists it into the following statement's run, so it leaves no record either.

- **Which constant a callee-saved register holds counts that literal's uses.** Where the original keeps 0 in `ebx` and spells 1 as an immediate and ours does the reverse, an argument is wrong somewhere and the allocator is not the explanation. `World::step` sat at 65 until its two inner `Profiling::Mark` objects took the `false` the outer one does not.
- **The frame tells a named local from a parameter copy where the store cannot.** MSVC packs a call's temporaries together and puts the register-spill slot below them, so two bodies that are instruction for instruction identical with one pair of 4-byte slots swapped differ only in whether the value reaching the callee was named. `World::destroyJoints` sat at 90.77 through eight spellings of its loop and its condition until the ignore-group test became a helper holding its own null check.
- **A pure permutation of `ebx`, `ebp`, `esi` and `edi` means a local is missing.** Where the two bodies are instruction for instruction identical and only the callee-saved assignment differs, the original named one more value; the extra name re-ranks the allocator's candidates and the whole assignment falls into place. Four of `SpatialHash.cpp`'s functions failed this way and each took exactly one name: the `Assembly*` between the accessor and the test, the `SpatialNode*` between `findNode` and `destroyNode`, the old bucket head before it is relinked. Do not read it as a preference the compiler is free to make.
- **A returned aggregate read through `eax` was never named.** MSVC addresses a call's hidden return slot through the pointer the callee leaves in `eax` only while the value stays a pure temporary; a named local, and a `const T&` bound to it just the same, moves every read to `esp`. So an original that reads two or three members off `[eax]` is feeding the call result straight into another inline, and a file-static helper that is expanded at every site leaves no symbol, no line records and no contribution row: its absence from the PDB is not evidence it was not written.
- **Naming a temporary the callee takes by const reference changes its x87 drain.** `f(AABox(lo, hi))` and `AABox box(lo, hi); f(box)` compute the same six products in the same order and then spill them with a different `fxch` schedule. A named local of a class type need not reach `S_BPREL32`, so its absence from the local list does not rule the spelling out.
- **A leading underscore on a parameter is the strongest tell there is,** because it exists so the body can bind the unprefixed name.
- **Alias only what the type records name an accessor for.** An alias over a member reached through its own accessor is a real lever; the identical trick on a plain member can cost double figures.
- **Aliases leave no `S_BPREL32` record,** so an empty local list never rules them out. The list is still authoritative for what did get a stack slot.
- **A named local can also be wrong.** Two loops with identical shape can want opposite answers, so read the record per function and change one loop at a time.
- **Declaration order between two symmetric values decides which callee-saved register each gets,** one pair per swap. Where the two bodies differ only in that a pair of registers is exchanged, swap the two declarations rather than rewriting the statement. Index one before index zero is common, because the original wrote the argument list and the compiler evaluated it right to left.
- **Where a declaration sits decides when its value is loaded,** to the instruction. Moving one declaration between two others moves its load into that slot, so a value loaded too late in ours is a name written too late, not an allocator preference.
- **The frame size bounds what locals can exist.** A frame with no room for the class type the obvious source would need proves the original named scalars and wrote the arithmetic out. No spelling of a call on that class type will ever match, because the vendored inline fixes an evaluation order the scalars do not have.
- **An `add reg, offset` ahead of a loop is not evidence of a source alias.** MSVC strength-reduces a member subobject used repeatedly in a loop on its own, and writing the reference can stop a trivial accessor inlining, turning the loop head into a call.
- **Where a callee stores its result decides its caller's register allocation.** A function that already matches is still worth respelling when its caller does not.

### Inlining

- **`wasinlined` in `S_FRAMEPROC` answers whether the original ever expanded a callee.** Check it before assuming either way.
- **An empty callee has to be defined out of line to keep its call.** Written in the class it expands to nothing and the caller loses a call the original makes. `ClumpStage`'s two empty notification methods are why `World::onMotorAngleChanged` stopped at 89.47.
- **When the original keeps a call out of line and `/Ob2` inlines ours, the callee's body is too small,** not the attribute list too short. `S_FRAMEPROC` carries `inl_specified` and `wasinlined`: a function the original marked `inline` and never expanded is one whose real body exceeds the budget, so writing it is the fix. `Instance::raisePropertyChanged` is 160 bytes and `wasinlined` is 0.
- **Which sites MSVC inlines a large function into is cost-driven and not spelled.** Source order is not the lever: MSVC 8 inlines a callee defined later in the same translation unit just as readily. Neither is a stubbed grandchild callee. Where the original inlines at some sites and calls at others, the cause is in the bodies, not in an attribute: a second `__forceinline` copy of a function the original declares once is a duplicate the type records do not have. A thin wrapper does not work either, because the expansion carries the wrapper's parameter copy and flips the first callee's operand order.
- **Two expansions of one inline in one function need not schedule alike.** `GlueJoint::putInKernel` expands `pointToWorldSpace` twice; the second is byte for byte right while the first permutes which operand of every product is `fld`ed and how the reassociated sum is grouped. No spelling of the caller moves them together, so read a residual confined to one of several identical expansions as scheduling, not as a wrong statement.
- **An inline the original emitted only because its address was taken cannot be reproduced early.** MSVC emits no COMDAT for an inline nothing in the translation unit references, so `/OPT:NOREF` has nothing to keep and the marker resolves to nothing. `World.h`'s four metric accessors wait on `Workspace.cpp`, which stores their addresses in a stats table. Check for a `mov reg, imm32` of the address before assuming a missing body is a spelling problem.
- **A stub's register footprint reaches its caller.** MSVC 8 tracks which registers a callee in the same translation unit clobbers, so a caller of a one-store stub keeps `this` in `ecx` across the call where the original spilled it to a callee-saved register. `ContactManager::onNewPair` sat at 54 until `createContact` grew the two virtual calls the original's first lines make. Give a stub the calls its real body starts with before reading the caller's frame as wrong.
- **A stub that ends `return someOtherStub();` folds onto that other stub.** The callee inlines, its store overwrites the caller's, and the caller's `STUB()` constant is eliminated as dead.
- **A virtual destructor with no standalone function in the PDB was inlined everywhere.** Define it inline in the header without an annotation. Out of line in the `.cpp` it cannot inline across translation units and every derived destructor calls it.
- **A base destructor that does have a standalone function is still inline when the contribution table places it in an unrelated object.** Only a COMDAT can be kept from an object that has nothing to do with the class. Out of line in its own `.cpp`, every derived deleting destructor calls it instead of expanding it, the bodies stop being identical, and a family the original folded onto one address splits into one group per translation unit.
- **The order of the stores inside an inlined setter is not recoverable from the emitted order.** MSVC schedules them against the surrounding register and x87 pressure, so the same source emits them differently at two call sites. Sweeping the setter's statement order is wasted; the lever is always in the caller.
- **A recursive helper and the loop it compiles to are not interchangeable.** MSVC turns tail recursion into a loop at the definition, so both spellings match there, but a caller inlines exactly one level of the recursion and none of the loop.
- **A helper's temporaries are allocated once per expansion,** so an expression that reads identically when written out does not schedule identically. Sweeping spellings of a hand-inlined form is hopeless, and the score ordering of hand-written variants carries no information about which one the original had.
- **Score a vendored inline's own out-of-line copy before blaming its spelling.** An inline that is also emitted as a COMDAT can be compared directly; where that copy is byte for byte right, every residual at an expansion of it is caller context, and rewriting the vendored body will not move it. Confirm the copy exists first: the contribution table placing it in an object with nothing to do with the class is what proves the definition is in a header rather than the matching `.cpp`.
- **Two recorded helpers that look composable need not compose.** Routing one through the other can drop four functions from 100 to the seventies.
- **Where two spellings each fix one half of a residual and break the other, neither is the cause.** Two halves that trade off share an upstream cause, usually the x87 stack the earlier statement left behind, and sweeping the pair against each other never converges. Fix the statement that fills the stack, not the two that read it.

### Loops and control flow

- **A dense switch lowers to descending `sub eax, N / je`.** An if/else chain emits repeated `cmp` against each value. Ghidra prints both as `if` chains when the cases are sparse, so the bytes are the only witness.
- **A `switch` case is not a scope MSVC 8 reuses a temporary in.** Six bodies written as case bodies give four `Vector3` slots each where the same six at function scope share four. The frame size is the evidence, and dispatching with `case NORM_X: goto x;` over labelled statements below is the one sanctioned goto.
- **The comma operator decides how many temporaries stay live.** `c0 = f(4), c1 = f(6), c2 = f(7), c3 = f(5);` keeps four alive at once where four statements reuse one.
- **A `for` increment is attributed to the last statement of the body,** not to the `for` line. A loop with no record at its `_Inc` call has its step written out at the bottom of a `while`.
- **Branches that leave the x87 stack at different depths cannot merge,** so MSVC clones the statement after the `if` into both arms rather than jumping to a shared tail. Two copies of one line number in the line records is that, not two statements, and a merged tail in ours means the arms are computing the same shape where the original's differ.
- **A guard written as `break` out of `do { } while (0)`** makes the whole body a loop body, so MSVC hoists every callee-saved push to the loop preheader as one group instead of shrink-wrapping per register. An `if (p != NULL) { ... }` around the same body moves two stack displacements and the pop order.
- **`std::for_each` passes its iterators through `_Unchecked`.** A raw node walk with none of the `_invalid_parameter_noinfo` checks the file's other loops carry is `for_each` plus `std::mem_fun`, not a written-out `for`.
- **An if/else tail merge hoists the shared setup above the branch.** A ternary emits the whole address selection up front and costs the function its register assignment as well.
- **Identical early exits are one shared return.** The line records give it away by sending both tests to the same line.
- **A loop MSVC fully unrolls leaves no line record on its `for` line,** so one missing line in an otherwise dense run of records is an unrolled loop, not a missing statement. Count the stores in the body to recover the trip count.
- **Two tests of one condition on two lines is an `if` wrapping a `while`.** A rotated loop puts the entry guard and the back-edge test on the same line; a second line holding the same test means the original wrote the redundant guard, and the back edge targets the inner test. `~SpatialHash` lost forty points to this.

### Class shape and layout

- **Abstract bases carry `__declspec(novtable)`.** Without it the constructor stores a vftable the binary never stores, and any derived destructor gains a base vftable store plus a whole SEH frame, which reads as a fifty percent diff. Do not delete virtuals to force the number.
- **A `ComputeProp` member records its owner's inheritance shape.** MSVC sizes the pointer-to-member by how the class inherits, so `ComputeProp<float, Primitive>` is 16 bytes and `ComputeProp<float, Assembly>` is 24. A class size off by that difference means a base is missing.
- **Virtual slots follow declaration order, not access order,** exactly as data members do. Declaring a public override above the protected block it belongs under moves every slot below it, which reads as one wrong `call [eax+N]` in an otherwise perfect body. The field list gives each slot's `vfptr offset`, so the order is recorded rather than inferred.
- **A gap between a class's last member and its recorded size is a vtordisp, not padding.** MSVC inserts four bytes ahead of each virtual base whose virtuals the class overrides, when the class also has a constructor or destructor. `LF_VBCLASS` names the virtual bases and its `vbpoff` names the vbptr slot the member list leaves as a hole; an empty virtual base costs nothing, so the arithmetic only closes once the vtordisps are counted. `RBX::Tool` ends its members at 0x230 and is 0x238: one vtordisp plus a four byte `ILocation`, with `ISelectable3d` free.
- **A derived class starts its own members at the base's non-virtual size,** below the base's `sizeof`, because the virtual bases move to the tail of the most derived object. `RBX::Flag` puts `evilClone` at 0x230 inside a `RBX::Tool` the type records call 0x238, and the two disagree by exactly the tail the vbases vacated.
- **`Math` and `Units` are classes of static members, not namespaces.** The difference is visible: a class's magic-static guard mangles a name reccmp can pair, where a namespace function's guard is `$S` and anonymous. A guard defined in a `.cpp` stays anonymous whatever the scope.
- **A `static const float` folds to a constant and loses its magic-static guard** unless the initializer calls something. A guard in the binary proves the initializer was a call.
- **A `const float` at namespace scope** is emitted as a real global and referenced by name, where the binary reads an anonymous constant out of the float pool. Write the literal at each use.
- **Which object emits a scalar deleting destructor decides whether it inlines the destructor or calls it,** and the contribution table says which. Reproducing the split needs the original's own code, usually an inline constructor that forces the vftable out of a second object.
- **A direct call to a method the type records mark virtual means the original qualified the call.** MSVC 8 never devirtualizes a call on `this`, so the class name is written out at the call site.
- **boost's `shared_ptr::operator<` compares ownership, not the pointer,** so it loads offset 4.
- **A `__int64` multiply lowers to `_allmul`,** and the conversion back to float tells you the signedness: a plain `fild` is signed; a pair that splits the sign bit out and subtracts it is unsigned.
- **Read a CRT call's name out of the publics.** Never infer it from what the surrounding code looks like it should do.

### Templates and folding

- **Most of what is left in the engine is template instantiations**, not one-off functions. Count the unannotated symbols by owning template before picking work: one template written right settles every instantiation of it.
- **A template instantiation belongs to whichever object the linker kept it from,** and that decides its flags. An instantiation kept from a `/GL` object is LTCG where ours is not, and no source spelling closes that.
- **The instantiation set names the spelling.** `std::vector<bool>::back` is emitted out of line where `vector<T*>::back` inlines to nothing, so a helper whose module shows one and not the other called `back()` rather than `*(end() - 1)`. `fastRemoveIndex<bool>` went from 46.51 to 100 on that plus the recorded `newSize`, which its pointer sibling records and it does not.
- **An explicit instantiation carries no line records,** so it takes a `// TEMPLATE:` marker with a nameref on the template body in the header.
- **Explicit instantiation of a derived class does not reach its bases.** Name each one.
- **A template constructor is user-declared,** so a class that has one also needs an explicit default constructor back or it stops being default constructible.
- **Name a non-type template parameter something an inherited member does not shadow.** MSVC 8 rejects forwarding it with "expected compile-time constant expression" when a member of the same name is visible.
- **The declaring namespace of a template member sits after its arguments in the mangling,** so splitting on the first `@@` misreads `boost::shared_ptr<RBX::Instance>::~shared_ptr` as Roblox's.
- **A nameref cannot always live on the template body.** `Notifier`'s two `raise` overloads interleave in address order across their instantiations, so no arrangement of two blocks in `Events.h` keeps one ascending sequence. The instantiation's nameref goes in the header of the class that owns the instantiating object instead, wherever ordering does hold.
- **A folded address only belongs in a library file when every alias does.**
- **`/OPT:ICF` folds heavily.** Many distinct source functions compiled to the same three-byte body share one address. This is also why Ghidra's PDB line importer throws several hundred `IllegalArgumentException` warnings, which are expected and harmless. Run `check_folded.py` before concluding a function at a shared address is unmatched.
- **A wrong constant in a three-byte body scores 100** against whichever alias reccmp pairs and shows up only as the fold not happening.

## reccmp Data Files

reccmp parses the recompiled PDB only, so the original's side gets its names from CSVs under `reccmp/`.

One file per library, named after the library, the way racers splits `directshow` from `dxguid`. No comments: the files carry rows only. reccmp ignores any column it does not know, which is what `x-text` in the strings file is for.

- **`webservice-function-size.csv`** gives the original's 12,633 function extents, so reccmp knows where each one ends without a symbol. Every other file edits this baseline.
- **`webservice-atl.csv`, `-boost.csv`, `-crt.csv`, `-g3d.csv`, `-ijg.csv`, `-libpng.csv`, `-stl.csv`, `-zlib.csv`** are the third-party code, typed `library` so `--nolib` drops them. IJG, libpng and zlib ship inside G3D and are still their own files, because a row is only reviewable next to the library it came from.
- **`webservice-symbols.csv`** names Roblox functions the tree does not annotate yet, with no type at all, so they stay in the denominator as unimplemented.
- **`webservice-floats.csv`** types the 394 floating-point constants MSVC emitted as their own `.rdata` COMDATs, 243 four byte and 151 eight byte. MSVC names each one `__real@<hex of the value>`, so the symbol carries the bytes and every address was checked against the image. `ZOOM_FACTOR` and `SKY_BOX` are Roblox `static const float`s the linker folded onto the matching constant, so this file loads before `webservice-statics.csv` and those two keep their Roblox names.
- **`webservice-globals.csv`** names globals, class statics and vftables by mangled symbol. A function-local static never matches by mangled name: reccmp labels ours `<variable>___<enclosing function>` and the CSV hands the original the same label. Magic-static guards and `dynamic atexit destructor` symbols do match by symbol, the latter only once a `// SYNTHETIC:` nameref claims it.
- **`webservice-statics.csv`** matches by bare name, so a name under three characters or one two translation units share stays out.
- **`webservice-strings.csv`** types the original's 3,569 string literals. MSVC records the width in the mangling, `??_C@_0` narrow and `??_C@_1` wide, and without it reccmp reads all 145 wide ones as narrow.
- **`webservice-synthetic.csv`** covers functions the source cannot annotate: second-base adjustor thunks and compiler-generated bodies with no source line. `synthetic` asserts a match exactly as `// FUNCTION:` does; `stub` names and pairs without asserting.

Rules that bite:

- **`library` means third-party code and nothing else.** It is not a way to quiet a function that will not match. Typing Roblox code `library` deletes it from the denominator *and* hides its accuracy, so a wrong claim behind one of those rows reads as a clean 100%. 1,674 Roblox rows were typed that way and were masking 132 annotations that asserted matches they did not hold.
- **A `symbol` or a `name` is a match, not a label.** `match_functions` pairs on either, so there is no way to name an address without pairing it. A naming file may therefore only carry originals our build has no counterpart for; the moment a row starts pairing, it belongs in source as an annotation. This is why `webservice-symbols.csv` is regenerated rather than edited.
- **Which column a row keys on is not a detail.** A mangled symbol goes in `symbol` and a bare name in `name`, and a writer that emits only one of the two silently drops every row that used the other, leaving an address with no label attached to anything. The 43 `dynamic atexit destructor for 'prop_X'` rows are bare-name rows for exactly this reason: neither the initializer nor its destructor reaches the PDB as a public.
- **Every public carries a section-relative address, not a flat one.** Section 1 is `.text`; anything in `.rdata` or `.data` needs the section header stream, and an extractor that only maps `.text` reports 4,405 vftables, every RTTI record and all 394 `__real@` constants at zero. Check a `__real@`'s bytes against the value in its name before writing a row.
- **`stub` is the type for a pairing you do not want scored.** It names and pairs the function but leaves it out of the accuracy average, which is what a vftable slot needs when our body is not byte-exact yet. Dropping the row instead breaks `reccmp-vtable` on that slot.
- **A blank type is skipped, not applied.** It cannot clear a `library` flag an earlier file set, so an address must not appear in a library file at all if any of its aliases is Roblox.
- **Order is by data-source list, then by row.** Every row that overrides a folded address has to load after the row it overrides.
- **A row at a folded address must carry the alias reccmp's recompiled entity actually holds,** which is not the name reccmp prints in the diff. Sweep the aliases the recompiled PDB lists at that address rather than reasoning about which one `min()` should pick.
- **Adding an instantiation to a template already folded there shifts the pick,** so those rows have to be rechecked whenever a new family lands. The symptom is not a missing function but a caller stuck short of 100% whose diff prints `call <OFFSET>` on the original's side against a resolved name on ours: the row is there and carries a sibling alias our binary never emitted.
- **A library function from a prebuilt archive has no `S_GPROC32`,** only a public, so its row has no size. reccmp still names the call, but handing it every unsized row lets `reccmp-vtable` pair ATL and STL tables it cannot resolve. They go in one at a time, listed in `UNSIZED_KEEP`.
- **A label with commas has to be quoted.** Otherwise the symbol reaching reccmp is a prefix that matches nothing and the row silently does nothing.
- **A third-party vftable goes in one at a time,** listed in `VENDORED_VTABLES`, for the same reason.
- **Two overloads of one template function share a 255-char prefix and pair in address order,** so a link that emits them in the opposite order crosses them: the caller's diff prints one overload's signature against the other's. Blank the symbol on the row whose pairing is wrong and give that address a bare-name row in `webservice-statics.csv`; the surviving symbol row then pairs correctly and the blanked one takes what is left.
- **A dynamic initializer and its atexit destructor pair by name, not by symbol,** because neither reaches the PDB as a public. They take a `// SYNTHETIC:` nameref in the owning header, or `// LIBRARY:` in `library_msvc.h` for CRT, ATL and G3D, and a row in `webservice-synthetic.csv` does nothing for them.
- **A file static an initializer stores through needs its own row before that initializer can match.** The diff prints `<OFFSET>` on the original's side against the resolved name on ours, and the name to use is the one already on the `+` line. Statics with no public go in `webservice-statics.csv` by bare name; class statics and vftables go in `webservice-globals.csv` by mangled symbol.
- **Where our link folds two functions the original kept apart, only one of them can ever be claimed.** The recompiled entity is already paired, so the second original has no counterpart to match and is not a gap a row can close.

reccmp is `Asphaltian/reccmp@msvc800`, a fork, and `tools/requirements.txt` pins that branch.

A module is not a file. Whether an original object is finished is a different question from whether one of our source files is, because a header can carry functions from several objects. Claim a file complete only by scoring every annotation that file carries.

## Build Layout

Each original static library gets its own OBJECT library, because each was compiled with different flags. The PDB records a command block per object and `reccmp-cvdump -s` prints it, so the flag set is read rather than inferred. Grouping objects by command block is also how the Lua 25 are told apart from App proper. `CMakeLists.txt` carries the full set. The differences that bite:

- App, RbxView and AppDraw were built `/GS-`. Network and RenderLib were not. Stack cookies show up as spurious mismatches if this is wrong.
- App is the only library with `/fp:fast`, and the only one with `/Ob2 /Oi /Oy` together.
- App carries `/Gy` here and did not in the original, and it has to stay on. Dropping it looks harmless and then fails `check_folded.py` the moment another in-class inline lands.
- RenderLib is the only one with `/Wp64`.
- Six translation units are the only objects compiled `/GL`, so the link ran `/LTCG`. Nothing else in the binary is LTCG. A `/GL` function's codegen depends on the whole program, and ours is missing most of the engine, so prove one against an `/FA` listing built without `/GL` rather than against the linked image.

Within a library, App uses subdirectories (`util`, `v8world`, `v8tree`, `v8kernel`, `v8datamodel`, `v8xml`, `gui`, `humanoid`, `reflection`, `script`, `security`, `tool`) with headers under `include/`. AppDraw, RbxView, RenderLib and the WebService project are flat, and each library's headers live under `include/<libname>/`.

`util/decomp.cpp` exists so the DLL target has a source file while everything else is empty. Keep it out of the LTCG list; it is ours, not theirs.

`OPENRBX_KEEP_UNREFERENCED` links with `/OPT:NOREF` so reccmp can see functions nothing calls yet. The original used `/OPT:REF`. Turn it off once enough of the DLL is reachable.

## The DLL

`WebService/WebService.cpp` is the ATL Server ISAPI surface. `_AtlModule` is a `CDllMainOverride` and `theExtension` a `CRbxIsapiExtension`; both names come from the publics, not from the wizard's defaults. It needs `_WIN32_WINNT 0x0400` for `CWorkerThread::AddTimer` and `_WIN32_DCOM` for `CoInitializeEx`, or `atlisapi.h` will not compile.

`_ATL_NO_COM_SUPPORT` has to be a `target_compile_definitions` entry and never a `#define` in one source. The macro changes ATL's inline bodies, so defining it in one file leaves the target's other translation units disagreeing and the linker keeps their copies.

`WebService/WebService.def` names the eight exports the linker cannot infer. The three handler exports come from two `_HANDLER_ENTRY` records and appear on their own once `DECLARE_REQUEST_HANDLER` registers a concrete handler.

`WebService/WebService.h` is the service's own type system, all of it out of the type records. `WebService/WebServiceMaps.cpp` is the attribute provider's `_soapmap` tables written back out, generated from `.rdata` under the original's own symbol names rather than typed by hand; `reccmp-datacmp` compares all 69 against the original. `_soapmap` is fifteen dwords and `_soapmapentry` fourteen, not fifteen, and getting that stride wrong reads garbage that still looks plausible.

Attributed ATL is not available in MSVC800-SP1. `atlprov.dll` ships in `VC/bin` and loads, and the compiler's own attributes (`module`, `uuid`, `object`) work, but no provider-supplied attribute resolves: `soap_handler`, `request_handler` and OLE DB's `db_source` all fail with C2337. Registering the provider's coclasses changes nothing, so the lookup is not the ProgID. Whatever is missing belongs in the toolchain repository. Until it is found, the maps stay generated from the tables.

Smoke test the result by loading it: the DLL has to map at 0x10000000, resolve every export, and answer `GetExtensionVersion` with 6.0 and `"ROBLOX Web Service"`. That call runs ATL's whole startup, the thread pool and the DLL, file and stencil caches, so it is worth more than a load. The loader must be built 32-bit.

## Reflection and Registration

Every registered class hangs off one chain: `X` to `DescribedCreatable<X, Base, sX>` to `Described<X, sX, FactoryProduct<X, Base, sX>>` to `FactoryProduct<X, Base, sX>` to `Base`. None of the three templates adds a byte. They live in `include/util/object.h`, `include/reflection/reflection.h` and `include/v8tree/Instance.h`, and each class's copies are emitted from its own `.cpp`.

- **Wire a class from the type records and the contribution table,** rebasing one the tree already declares rather than duplicating it. A class whose base is not declared yet cannot be wired at all. One base unlocks a family at a time.
- **`Described<T, sName, Base>::classDescriptor` names its local static `foo`,** in all 98 instantiations. The name reaches the mangling of the static, of its guard and of its atexit destructor, so getting it wrong costs every one of those three a pairing.
- **`TypedPropertyDescriptor<T>::GetSet` has no virtual destructor,** and its first slot is `isReadOnly`, which the descriptor forwards to. Declaring the destructor turns every `dynamic atexit destructor for 'prop_X'` into a null test, a virtual deleting call and an SEH frame where the original spends one `operator delete`.
- **`PropDescriptor` builds its `GetSet` in a static `getset` helper,** not inline in the constructor's initializer list. Inlining it costs the constructor its whole frame and reads as a sixty point diff.
- **The chain's namerefs live in one block per header** and a generator has to rewrite the whole block, so a class it skips still has to be counted or its nameref is dropped. Write a nameref only where the address already reaches 100%.
- **Both `NonFactoryProduct` and `FactoryProduct` declare `getClassName`,** so the nameref block replaces the first only or every annotation lands on two addresses and decomplint reports `duplicate_offset`.
- **A class whose placeholder members sit where its real base's data lives cannot just be rebased.** Move the members to the base that owns them first.
- **Five of the six events a `Notifier` carries are `struct`, not `class`.** The mangling records which, and one wrong keyword misses every construction vftable and `Notifier` name that mentions them.
- **`FactoryProduct` and `DescribedCreatable` both declare a protected virtual destructor,** which the type records state. Leaving it implicit means no vftable stores, every instantiation compiles to the same body, and they fold onto two addresses. `DescribedCreatable` must not declare one of its own: the original's is a five-byte tail jump into `~FactoryProduct`.
- **`Creatable<T>::operator new` and `operator delete` are `malloc` and `free`,** private in the mangling and inlined at every site. The original never imports the CRT's `operator delete` at all. Both have to be protected here, because modern conformance enforces that a virtual destructor can reach its deallocation function where MSVC 8 does not.
- **A `ClassDescriptor` static hands itself to atexit,** and that destructor is a real function per instantiation whose name cvdump truncates. Build it by hand and take its address from the `push` the `call` follows; anchor on the pair, since a bare `0x68` scan lands inside another instruction.
- **A property descriptor is a `const` static,** so it mangles with a trailing `B`. Ours ending in `A` means no setter can bind to it.

## Third Party

Everything is vendored under `3rdparty/`. RakNet is the exception, reconstructed rather than vendored, because the 3.0 release RBXGS used was never archived and the closest archived drop still differs in nine files.

Two dependencies are easy to miss because they were compiled into someone else's `.lib` instead of being linked: Lua 5.1.1 went into app.lib as 25 translation units, and zlib, libpng and IJG jpeg arrive inside G3D as `g3d/zlib`, `g3d/png` and `g3d/ijg`.

`RakPeer`'s vtable annotation needs a commented declaration under it. `class_decl_regex` takes the first identifier after `class`, so `class RAK_DLL_EXPORT RakPeer` reads as a class named `RAK_DLL_EXPORT`; a `// class RakPeer` line under the marker names it. Constness is what stops RakNet slots pairing: the original spells `char*` as `const char*` across `Connect`, `Send`, the RPC registration methods and all four `RPCMap` identifier methods. Compare `?Name@RakPeer@@` across both PDBs to find the rest.

## Gotchas

- **`reccmp-reccmp --verbose` dies printing a match.** The success line carries an emoji and a cp1252 console cannot encode it, so a 100% match looks like a traceback. Export `PYTHONIOENCODING=utf-8`.

- **`.clang-format` sets `IncludeBlocks: Regroup`,** so it merges include blocks and sorts across them. A blank line will not hold an include in place. `WebService.h` needs `<winsock2.h>` ahead of `<unknwn.h>`, and only `// clang-format off` around it survives.
- **Never sweep timestamps across the working tree.** `core.autocrlf` is true and `.gitattributes` marks `3rdparty/**` as `-text`, so git stores those files byte for byte from disk. Touching their mtimes forces a re-stat, and a `git add -A` then rewrites every one of them to whatever line ending the working copy happens to hold. The CI workflow's backdate step does exactly this, which is fine on a throwaway runner and destructive in a clone.
- **Data past the end of `.data`'s raw bytes is fine;** reccmp zero fills it, as the loader does. An empty base class is what puts a member above offset zero.

## Project Structure

```
common/app/          # App static library, subdirectories by subsystem
  include/           # Headers, mirroring the source subdirectories
common/appdraw/      # AppDraw
common/network/      # Network
common/rbxview/      # RbxView
common/renderlib/    # RenderLib
common/rbxgraphics/  # Headers only; its code was compiled into AppDraw and RenderLib
common/win/          # VersionInfo
WebService/          # The DLL project, plus library_msvc.h and WebServiceMaps.cpp
util/                # decomp.h, decomp.cpp, compat.h
3rdparty/            # Vendored dependencies, RakNet reconstructed
reccmp/              # The six CSVs reccmp reads for the original's side
cmake/               # reccmp CMake integration
tools/               # check_folded.py, reccmp_addr_padding.py, ncc
```
