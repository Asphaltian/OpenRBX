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

Namerefs obey the ascending order too, and getting it wrong is silent: Team's `0x100be5xx` markers placed above `Instance`'s own lower-addressed annotations dropped every annotation after them in that header and took forty functions with it, showing up as `raisePropertyChanged` losing its match rather than as an error.

The colon is required on everything except `// SIZE`. reccmp silently ignores `// VTABLE WEBSERVICE 0x...`, and the vtable stores inside constructors then compare as `<OFFSET>` instead of the vftable symbol, costing five to ten percent.

`// FUNCTION:` asserts a 100% match. Anything less is `// STUB:`.

`FOLDED` marks siblings the linker merged onto one address. They share that address, are exempt from ascending order, and do not need the `STUB()` macro.

## Name

The interning subsystem is `util/Name.cpp` and all nine functions match. `moo2` is a magic-static `boost::mutex` called `mutex2`, `initMoo` a five-byte tail jump into it, and `boost::call_once(initMoo, flag)` guards it at the head of `declare`, `lookup` and `getNullName`. `namMap` returns a `NamMap`, which derives from `std::map<std::string, Name*>`, and `dictionary` a `std::map<int, Name*>`; their statics are named `n` and `d`. `getNullName` is `declare("", 0)` behind a magic static, and `empty` is `this == &getNullName()`.

Three things gate that subsystem and each is general. Both accessors and `initMoo` need `DECOMP_NOINLINE`, or `/Ob2` inlines them and the original's out-of-line calls have nothing to bind to. `boost::mutex::do_lock` and `find` are folded in the original with other instantiations, so the vendored table names them for whichever the `min()` rule picked and our call reads as unresolved: the fix is an extra row at the same address carrying the instantiation our build emits. And the `boost::once_flag` is a file static called `flag`, which `gen_statics.py` drops for not being unique across the image, so it needs naming by hand.

`declare` took two things past the register allocation it looked like. Its insert path writes `dictionary()[dictionaryIndex]` unconditionally, where the found path guards on `-1`, and the extra branch was worth twenty percent. The rest was the string key: an explicit `std::string(name)` temporary pushes the constructor's own return value and an implicit conversion recomputes the address, and the two also rank the enregistration candidates differently, so the original holds the mutex in `ebx` for the whole body under one spelling and in `ebp` under the other. `find` has the explicit temporary and `operator[]` the implicit one; either alone stalls at 79 or 90 percent.

## Guid

`util/Guid.cpp` is complete, all nine functions. `Guid` is 8 bytes over a private `boost::noncopyable`, holding a `Data` of `const Name* scope` and `int index`; `Guid()` takes its scope from `getLocalScope()` and its index from `InterlockedIncrement` on a file-static `nextIndex`. `getLocalScope` holds the `boost::once_flag` as a function-local static, and the constructor reaches the same flag by inlining that call, which is why the flag's only name is the one reccmp builds for a local static. `initLocalScope` is a file static that interns `generateGUID`'s output, and `generateGUID` is `"RBX"` plus `CoCreateGuid` formatted through `StringFromGUID2`, with six `erase` calls in descending order stripping the braces and dashes.

Three lessons came out of it and all three are general. MSVC lays a block out where its statement sits in the source, so a constant return written last ends up last: `operator<` and both `compare` overloads only matched once the rare `return true` and `return -1` were moved below the hot path, which needs the result tested afterwards (`int order = 0; if (...) order = ...; if (order == 0) return ...; return true;`) rather than returned early. Argument evaluation is right to left, so the pair named in the second argument is the pair that gets the callee-saved registers; assigning the first argument to a local ahead of the call is what put `a0` and `a1` in `ebx` and `edi` the way the original has them. And `std::min` returns a reference, which is visible as a `lea` of both candidates and a deref, so `windows.h`'s `min` macro has to be suppressed or the comparison compiles inline and nothing lines up; the argument order is visible too, since the two candidates land in declaration order.

## Assembly

`v8world/Assembly2.cpp` is complete: all 48 of its own functions match, and everything left in the object is vendored STL that `--nolib` scores out.

Two of them turned on the same thing, which is general. A folded address carries one name per build, and `min()` can pick a different one on each side: the original folded three `push_back` instantiations onto 0x10127450 and the vendored table named it for `Listener`, where our build folds `Assembly*` with `EnumDescriptor const*` and picks `PAVAssembly`. The extra row has to carry the name our side picks, not the one reccmp prints in the diff. `resize` at 0x10041ee0 is the same case against the `unsigned int` instantiation.

`fastRemoveShort` in `util/StlExtra.h` is the one to remember for method rather than content. Sixty spellings, the original's own command line and five optimisation levels all left it near 48%, because the byte counts I was steering by came from an `/FA` listing whose relocated `call` and `mov ebp, [__imp_...]` bytes my extractor was dropping; a spelling that was already right read 42 bytes short. The line records settled it in one look: `0001:00102660-00102745` maps to lines 42, 43, 45, 54, 55, 59, 60, 62, 67 and 68 of `stlextra.h`, which is eight code-generating statements where the obvious source has seven, and lines 54 and 55 both feed `last`. It is `items.end()` and then `--last`, not `items.end() - 1`. Count the line records before sweeping spellings, and measure with reccmp on the linked image rather than by eye on a listing.

## Clump

`v8world/Clump2.cpp` is complete, all ten functions: `PrimIterator`'s four search statics, its `isParent` and `operator++`, and `EdgeIterator`'s four.

A ternary materialises its arms into a 32 bit register, so a `bool` function that ends `movzx eax, al` on one path and `mov eax, 1` or `xor eax, eax` on another returned a ternary; a plain `return false` writes the shorter `xor al, al`. `isParent` ends `searchType == IN_CLUMP ? sameClump : sameClump || parentCandidate->getAssembly() == child->getAssembly()`, and the three `if`s that say the same thing held it at 80 percent. Its early exits are one shared `return false` at the end, which the line records give away by sending both the joint test and the body test to the same line.

Where a callee stores its result decides its caller's register allocation, and a function that already matches is still worth respelling when its caller does not. `PrimIterator::operator++` matches at 100 percent either way, but only assigning `primitive` in each branch lets `findEdgeOnNextPrimitive` share one callee saved register between the parent and the merged result; storing a local once after the merge gives that value a home from `findFirstChild`'s return, which overlaps the parent, costs a fourth callee saved register and 33 percent.

An `/FA` listing's byte counts mislead, but its mnemonic stream does not. Aligning the stream against the original's disassembly scored 88.68 where reccmp scored 88.89 and 55.05 where reccmp scored 61.82, which is a compile rather than a link per spelling, and it is what made a sweep of `operator++` shapes affordable. `findEdgeOnNextPrimitive` is three statements: `Primitive* next = *++PrimIterator(primitive, IN_ASSEMBLY);`, then `if ((primitive = next) == NULL)`, then the edge. Incrementing the temporary in the same statement is what keeps the construction, the increment and the dereference on one line record.

## Extents

`util/Extents.cpp` is 15 of 16. `contains` is a direct `point.x >= low.x`, not the negated `!(point.x < low.x)` that reads the same: the direct form emits the unordered-safe `test ah, 0x41 / jp` where the negation emits `je`, which is the same rule as `Math::fuzzyEq`'s but on `>=`. `containedByFrustum` names two locals the loop would otherwise recompute, `const Plane& plane = frustum.faceArray[i].plane` outside the inner loop and `const Vector3 corner = getCorner(j)` inside it, and the line records place both before either was written; without them our build spends a fourth callee saved register and recomputes the plane's address eight times per face.

`Extents::vv`'s line records name `include\util\extents.h` lines 61 to 66, so it belongs in the header rather than the `.cpp` it matches from today.

`getFaceCorners` took a frame size to solve, and both halves of it are general. A `switch` case is not a scope MSVC 8 reuses a temporary in: the same six bodies give a 0x30 frame written at function scope and a 0x120 one written as case bodies, four `Vector3` slots against four per case, and the slot displacements are what the tail merge of the shared fourth `getCorner` call depends on. So the switch only dispatches, `case NORM_X: goto x;`, and the six bodies sit below it as labelled statements. That is the exception to no gotos in Code Style, and the frame size is the evidence for it. How many slots there are is decided by the comma operator: `c0 = getCorner(4), c1 = getCorner(6), c2 = getCorner(7), c3 = getCorner(5);` keeps four temporaries live at once where four statements reuse one, and it is what the line records already said, one record per body and one for its jump.

Two things that looked like the answer were not. An inlined callee also gives four shared slots, since its temporaries are allocated once for every expansion, but it needs `__forceinline` for nine arguments and then `#pragma inline_depth(1)`, because inlining it makes MSVC inline `getCorner` too where the original keeps it out of line; `DECOMP_NOINLINE` on `getCorner` instead costs `express`, `toWorldSpace` and `containedByFrustum` their matches, since the original does inline it into those three. And the toolchain is not the difference, which the hotfixes settle: of the sixteen VS 2005 updates archived at `archive.org/download/vs80sp1-all-langs/sp1-updates`, every one that could hold a compiler postdates 2007-12-20, and KB937061 from July 2007 patches Crystal Reports. RTM 14.00.50727.42, out of `VSPROD1.iso`'s `_195` and `_196` cabs, gives the same 0x120 as SP1, as does every `/O`, `/Zi` and `/Z7` combination.

An inlined callee does produce the original's frame, its interleaved call and copy and all 19 calls, because the callee's temporaries are allocated once and every expansion shares them. Reaching it costs `__forceinline`, since MSVC will not inline nine arguments on its own, and then `#pragma inline_depth(1)`, since inlining the callee makes it inline `getCorner` too, where the original keeps it out of line; `DECOMP_NOINLINE` on `getCorner` instead costs `express`, `toWorldSpace` and `containedByFrustum` their matches, because the original does inline it into those three. Three compiler directives to reconstruct one function is not evidence, so the file carries the plain switch and the marker stays `// STUB:`.

## Joints

`v8world/Joint.cpp`, `MotorJoint.cpp` and `RigidJoint.cpp` are complete, and so are their headers. What unlocked the first two is `IPipelined::getWorld`, which the type records name along with `IWorldStage::getWorld` and `RevoluteLink::setJointAngle`, so none of the three had to be invented; a byte search for `call edx` followed by `cmp eax, 8` found the six sites that inline it and put it on `IPipelined` rather than `Joint`, since two of them are `BallBallContact` and `BallBlockContact::stepContact`.

Three spellings decided it and each is general. `getWorld` reads `currentStage` three times rather than naming it, so the condition has to test the member and the local has to be assigned in both arms of an if/else: naming it first keeps it in a callee saved register across the virtual call where the original reloads it, which is worth 15 percent to every caller. `Edge::getPrimitive` is `(&prim0)[index]`, not `index == 0 ? prim0 : prim1`, which only shows up once a caller passes a variable index and wants `[esi+ebx*4+0xc]`. And `MotorJoint::setCurrentAngle` compares `currentAngle != value`, not the other way round: the original loads `value` into st(0) and compares against the member in memory with `fcom`, where the reversed spelling loads both and needs `fucom`.

`MotorJoint::resetLink` picks its two coordinate frames with a pair of ternaries in one call, `link->reset(index == 0 ? jointCoord0 : jointCoord1, index == 1 ? jointCoord0 : jointCoord1)`. A `switch` compiles the dispatch as descending `sub eax, N / je` where the original compares against 1 and then 0, and an if/else gets the compares right but duplicates the argument pushes the original shares. Arguments evaluate right to left, so the second one carries the `== 1` test that the original performs first.

`World::onMotorAngleChanged` folds onto `onPrimitiveCanCollideChanged` at 0x100cf2b0, and `tools/check_folded.py` requires every annotation on an address to agree, so adding the sibling means adding `FOLDED` to the one already there.

## Cofm

`v8kernel/Cofm.cpp` is complete, all five functions, and `updateIfDirty` matched on the first build because the line records fixed its shape before any of it was written: fourteen statements at lines 21 to 38, which is the dirty test, the mass and the running position, a four statement loop over the children, the divide, `cofmInBody`, the world moment, a second loop, `Math::momentToObjectSpace` and the flag. Count the statements first and the body follows.

Everything it needs was recorded rather than guessed. `Body::getBranchMass` is `cofm != NULL ? cofm->getMass() : mass`, which is why the disassembly inlines a call to `Cofm::updateIfDirty` twice per child; `children` is `RBX::IndexArray<RBX::Body,&RBX::Body::getIndex>` at 0x10 over a `G3D::Array<Body*>`, and `include/util/indexarray.h` is in the PDB's string table even though no module's line info mentions it; `cofm` follows at 0x1c. The rest of `Body`'s layout is in the type records too: `root` at 4, `index` at 0xc, `simBody` at 0x20, `canThrottle` at 0x24, `link` at 0x28, `meInParent` at 0x2c and `moment` at 0x5c.

## Rect

`Rect.obj` holds one function and `util/Rect.cpp` matches it, so the object is done. `positionChild` builds its result with `fromLowSize(Vector2(x, y), Vector2(width, height))`, which the type records carry as a static beside a `Rect(const Vector2&, const Vector2&)` constructor. The four float constructor compiles the same arithmetic in the wrong order: it stores `low` with a non-popping `fst` and adds afterwards, where the original computes both sums and then stores all four, which is the rule that building a return value through a constructor evaluates every argument before storing anything. The two switches were already right, and each is a real `switch`, lowered as descending `sub eax, N / jz` over 2, 3, 4 for the x locations and 0, 1, 4 for the y.

## Constants

`v8kernel/Constants.cpp` is complete, all twelve functions. `getJointK` sorts the size, clamps it with `sorted.max(Vector3(1.0f, 1.0f, 1.0f))` and scales `getJointKMultiplier` by 960000; `getKmsMaxJointForce` rounds both stud counts with `G3D::iRound`, clamps each to at least 1, and indexes `MAX_LEGO_JOINT_FORCES_MEASURED`, seven measured forces ending 4.681, by the larger of the two.

Four things settled it and each is general. A bare `fld`/`fistp` with no control word change is round to nearest, not a cast, so it is `G3D::iRound`, which is `lrintf`. The component-wise clamp is `Vector3::max`, recognisable because it builds one `Vector3(1,1,1)` temporary and compares z, y, x in that order rather than three separate constants. `MAX_LEGO_JOINT_FORCES_MEASURED` cannot be `const`: as a constant our build folds `(1.0f / 7.0f) * table[6]` into a single pool entry where the original keeps the two multiplies apart, so the array is written without `const` and named in the statics table. And the ternaries in `getJointKMultiplier` are spelled `value < 15.0f ? a : b`, not `15.0f <= value ? b : a`; the original loads the constant into st(0) and tests `ah, 0x41`, ours loads the value and tests `ah, 1`, which is the same `<` rule that decides `Extents::contains`. Those three ternaries were the whole of the last 9.5 percent.

`tools/ncc/ncc.style` gained an `^[A-Z][A-Z0-9_]*$` alternative for the same reason it carries `prop_` and `event_`: the caps spelling is the original's, recovered from its symbols rather than chosen.

## StandardOut

`util/standardout.cpp` and the `Notifier` template it instantiates are complete. Five things did it and each is general.

`Notifier<Source, Event>::raise(Event, Listener*)` wraps the listener call in a `try`/`catch` that reports through StandardOut: `catch (std::exception& error)` takes `error.what()` into a `std::string` and calls `StandardOut::singleton()->print(MESSAGE_WARNING, "Exception caught in onEvent. %s", ...)`. That accounted for the frame pointer and forty of its seventy-seven instructions, and the line records placed it at events.h 187 to 197 before any of it was written.

`Events.h` cannot include `standardout.h`, which includes it back, and a mid-file include breaks boost's own header ordering. A forward declaration of `class StandardOut` is enough, because MSVC only analyses a template body when it is instantiated, which happens in `standardout.cpp` where the class is complete.

`raise(Event)` needs `throw()`. Without it MSVC emits an unwind funclet to destroy the by-value `Event` parameter and the whole EH frame with it, where the original has no unwind table at all; the PDB shows the two-argument overload carrying `$0` and `$2` funclet symbols and the one-argument overload carrying none. That was worth 47 percent. The last three came from writing the increment inside the subscript, `listeners[range.index++]`, which the line records give away by attributing the increment to the same line as the call.

`StandardOutMessage` sets its time with `_time64(&time)` in the constructor body, not `time(_time64(NULL))` in the initialiser list; the difference is `push <address>` against `push 0` and a pair of stores. And `~StandardOut` is `(compgenx)` in the type records, so the original never declares it: declaring one adds a vftable store to the destructor that the original does not have, which is the same rule as `FactoryProduct`'s but read in the other direction.

One more folded-alias row: the original's `~mutex` call target at 0x101e7700 is named `??1mutex@boost@@QAE@XZ` in the vendored table, but our build folds `~mutex` with `~try_mutex` and resolves ours as the latter, so the address needs both names.

## Class Names

Every registered class carries a namespace-scope char array holding its name, and `Name::doDeclare<&sFoo>` turns it into the interned `Name`. 105 of them, each with a `callDoDeclare<&sFoo>` beside it that is a five-byte tail jump into `doDeclare`. Both templates take `const char*`: 48 arrays are `const char[]` and 46 are `char[]`, the mangling records which (`QBDB` against `PADA`), and one template accepts both through the qualification conversion.

The array's own translation unit comes from the contribution table, the instantiation's from the `S_GPROC32`'s module, and the two are usually different objects: `factoryregistration.cpp` instantiates 27 whose arrays are spread over as many files. Getting that wrong shows up as a `/GS` cookie, because App is `/GS-` and the WebService project is not.

`GlobalSettingsItem<T, &sName>` takes the array as its second parameter, not a pointer to the instance.

## Object and Creatable

Below `Instance`'s registration chain sit three more classes, all in `include/util/object.h` and all 4 bytes: `RBX::Object` carries the vfptr and a public virtual destructor, `Creatable<T>` adds a nested `Deleter` and a private static `operator new` and `operator delete`, and `AbstractFactoryProduct<T>` adds `getCreators` and `create`. Since none of them adds a member, putting them under `AbstractFactoryProduct` leaves `Instance` at 0xf8.

`Creatable<T>::operator delete` is `free(p)`, and that one line is what every scalar deleting destructor in the tree was missing. The original never imports the CRT's `operator delete` at all; `??3@YAXPAX@Z` appears nowhere in the image, because the class-scope one is inlined at every delete site. `operator new` is `malloc`, which is why `create` allocates with `malloc` and not `??2@YAPAXI@Z`. Both are `CA` in the mangling, private, and both have to be protected here: a virtual destructor has to reach its deallocation function and modern conformance enforces that where MSVC 8 does not. Neither reaches our own map, so the original's six-byte out-of-line copy at 0x100078f0 has nothing to annotate.

`Creatable<Instance>::create<U>` is `boost::shared_ptr<U>(new U(), Deleter())` with 108 instantiations, and `FactoryProduct<T, Base, sName>::Creator::create` returns `Creatable<Instance>::create<T>()`. `Deleter::operator()` calls `Instance::predelete` and then deletes, and `predelete` has two private overloads, a member and a static taking `Instance*` that tail jumps into it. Private matters: a public static mangles `SA` where the original has `CA`. Both `predelete` and `~Instance` are inaccessible to `Deleter`, so `Instance` declares it a friend.

`JointInstance` owns `Joint* joint` at 0x108, under an `IRenderable` base at 0xf8 and an empty `IJointOwner` at 0x108. `Motor` and `VelocityMotor` each held a copy of it typed `MotorJoint*`, which is what blocked rebasing them onto `AutoJoint` and `JointInstance`; the setters reach the derived type through a `static_cast`, which costs no instruction under single inheritance.

## Reflection Descriptors

`Descriptor` (8) to `Type` (16) to `EnumDescriptor` (0x28) to `EnumDesc<T>` (0x98), each recorded in the type records. `Descriptor` holds `const Name& name` at 4 under a vfptr, and its constructor takes a `const char*` and stores `Name::declare(name, -1)`; it never reached the publics, so it is inlined everywhere and belongs in the header. `Type` adds `const std::type_info& type` and `const Name& tag`, and its two-argument form fills `tag` with `Name::lookup(name)`, not with its own name. `EnumDescriptor` passes the literal `"token"` as that tag.

`Descriptor` and `Type` have byte-identical scalar deleting destructors that the original did not fold and ours does; the contribution table puts them in `ThumbnailGenerator.obj` and `Surfaces.obj`, one LTCG and one not. Only one of the two can hold the match, so the other's vtable slot read as missing against code that is the same. reccmp now compares the code when the slot's original function is unclaimed, which is the mirror of `FOLDED` and the general answer wherever the original kept apart what we merge.

The property chain hangs off the same root: `Descriptor` (8) to `MemberDescriptor` (0x10) to `PropertyDescriptor` (0x18) to `TypedPropertyDescriptor<T>` (0x1c) to `PropDescriptor<Class, T>` (0x1c), which is 499 functions and the largest family left. `MemberDescriptor` adds `const Name& category` at 8 and `const ClassDescriptor& owner` at 0x0c; `PropertyDescriptor` adds two one-bit fields at 0x10 and `const Type& type` at 0x14, and introduces nine virtuals at vtable slots 4 through 0x20, seven of them pure. `TypedPropertyDescriptor<T>` holds `std::auto_ptr<GetSet> getset` at 0x18 and overrides all but `read`.

Writing that layout is blocked on the descriptors' construction, not on the layout. Both classes hold reference members, so neither can be default constructed, and every `static PropDescriptor<Class, T> prop_Name;` in the tree today is default constructed against the placeholder. The real constructors take the owning `ClassDescriptor`, so that has to come first.

`ClassDescriptor` is 0x88 and is four bases before it is anything else: `Descriptor` at 0 and a `MemberDescriptorContainer` at 8, 44 and 80 for properties, signals and functions, each 0x24 of two vectors and a base pointer. `derivedClasses` follows at 0x74 and `base` at 0x84. It lives in `include/reflection/object.h` beside `reflection/reflection_object.cpp`, and its size assertion is what proves all four bases at once. Its destructor is compiler-generated but has a standalone body in the image, so it is declared and defined out of line rather than left implicit; inline, it folds into the scalar deleting destructor and costs that function nine percent.

The chain above every registered class is `X` to `DescribedCreatable<X, Base, sX>` to `Described<X, sX, FactoryProduct<X, Base, sX>>` to `FactoryProduct<X, Base, sX>` to `Base`, and none of the three templates adds a byte. They live in `include/util/object.h`, `include/reflection/reflection.h` and `include/v8tree/instance.h`, and each class's copies are emitted from its own `.cpp`, so `Team.obj` holds all eight of Team's. Explicit instantiation of the derived class does not reach the bases; each of the three has to be named. `getClassName` is `Name::declare<sName>()`, which is `boost::call_once(&callDoDeclare<name>, flag)` followed by `doDeclare<name>()`. `classDescriptor` is a magic static `ClassDescriptor d(Base::classDescriptor(), sName)`. 36 of the 71 classes have a single base and are writable as skeletons; the other 35 carry extra or virtual bases. `scratchpad/classwrite.py` wires them from the type records and the contribution table: it rebases a class the tree already declares rather than duplicating it, refuses one whose base is not declared yet, and rewrites the whole nameref block in `util/object.h` each run, so a class it skips as already wired still has to count or its nameref is dropped. One base unlocks a family at a time: `CharacterAppearance` carries four classes, `BodyMover` six, `JointInstance` and `AutoJoint` eight between them, and `GlobalSettings::Item` three. `DescribedNonCreatable<T, Base, sName>` is `Described<T, sName, NonFactoryProduct<Base, sName>>`, and `NonFactoryProduct` takes the base class rather than T; `GlobalSettingsItem<T, &sName>` is `DescribedCreatable<T, GlobalSettings::Item, sName>` with `Service` beside it. Both `NonFactoryProduct` and `FactoryProduct` declare `getClassName`, so the nameref block has to replace the first only or every annotation lands on two addresses and decomplint reports `duplicate_offset`.

Everything in the chain except `getClassName` is blocked on one thing, and the constructor says what it is. `FactoryProduct`'s stores eleven vftables, at 0, 0x10, 0x14, 0x20, 0x24, 0x3c, 0x54, 0x6c, 0x84 and 0x9c, which is exactly where `Instance`'s bases sit in the type records. Ours stores at 0, 0xc, 0x10, 0x14, 0x18, so our bases are 12 bytes short of the original's: `GuidItem<Instance>` is 16 and ours is 12, `Reflection::Described<Instance, &sInstance, DescribedBase>` is 12 and ours is 4 because `DescribedBase` carries `const ClassDescriptor& descriptor` at 8 over an 8 byte base of its own. `Instance` still asserted 0xf8 because its members absorbed the difference, so every offset comment in it below `enable_shared_from_this` was 12 too low. Four records put it right: `GuidItem<Instance>` keeps `bool registeredGuid` at 4 and `guid` at 8, `DescribedBase` sits on `SignalSource` and holds `descriptor` at 8, `SignalSource` carries the vfptr itself so the base stays at offset 0, and `Association<Instance>` is a `std::vector<Item*>` at 0x10 rather than a `std::string`. A reference member would stop `Described` default constructing, so `descriptor` stays a sized placeholder with its name in a comment.

That lines the constructors' eleven vftable stores up with the original's, at 0, 0x10, 0x14, 0x20, 0x24, 0x3c, 0x54, 0x6c, 0x84 and 0x9c. The original's construction vftables then have to be named, and naming them used to break `reccmp-vtable`: it compared 307 tables and 281 failed, because a table reaching the database through a data source has no `// VTABLE:` marker claiming its slots. reccmp now records which tables a marker asserts and compares only those, so `gen_globals.py` can hand over the `??_7?$FactoryProduct@`, `??_7?$DescribedCreatable@` and `??_7?$DescribedNonCreatable@` publics without it. That is the general rule: a `// VTABLE:` marker asserts a layout, a data source only resolves a store.

Five of the six events a `Notifier` carries are structs, not classes. The original mangles `ChildAdded`, `ChildRemoved`, `DescendentAdded`, `DescendentRemoving` and `AncestorChanged` with `U` and only `PropertyChanged` with `V`, so every construction vftable and every `Notifier` name mentioning them missed by one character. Declaring them `struct` took `FactoryProduct`'s constructor from 62 to 94 percent, and annotating `Instance::Instance` at 0x1004eb10 finished it.

A label with commas in it has to be quoted in a reccmp data source. `d___RBX::Reflection::Described<RBX::Team,&RBX::sTeam,...>` ends at the first comma otherwise, so the symbol reaching reccmp is a prefix that matches nothing, and 101 rows silently did nothing at all.

`classDescriptor` hands its static to atexit, and that destructor is a real function per instantiation. cvdump truncates its name, so build it as `` `<the classDescriptor name>'::`2'::`dynamic atexit destructor for 'd'' `` and take its address from the `push` that the `call` follows. Anchor on the pair: a bare 0x68 scan lands inside another instruction and yields an address that is not a function start, which reads as 0.00% rather than as an error.

`FactoryProduct` and `DescribedCreatable` both declare a protected virtual destructor, and the type records say so. Leaving it implicit costs more than the destructor: without it MSVC emits no vftable stores, every instantiation compiles to the same body, and 27 of them fold onto two addresses so 25 namerefs have nothing to bind to. The original's is ten stores and a tail jump into the base.

`DescribedCreatable` must not declare one. Its destructor is five bytes in the original, a tail jump into `~FactoryProduct`, and declaring it makes MSVC re-establish all ten vftables instead. `FactoryProduct::Creator::create` is `virtual boost::shared_ptr<Object> create() const`, so a body returning `T*` mangles differently and one returning the same empty pointer for every class folds all 27 onto one address; it needs `Object`, which the tree does not have yet.

`scratchpad/chainrefs.py` writes the rest of the chain's namerefs, one block per header, reading the addresses that already reach 100% so a marker is only ever written where it holds. Three things it has to get right: the scalar deleting destructor is one address shared by `FactoryProduct` and `DescribedCreatable`, so the deduplication spans the files; the block it rewrites has to include `getClassName` or that run deletes what `classwrite.py` wrote; and a nameref clang-format has already wrapped will not match the pattern that removes it, so a stale copy survives and decomplint reports `duplicate_offset`.

A class whose placeholder members sit where its real base's data lives cannot just be rebased. `Motor` keeps `joint` at 0x108, which is inside `JointInstance`, and `Decal`'s padding starts at `Instance`'s size rather than `FaceInstance`'s; both need the members moved to the base that owns them first.

Name the non-type parameter `sName`, not `name`. MSVC 8 rejects forwarding it into `Name::declare` with "expected compile-time constant expression" when an inherited member of the same name is visible, and `Descriptor::name` is visible through every one of these.

`getClassName` matches at 100% for every class wired this way, and the last thing in its way was one operand. A static local to a template instantiation reaches the original's PDB only as a mangled public, because the original inlines the template everywhere; ours keeps the enclosing `S_GPROC32`, so reccmp labels it `flag___RBX::Name::declare<&RBX::sTeam>`. Handing the mangled name over as a symbol does pair it, and costs more than it gives: the long ones collide once reccmp truncates at 255 for C4786, and the accuracy falls to 98.34. Rebuilding reccmp's own label out of the mangling is what works, and `gen_globals.py` does that for the one-argument address-of form, which is what `Name::declare` and `doDeclare` are. 210 statics reach a label that way.

`classDescriptor` holds its `ClassDescriptor` in a function-local static the original's compiler emitted no symbol for at all, so neither the publics nor the nested walk reaches it and no demangling would help. The function reaches it though: it loads the object's address into `ecx` before constructing it, so `gen_globals.py` reads that operand out of the body and pairs it with the enclosing name cvdump already prints, which names 99 of them. Ninety-five of those addresses already carry a name from the publics that does not pair, so the row has to overwrite rather than defer.

clang-format reflows a `//` comment past the column limit, and a nameref is a comment. `Described<RBX::Team,&RBX::sTeam,RBX::FactoryProduct<...> >::classDescriptor` is 118 characters, so it got wrapped, the name line lost its tail, and the annotation silently stopped resolving. Wrap long namerefs in `// clang-format off`.

`rootDescriptor` is written and unannotated: it is inline in the header and nothing calls it until `Described<T, &sName, Base>::classDescriptor` does, which is the next 98 functions and needs `DescribedBase` (12 bytes, `const ClassDescriptor& descriptor` at 8) first.

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
6. Write clean C++, not decompiler pseudocode. No `*(int*)(this + 4)`, no gotos, no raw float bit patterns. `Extents::getFaceCorners` is the one goto, and its frame size is why.
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

Argument evaluation is right to left for operators too, so `(a * b) * c.inverse()` runs `c.inverse()` first where the original runs the left product first. Naming the left product in a local is what orders them: `RigidJoint::align` sat at 46 percent until it read `CoordinateFrame world = prim1->getCoordinateFrame() * coord1; return world * coord0.inverse();`. Naming the receiver has the same effect on scheduling, and is what took `isAligned` from 84: `Primitive* prim0 = getPrimitive(0);` ahead of the multiply hoists the member load above the argument pushes, where the inline call defers it to just before the call.

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

## Sub-100 Functions

`reccmp-reccmp --json` is how to find what is left: type 1 rows that are not stubs and whose `effective` score is below 1.0. Eleven of the nineteen the summary called incomplete carry an effective 100, which reccmp already counts as matched, so the real list was eight. Five of them fell to naming rather than to code.

`_ATL_NO_COM_SUPPORT` is what both DLL registration exports were missing. ATL guards `_AtlComModule.RegisterServer` on it inside `CAtlModuleT::RegisterServer`, and the original calls neither `AtlComModuleRegisterServer` nor `AtlComModuleUnregisterServer`, going straight from the locale save to `_pPerfRegFunc`. It has to be a `target_compile_definitions` entry and never a `#define` in one source: the macro changes ATL's inline bodies, so defining it in `WebService.cpp` alone leaves the target's other translation units disagreeing and the linker keeps their copies, which cost seven of `CWebService`'s one-line inline members their matches. Two blank lines at the top of the same file cost nothing, so line shifts are not the cause. `_pPerfRegFunc`, `_pPerfUnRegFunc` and `_AtlBaseModule` then need naming in `webservice-globals.csv`, and the DLL still loads and answers `GetExtensionVersion`.

A folded address needs a row per alias, on either side. Our build folds `Matrix3`'s copy constructor with its `operator=` and keys the pair on the latter, so `Geometry::getMoment`'s call read as an offset until the vendored table carried `??4Matrix3@G3D@@QAEAAV01@ABV01@@Z` at the constructor's original address.

Two matcher bugs in the fork were behind four of them. `match_variables` indexed function-local statics under their bare name, which is `match_static_variables`' job, so G3D's two `next` locals took RakNet's file-scope one and `randomMT` and `reloadMT` held at 90.91 and 98.92 percent; skipping any recomp entity whose symbol is the `<name>___<function>` decoration fixes both and leaves `reccmp-datacmp` clean. And `create_analysis_strings` recorded the extent of a literal only when it created the entity itself, so a `webservice-strings.csv` row at the base suppressed creation without suppressing the substring: the original grew an entity for `"NOWN]"` inside `"[UNKNOWN]"` that our image, where the scan set the range, never had, and `RakPeer::GetRPCString` read one operand short.

Two are ceilings and neither is a spelling. `CWebService::GetVersion` is the LTCG one: the original puts the `c_str()` pointer in `eax` and the `CStringA` in `ecx`, ours does the reverse, and whole-program codegen decides that. `ReliabilityLayer::HandleSocketReceiveFromConnectedPlayer` differs on two instructions because math.h's `ceil(long double)` forwards to the imported `ceil`, and the out-of-line copy the original's link kept is a bare six-byte tail jump where ours materialises the argument first; casting to `double` instead is worse, because `_CRTIMP` is `dllimport` under `/MD` and the call becomes an indirect one through `__imp__ceil`. Accuracy reads 100.00 with both still in it.

## Gotchas

A property descriptor is a `const` static, so it mangles with a trailing `B`. Ours ended in `A` and no setter could bind to it. `tools/ncc/ncc.style` carries a `prop_`/`event_` alternative for the same reason: those names are the original's convention, not one-offs for `skip.yml`.

`build.yml`'s "Current MSVC" job builds `app` and `network` with a modern cl plus clang-tidy, and it does reproduce locally: VS 18 Community carries the same cl the runner uses, clang-tidy under `VC/Tools/Llvm/x64/bin` and ninja under `Common7/IDE/CommonExtensions/Microsoft/CMake/Ninja`, so `cmake -GNinja -DENABLE_CLANG_TIDY=ON` out of an `amd64_x86` vcvars shell runs the whole job. Run it, because it is the only check that sees C++ conformance MSVC 8 lets through, and it fires whenever a class or template lands. A `const` object of class type needs a user-provided default constructor on the most-derived class, not just on a base, which is why every descriptor template carries an empty one; a virtual destructor has to be able to reach its deallocation function, which is why `Creatable`'s `operator delete` cannot stay private. `SKIP_LINTING` in `CMakeLists.txt` is for the other kind, a diagnostic inside a vendored header: boost 1.34's `scoped_lock` throws and clang-tidy calls that an error whatever `/EHsc` the command line carries, so every source that includes `boost/thread/mutex.hpp` needs a line there.

`reccmp-decomplint` runs over `WebService common` only, so nothing under `3rdparty` is linted and a bad marker there fails silently. `DataBlockEncryptor::~DataBlockEncryptor` had its body written `{}` on one line, which leaves the parser inside the function, and the next marker was reported `unexpected_marker` and dropped: `IsKeySet` could not be matched at all and both `ReliabilityLayer` functions calling it were stuck a call short. Splitting the braces fixed all three. Point decomplint at `3rdparty/RakNet30` by hand after touching RakNet; it cannot join CI until RakNet's 56 `function_out_of_order` findings are worked through.

`.github/workflows/build.yml` runs eight checks and seven of them are not `reccmp-reccmp`. `tools/ncc/ncc.py` runs on its own Linux job and needs libclang plus the `clang` python bindings to run locally; it counts its findings and still exits 0, so the count is the verdict. `reccmp-decomplint` runs with `--warnfail`, so a warning fails the build: a nameref belongs in a header, never in a `.cpp`. `reccmp-vtable` compares every vtable that has a name on both sides, which is why a vendored vftable only goes into `webservice-globals.csv` one at a time, listed in `VENDORED_VTABLES` — naming them all pairs ATL and STL tables whose entries reccmp cannot resolve. `reccmp-datacmp` compares every named variable, and it matches statics by bare name, so a name has to be unique in both builds; `next` named RakNet's file-scope one and a function-local one our G3D keeps. Run all seven before committing, not just the accuracy number.


`.clang-format` sets `IncludeBlocks: Regroup`, so it merges include blocks and sorts across them; a blank line will not hold an include in place. `WebService.h` needs `<winsock2.h>` ahead of `<unknwn.h>`, which drags in `windows.h` and then `winsock.h`, and only `// clang-format off` around it survives. An incremental build will not catch this, because the object file that breaks is the one that did not need recompiling: delete `build/` before trusting a green result on anything that moves an include.

Never sweep timestamps across the working tree. `core.autocrlf` is true and `.gitattributes` marks `3rdparty/**` as `-text`, so git stores those files byte for byte from disk. Touching their mtimes forces a re-stat, and a `git add -A` then rewrites every one of them to whatever line ending the working copy happens to hold. That is what the CI workflow's backdate step does, which is fine on a throwaway runner and destructive in a clone.

`RakPeer`'s vtable annotation needs a commented declaration under it. `class_decl_regex` takes the first identifier after `class`, so `class RAK_DLL_EXPORT RakPeer` reads as a class named `RAK_DLL_EXPORT`; the regex also accepts a `//` line, so `// class RakPeer` on the next line names it. Both vtables hold 71 slots and always did. What stopped the slots pairing was constness: the original spells `char*` as `const char*` in `Connect`, `Send`, `RegisterAsRemoteProcedureCall`, `RegisterClassMemberRPC`, `UnregisterAsRemoteProcedureCall`, `SendConnectionRequest`, both `RPC` overloads and all four `RPCMap` identifier methods, so every one of those mangled to a name our build never emitted. Compare `?Name@RakPeer@@` across both PDBs to find the rest.

reccmp's summary prints one row per recompiled address, so where our link folded two functions the original kept apart, only one of the two annotations appears and the other is missing from the report rather than listed as a miss. `--verbose` on the address still resolves it. Reading absence as failure is what made 47 matching `Creator::create` instantiations look like 47 misses; probe the address before believing the summary.

`reccmp/webservice-vendored.csv` is order sensitive and must not be sorted whole. reccmp applies the rows in file order and the last one at an address wins, so every row that overrides a folded address has to sit after the row it overrides; those live at the end of the file. Sorting the file by address moved `find` and `_Buynode`'s `PAVName` overrides ahead of the instantiations they exist to displace, and `Name::lookup`, `namMap` and `declare` silently fell off 100 percent while the accuracy total barely moved.

A module is not a file. `modulestate.py` answers whether an original object is finished, which is a different question from whether one of our source files is: `util/Events.h` carries three functions out of the LTCG `WebService.obj` beside the ones from `standardout.obj`, and measuring by module never looked at them. Claim a file complete only against `filestate.py`, which scores every annotation the file carries.

The original link used `/OPT:ICF` and folded heavily. Many distinct source functions compiled to the same three-byte body and share one address, which is also why Ghidra's PDB source line importer throws several hundred `IllegalArgumentException` warnings when the symbols load. Those warnings are expected and harmless. Run `tools/check_folded.py` before concluding that a function at a shared address is genuinely unmatched.

`tools/check_folded.py` diverges from racers there. MSVC 8 keeps a PDB symbol for every alias the linker folded away, so all the `FOLDED` annotations on one address resolve rather than just the survivor. The check asks whether they landed on a single recompiled address instead of counting how many resolved.

`tools/` otherwise holds only what came from racers. Analysis scripts written along the way live in the session scratchpad.
