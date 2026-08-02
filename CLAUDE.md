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

`reccmp-cvdump -m -seccontrib` maps any address to the object that contributed it. That is the only way to place a global whose module the symbols do not record: a namespace-scope variable reaches the PDB as an `S_PUB32` with no module, and only the contribution table says which translation unit defined it. Pair it with that module's `src =` and both the file and its casing are recovered.

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

## Class Names

Every registered class carries a namespace-scope char array holding its name, and `Name::doDeclare<&sFoo>` turns it into the interned `Name`. 105 of them, each with a `callDoDeclare<&sFoo>` beside it that is a five-byte tail jump into `doDeclare`. Both templates take `const char*`: 48 arrays are `const char[]` and 46 are `char[]`, the mangling records which (`QBDB` against `PADA`), and one template accepts both through the qualification conversion.

The array's own translation unit comes from the contribution table, the instantiation's from the `S_GPROC32`'s module, and the two are usually different objects: `factoryregistration.cpp` instantiates 27 whose arrays are spread over as many files. Getting that wrong shows up as a `/GS` cookie, because App is `/GS-` and the WebService project is not.

`GlobalSettingsItem<T, &sName>` takes the array as its second parameter, not a pointer to the instance.

## Reflection Descriptors

`Descriptor` (8) to `Type` (16) to `EnumDescriptor` (0x28) to `EnumDesc<T>` (0x98), each recorded in the type records. `Descriptor` holds `const Name& name` at 4 under a vfptr, and its constructor takes a `const char*` and stores `Name::declare(name, -1)`; it never reached the publics, so it is inlined everywhere and belongs in the header. `Type` adds `const std::type_info& type` and `const Name& tag`, and its two-argument form fills `tag` with `Name::lookup(name)`, not with its own name. `EnumDescriptor` passes the literal `"token"` as that tag.

`Descriptor` and `Type` have byte-identical scalar deleting destructors that the original did not fold and ours does; the contribution table puts them in `ThumbnailGenerator.obj` and `Surfaces.obj`, one LTCG and one not. Only one of the two can hold the match, so the other's vtable slot read as missing against code that is the same. reccmp now compares the code when the slot's original function is unclaimed, which is the mirror of `FOLDED` and the general answer wherever the original kept apart what we merge.

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
5. A stub is one store, and App builds `/Ob2`, so the caller inlines it and loses whatever stack the real call needed. `DECOMP_NOINLINE` on the stub keeps the call the caller's own match depends on; `Instance::raisePropertyChanged` is the one that matters, because every property setter tail-jumps to it. Drop the marker once the body is real.
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

## The DLL

`WebService/WebService.cpp` is the ATL Server ISAPI surface: `_AtlModule` is a `CDllMainOverride`, `theExtension` a `CRbxIsapiExtension`, and both names come from the publics, not from the wizard's defaults. It needs `_WIN32_WINNT 0x0400` for `CWorkerThread::AddTimer` and `_WIN32_DCOM` for `CoInitializeEx`, or `atlisapi.h` will not compile.

`WebService/WebService.def` names the eight exports the linker cannot infer. The other three the original has, `_InitializeAtlHandlers@8` and its two siblings, are `__declspec(dllexport)` inside ATL's `HANDLER_ENTRY` macro and appear on their own once a request handler is declared.

Smoke test the result by loading it: the DLL has to map at 0x10000000, resolve every export, and answer `GetExtensionVersion` with 6.0 and `"ROBLOX Web Service"`. That call runs ATL's whole startup, the thread pool and the DLL, file and stencil caches, so it is worth more than a load. The loader must be built 32-bit, so a 64-bit shell cannot run it.

The three handler exports come from two `_HANDLER_ENTRY` records the original keeps at 0x102f4fdc and 0x102f4fec: `"Default"` for `Roblox::CWebService` and `"WSDL"` for `ATL::CSDLGenerator<Roblox::CWebService, s_szClassNameWSDL34>`, where that name is the char array `"Default"`. `DECLARE_REQUEST_HANDLER` is the macro; the generator's comma needs a typedef first or the macro takes it as a third argument. `CWebService` derives from `Roblox::IWebService` at 0 and `ATL::CSoapHandler<CWebService>` at 4, is 384 bytes, and holds `errorMessage` at 0x17c with `jobs` and `sync` static. A skeleton compiles as far as the `"Default"` entry; `CSDLGenerator` then needs the SOAP function map, so the exports wait on the `_soapmap` tables the attribute provider generated. `CWebService::GetNamespaceUri` returns `urn:Roblox` and `GetServiceName` returns `Service`, which is the `soap_handler` attribute's argument list.

The maps themselves are plain data in `.rdata` and can be read out instead of regenerated. `___Roblox_CWebService_funcs` at 0x102f5000 holds fifteen `_soapmap` pointers, one per SOAP method: `HelloWorld`, `GetVersion`, `GetStatus`, `Update`, `OpenJob`, `TouchJob`, `Execute`, `CloseJob`, `BatchJob`, `GetTimeout`, `CloseOrphanedJobs`, `CloseAllJobs`, `CloseTimedoutJobs`, `GetAllJobs`, `GetStandardOutMessages`. `___Roblox_CWebService_headers` at 0x102f5040 mirrors it, every entry empty but for the `errorMessage` slot `GetHeaderValue` returns. `_soapmap` is fifteen dwords and `_soapmapentry` fourteen, not fifteen; getting that stride wrong reads garbage that still looks plausible. The chained maps are what explain `LuaValue1` through `LuaValue10`: a Lua table is recursive, so the provider unrolled it ten levels deep, each level chaining to the next and bottoming out at `LuaType`.

`WebService/WebServiceMaps.cpp` is those maps written back out, generated rather than typed: the scratchpad's soapgen reads them from `.rdata` and emits the 24 entry arrays and 46 maps under the original's own symbol names. `reccmp-datacmp` then compares all 69 against the original and every field matches except the `const WCHAR*` members, which it reads as narrow on the original's side and reports as different; that is what validates the reconstruction, and it is why those symbols are the one thing `gen_globals.py` leaves out. With the maps in place `CWebService` is concrete, `DECLARE_REQUEST_HANDLER("Default", ...)` registers, and the three handler exports appear on their own.

`WebService/WebService.h` is the service's own type system, all of it out of the type records: `Status`, `Strings`, `ScriptExecution`, `StandardOutMessage`, the `LuaType` and `MessageType` enums, and the `LuaValue1` to `LuaValue10` chain that ends at ten because `LuaValue10` drops the `table` member. `IWebService` is fifteen `HRESULT __stdcall` methods at vtable slots 0x0c to 0x44; the mangled names in the publics give every signature. `CallFunction` unpacks six parameter blocks whose layouts the map entry offsets describe, and it matching at 100 percent is what proves the vtable order, the block layouts and the argument order together.

Attributed ATL is not available in MSVC800-SP1. `atlprov.dll` ships in `VC/bin` and loads, and the compiler's own attributes (`module`, `uuid`, `object`) work, but no provider-supplied attribute resolves: `soap_handler`, `request_handler` and OLE DB's `db_source` all fail with C2337 attribute not found. Registering the provider's two coclasses under `HKCU\Software\Classes`, in both registry views, changes nothing, so the lookup is not the ProgID. Whatever is missing belongs in the toolchain repository, not here; until it is found the SOAP map has to be written by hand from the `___Roblox_*_entries` tables.

## Third Party

Everything is vendored under `3rdparty/`. RakNet is the exception that is reconstructed rather than vendored, because the 3.0 release RBXGS used was never archived; the closest archived drop still differs in nine files.

Two dependencies are easy to miss because they were compiled into someone else's `.lib` instead of being linked. Lua 5.1.1 went into app.lib as 25 translation units, and zlib, libpng and IJG jpeg arrive inside G3D as `g3d/zlib`, `g3d/png` and `g3d/ijg`.

## MSVC 8.0 Codegen Patterns

Each of these cost a round trip. Keep the sources clean and leave the reasoning here.

A stub that ends `return someOtherStub();` folds onto that other stub. The callee inlines, its store overwrites the caller's, and the caller's `STUB()` constant is eliminated as dead, leaving identical bodies. `Name::declare`, both `lookup` overloads and `getNullName` all shared one address until the three returned the null reference directly, and until they did, every call into `Name` from a matched function read as unresolved.

The engine's own `Notifier` members cannot be claimed while StandardOut's are. `/OPT:ICF` folds `Notifier<Instance,ChildAdded>::addListener` and its five siblings onto one address, which is right, but it folds `Notifier<StandardOut,StandardOutMessage>::addListener` onto the same address, which the original kept separate because it comes out of the LTCG `WebService.obj`. One recompiled address carries one match, so the two sets compete and the annotation only binds to whichever alias the PDB wrote last.

A `const float` at namespace scope is emitted as a real global and referenced by name, where the binary reads an anonymous constant out of the float pool. Write the literal at each use.

A dense switch lowers to descending `sub eax,N / je`. An if/else chain emits repeated `cmp` against each value. They are not interchangeable. Ghidra prints both as `if` chains when the cases are sparse, so the bytes are the only witness: `getJointKMultiplier` has six nested dispatches and every one of them is a switch, which took it from 43 to 90 percent.

Locals are pushed in declaration order, so getting that order wrong leaves an extra value on the x87 stack and shifts every later `st(n)` operand. It reads as a large diff for a one-line cause.

Building a return value through a constructor evaluates every argument before storing anything. Assigning members one at a time interleaves the arithmetic with the stores. Chained assignment (`a = b = c = value`) keeps the value in a register across the stores and writes them right to left; separate statements reload it each time.

How you spell the return path decides register assignment. `if (cond) return x; return y;` makes `x` the fall-through value; assigning to one named result and returning it keeps `y` in place.

Abstract bases carry `__declspec(novtable)`. Without it the constructor stores a vftable the binary never stores, and any derived destructor gains a base vftable store plus a whole SEH frame to unwind the base through, which reads as a fifty percent diff. `Edge` and `IPipelined` both need it. Do not delete the virtuals to force the number.

A virtual destructor with no standalone function in the PDB was inlined everywhere. Define it inline in the header, without an annotation. Out of line in the `.cpp` it cannot inline across translation units and every derived destructor calls it.

boost's `shared_ptr::operator<` compares ownership, not the pointer, so it loads offset 4, not 0.

`a > b` and `!(a <= b)` are not the same float compare. The direct `>` emits the unordered-safe `test ah,5 / jp`; the negated `<=` emits `test ah,1 / je`. Both `Math::fuzzyEq` overloads hold at 95 percent until the negated form is written.

`std::swap(a, b)` loads `a` into st(0) first, so the argument order is recoverable: the member the binary loads first is the one written first.

Which side of a float compare gets loaded first depends on the operator, not the operands: `<` and `<=` load the right operand into st(0), `>` and `>=` load the left. That fixes the spelling before the jump condition is even worth looking at, and it is why `Extents::contains` reads `!(point.x < low.x)` rather than any of the three equivalent ways to say it.

MSVC reorders the terms of an `&&` chain of float compares. `Extents::overlapsOrTouches` tests its second triple in the order y, x, z, and the source has to be written that way to match.

`Math` is a class of static members, not a namespace, and the difference is visible: its magic-static guard mangles `??_B?1??inf@Math@RBX@@SAABMXZ@51` and reccmp can name it, where a namespace function's guard is `$S` and anonymous. That one symbol took `Extents::express` and `toWorldSpace` from 32 and 35 percent to 100. A guard defined in a `.cpp` stays anonymous whatever the scope, which is the ceiling on `Extents::zero` and `negativeInfiniteExtents`.

A `static const float` folds to a constant and loses its magic-static guard unless the initializer calls something. The binary's guard on `Math::rotationFromByte` is what proves the step came from `G3D::pi()` rather than a literal.

A `ComputeProp` member records its owner's inheritance shape. MSVC sizes the pointer-to-member by how the class inherits, so `ComputeProp<float, Primitive>` is 16 bytes and `ComputeProp<float, Assembly>` is 24: `Assembly` also derives from `boost::noncopyable`, which makes the member pointer the multiple-inheritance form. A class size that is off by the difference means a base is missing.

Assigning a call's result to a local before comparing it changes the evaluation order. `f() != g()` runs `g()` first, per the right-operand rule; `x = f(); return x != g();` runs `f()` first. `Ball::hitTest` only matched with the call written inside the comparison.

Comparing two `bool`s is a byte compare. Writing one of them inline as a negation, `flag != !other`, promotes both to int and costs a `movzx`. Store the negation in its own `bool` first.

A recursive helper and the loop it compiles to are not interchangeable in the source. MSVC turns tail recursion into a loop at the definition, so both spellings match there, but a caller inlines exactly one level of the recursion and none of the loop. `onPrimitivesChanged` and `findNextRelative` both had to be written recursively before `setParent` and `EdgeIterator::begin` would match.

`x = a; if (cond) x = b;` and `x = cond ? b : a;` are not the same codegen. The if form folds into one load and a conditional reload; the ternary evaluates `a` into a scratch register first and copies it in the else branch, which is one instruction longer. Roblox wrote the ternary for the "pick the other primitive on this edge" idiom, so `heavyParent`, `findParent` and `findFirstChild` only reach 100% spelled that way.

Read a CRT call's name out of the publics, never infer it from what the surrounding code looks like it should do. `_floor` at 0x101e8ac4 spent a long stretch written as `sqrt` because a size clamp reads like one, and the wrong one still matched three quarters of the body.

A `__int64` multiply lowers to `_allmul`, and the conversion back to float tells you the signedness: a plain `fild` is signed, and a pair of `fild`s that splits the sign bit out and subtracts it is unsigned.

A comparison puts its right operand in the register: `a == b` on integers is `mov eax, b` then `cmp [a], eax`, and on floats `fld b` then `fcomp a`. So the operand the binary loads first is the one written second, which settles whether a member `operator==` was called as `x == y` or `y == x`.

Most of what is left in the engine is template instantiations, not one-off functions: `scratchpad/enginesurvey.py` counts 7,649 unannotated and the largest owners are `Reflection::EnumPropDescriptor` at 97, `Name::doDeclare` and `Name::callDoDeclare` at 86 each, `Creatable<Instance>::create` at 62 and `Reflection::BoundFuncDesc` at 59. Every `doDeclare` instantiation is the same body, 90 of them 0x65 bytes and 15 of them 0x74, so one template written right settles all of them. Four things have to line up: the name globals are `char sWorkspace[]` arrays, which mangle `PADA` exactly as a `char*` does, so the parameter is `template <char* name>` and the body passes `name` straight to `declare`; both templates are private, or the guard and the local static mangle `SA` where the original has `CA` and stop matching; the annotation is a `// TEMPLATE:` with a nameref on the template body in the header, since an explicit instantiation carries no line records; and the instantiation has to come from a WebService project source, because the original's copies come out of `ThumbnailGenerator.obj` and carry a `/GS` cookie that `app`'s `/GS-` does not.

A template instantiation belongs to whichever object the linker kept it from, and that decides its flags. `Notifier<StandardOut,StandardOutMessage>::addListener` and `removeListener` come out of `WebService.obj` in the original, so they are LTCG where ours are not: the original keeps a frame pointer and aligns the stack, and no source spelling closes that. `raise` and all of `StandardOut` come out of `standardout.obj` and do match.

The declaring namespace of a template member sits after its arguments in the mangling, so splitting on the first `@@` reads `boost::shared_ptr<RBX::Instance>::~shared_ptr` as Roblox's. Since a folded address only counts as vendored when every alias does, one misread name disqualified the whole address. Matching `@boost@@` and friends immediately before the access code fixes it and does not catch a vendored type used only as a parameter, which is always followed by another `@`. That took the vendored table from 2,915 names to 5,642.

WebService.cpp is one of the six `/GL` objects, so its codegen happens at link time and depends on the whole program. Ours is missing most of the engine, so register assignment can differ from the original's on a function whose source is right. `GetVersion` is the case to remember: compile the same file with `/FA` and no `/GL` and its `CStringA` construction is the original's instruction for instruction, but through LTCG the `c_str()` pointer lands in `ecx` and the object in `eax`, the reverse of what the original does. Prove a `/GL` function against the `/FA` listing, not against the linked image.

Data past the end of `.data`'s raw bytes is fine; reccmp zero fills it, which is what the loader does. What failed on `jobs` was reccmp reducing a type to scalars and leaving the bytes before the first one uncovered, so a 12 byte `std::map` produced an 8 byte format string and every unpack of it raised. An empty base class is what puts a member above offset zero. `TerminateExtension` matches once `jobs` is in the table, and `GetVersion` is left with only the LTCG register assignment.

Calls into vendored libraries used to read low, because reccmp only parses the recompiled PDB and had no name for the original's side. `reccmp/webservice-vendored.csv` hands it the 2,594 G3D, Boost, STL and ATL functions out of `WebService.pdb`, which reccmp then matches by mangled name; `--nolib` keeps them out of the score. Selection is by the scope a name is declared in, not the whole mangled string, or a Roblox method taking an `ATL::CStringT` counts as ATL's. Two cases are still unnamed: an address where the linker folded two vendored functions keeps one symbol in our PDB and may keep the other in theirs, and a function-local static mangles its enclosing scope's index, which the two builds number differently.

A static inside a free function reaches the PDB only as an `S_LDATA32` nested in the enclosing `S_GPROC32`; it has no entry in the publics. `gen_globals.py` walks those too, which is what took `normalIdToVector3` from 65 to 86 percent and `Extents::zero` from 80 to 96. The magic-static guard beside it is named `$S<n>`, numbered per translation unit, and the original's compiler emitted no symbol for one at all. Naming ours put a name against an unnamed original and every load of a function-local static read as a difference; reccmp now leaves `$S` unnamed so both sides fall to the same positional placeholder, which is what they are. `normalIdToVector3`, `Extents::zero`, `negativeInfiniteExtents`, `StandardOut::singleton` and three of `Math`'s matrix builders all reach 100 percent on that alone.

`webservice-statics.csv` matches by bare name, so a name under three characters stays out of it. A name that two translation units both use stays out for the same reason, and that is the ceiling on `Motor` and `VelocityMotor`'s setters, which share `prop_MaxVelocity` and `prop_CurrentAngle`: identical instruction streams, 92 percent. `Decal`'s four descriptors are worse off still, carrying no symbol anywhere in the original. Body's state counter is a file-scope `p`, and letting it through paired our `int` with somebody else's `double` and failed `reccmp-datacmp`. The two functions that read it hold at 62 percent on identical instruction streams; that is the cheaper price.

`reccmp/webservice-synthetic.csv` claims functions the source cannot annotate. A CSV row typed `synthetic` asserts a match exactly as a `// FUNCTION:` annotation does, and it needs no source line, so it reaches a compiler-generated function whose class has no header to hold a nameref. `scratchpad/claimable.py` builds it: every decorated name our PDB and the original share, minus the vendored scopes and minus whatever is already annotated, was 159 functions. It writes them all, because naming a callee is what lets its callers resolve, and types only the exact ones `synthetic`; the rest are `library`, which `--nolib` keeps out of the score. Claiming all 159 as `synthetic` instead reads 728 at 100 percent but drops the accuracy to 98.85, since the 124 that are not exact then count.

A vendored function from a prebuilt library has no `S_GPROC32`, only a public, so there is no size for its row. reccmp reads a blank size as unknown and still names the call, but handing it all 2,647 of them lets `reccmp-vtable` pair ATL and STL tables whose entries it cannot resolve and 54 fail. They go in one at a time, in `UNSIZED_KEEP`; `boost::call_once` is the first. Choosing which name a folded address carries is `min()`, not first seen, or `??_E` and `??_G` swap between runs and `G3D::Sphere`'s vtable stops matching.

reccmp is `Asphaltian/reccmp@msvc800`, a fork, and `tools/requirements.txt` pins that branch. Two of its assumptions predate MSVC 8: `S_FRAMECOOKIE`, `S_CALLSITEINFO`, `S_EXPORT`, `S_SECTION` and `S_COFFGROUP` were unknown symbol records, and `T_BOOL08` was unverified. Both are cosmetic, since unknown records are skipped before nodes are built. The one that cost real measurement is not MSVC's: reccmp paired a PDB source path to a local file on filename alone, so G3D's `Log.cpp`, which sits outside the source roots, resolved onto `util/Log.cpp` and merged its line records in. Two function starts then fell in one annotation's line range, the annotation was dropped, and `Log::writeEntry` and `Log::formatMem` reported nothing at all.

`reccmp/webservice-strings.csv` types the original's 3,569 string literals. MSVC records the width in the mangling, `??_C@_0` narrow and `??_C@_1` wide, and without it reccmp reads all 145 wide ones as narrow and every literal push diffs.

A function-local static never matches by its mangled name. reccmp reads ours out of the `S_LDATA32` nested in the enclosing `S_GPROC32`, which carries only the bare name, and labels it `<variable>___<enclosing function>`. `webservice-globals.csv` hands the original the same label. The magic-static guard beside it does match by symbol, and so does the `dynamic atexit destructor` the compiler emits when the static has one, but only once a `// SYNTHETIC:` nameref claims it. Those two kinds are the exception to the rule that vendored globals stay out of the table, because a singleton like `G3D::Vector3::zero` is read straight from Roblox code and is data, which reccmp cannot match a vendored function through. A vendored vftable is the third: `G3D::ReferenceCountedObject`'s is stored by every Roblox constructor and destructor below it, and we never declare the class, so no `// VTABLE:` annotation can name it.

The LINES section names only the files that contributed line records, so a header full of declarations is missing from it. The PDB's raw string table is not: it holds 2,001 Roblox source paths, and `scratchpad/pdbpaths.py` reads them straight out of the file. That is the authority on whether a header exists, and `include\util\log.h` is in it even though no module's line info mentions it. Every path there is lowercased, so it settles existence and never casing. Casing comes from `src =` alone, it covers `.cpp` files only, and Roblox's own is not uniform: `.\util\Log.cpp` sits beside `.\util\standardout.cpp` and `.\util\boost.cpp`.

`StandardOutLog` has no `// VTABLE:` annotation. Its scalar deleting destructor at 0x1000d340 needs a `// SYNTHETIC:` nameref for `reccmp-vtable` to pair slot 1, a nameref in a `.cpp` fails `reccmp-decomplint --warnfail`, and the LINES table puts the class in `webservice.cpp`, so there is no header to put it in.

## Gotchas

A property descriptor is a `const` static, so it mangles with a trailing `B`. Ours ended in `A` and no setter could bind to it. `tools/ncc/ncc.style` carries a `prop_`/`event_` alternative for the same reason: those names are the original's convention, not one-offs for `skip.yml`.

`build.yml`'s "Current MSVC" job builds `app` and `network` with a modern cl plus clang-tidy, and nothing local reproduces it. It is the only check that sees C++ conformance MSVC 8 lets through: a `const` object of class type needs a user-provided default constructor on the most-derived class, not just on a base, which is why every descriptor template carries an empty one.

`reccmp-decomplint` runs over `WebService common` only, so nothing under `3rdparty` is linted and a bad marker there fails silently. `DataBlockEncryptor::~DataBlockEncryptor` had its body written `{}` on one line, which leaves the parser inside the function, and the next marker was reported `unexpected_marker` and dropped: `IsKeySet` could not be matched at all and both `ReliabilityLayer` functions calling it were stuck a call short. Splitting the braces fixed all three. Point decomplint at `3rdparty/RakNet30` by hand after touching RakNet; it cannot join CI until RakNet's 56 `function_out_of_order` findings are worked through.

`.github/workflows/build.yml` runs eight checks and seven of them are not `reccmp-reccmp`. `tools/ncc/ncc.py` runs on its own Linux job and needs libclang plus the `clang` python bindings to run locally; it counts its findings and still exits 0, so the count is the verdict. `reccmp-decomplint` runs with `--warnfail`, so a warning fails the build: a nameref belongs in a header, never in a `.cpp`. `reccmp-vtable` compares every vtable that has a name on both sides, which is why a vendored vftable only goes into `webservice-globals.csv` one at a time, listed in `VENDORED_VTABLES` — naming them all pairs ATL and STL tables whose entries reccmp cannot resolve. `reccmp-datacmp` compares every named variable, and it matches statics by bare name, so a name has to be unique in both builds; `next` named RakNet's file-scope one and a function-local one our G3D keeps. Run all seven before committing, not just the accuracy number.


`.clang-format` sets `IncludeBlocks: Regroup`, so it merges include blocks and sorts across them; a blank line will not hold an include in place. `WebService.h` needs `<winsock2.h>` ahead of `<unknwn.h>`, which drags in `windows.h` and then `winsock.h`, and only `// clang-format off` around it survives. An incremental build will not catch this, because the object file that breaks is the one that did not need recompiling: delete `build/` before trusting a green result on anything that moves an include.

Never sweep timestamps across the working tree. `core.autocrlf` is true and `.gitattributes` marks `3rdparty/**` as `-text`, so git stores those files byte for byte from disk. Touching their mtimes forces a re-stat, and a `git add -A` then rewrites every one of them to whatever line ending the working copy happens to hold. That is what the CI workflow's backdate step does, which is fine on a throwaway runner and destructive in a clone.

`RakPeer`'s vtable annotation needs a commented declaration under it. `class_decl_regex` takes the first identifier after `class`, so `class RAK_DLL_EXPORT RakPeer` reads as a class named `RAK_DLL_EXPORT`; the regex also accepts a `//` line, so `// class RakPeer` on the next line names it. Both vtables hold 71 slots and always did. What stopped the slots pairing was constness: the original spells `char*` as `const char*` in `Connect`, `Send`, `RegisterAsRemoteProcedureCall`, `RegisterClassMemberRPC`, `UnregisterAsRemoteProcedureCall`, `SendConnectionRequest`, both `RPC` overloads and all four `RPCMap` identifier methods, so every one of those mangled to a name our build never emitted. Compare `?Name@RakPeer@@` across both PDBs to find the rest.

The original link used `/OPT:ICF` and folded heavily. Many distinct source functions compiled to the same three-byte body and share one address, which is also why Ghidra's PDB source line importer throws several hundred `IllegalArgumentException` warnings when the symbols load. Those warnings are expected and harmless. Run `tools/check_folded.py` before concluding that a function at a shared address is genuinely unmatched.

`tools/check_folded.py` diverges from racers there. MSVC 8 keeps a PDB symbol for every alias the linker folded away, so all the `FOLDED` annotations on one address resolve rather than just the survivor. The check asks whether they landed on a single recompiled address instead of counting how many resolved.

`tools/` otherwise holds only what came from racers. Analysis scripts written along the way live in the session scratchpad.
