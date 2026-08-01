#ifndef DECOMP_H
#define DECOMP_H

#ifndef NDEBUG
// Disable size assertions for debug builds because the sizes differ between debug and release
// builds. The release WebService.dll is what we ultimately want to decompile, so that is what
// we assert against.
#undef ENABLE_DECOMP_ASSERTS
#endif

#if defined(ENABLE_DECOMP_ASSERTS)
#define DECOMP_STATIC_ASSERT(V)                                                                                        \
	namespace {                                                                                                        \
	typedef int foo[(V) ? 1 : -1];                                                                                     \
	}
#define DECOMP_SIZE_ASSERT(T, S) DECOMP_STATIC_ASSERT(sizeof(T) == S)
#else
#define DECOMP_STATIC_ASSERT(V)
#define DECOMP_SIZE_ASSERT(T, S)
#endif

#ifndef sizeOfArray
#define sizeOfArray(arr) (sizeof(arr) / sizeof(arr[0]))
#endif

typedef unsigned char undefined;
typedef unsigned short undefined2;
typedef unsigned int undefined4;

extern undefined4 g_foldingDummyVariable;

// A stub body is one store, so /Ob2 inlines it and the caller loses the call its own match
// depends on. Mark a stub the original did not inline, and drop this once the body is real.
#define DECOMP_NOINLINE __declspec(noinline)

// Use STUB when a function has not yet been matched and/or is WIP.
#define STUB(val)                                                                                                      \
	do {                                                                                                               \
		g_foldingDummyVariable = val;                                                                                  \
	} while (0)

// Use MATCHING when a function is matched, but function folding inhibits success.
#define MATCHING(val)                                                                                                  \
	do {                                                                                                               \
		g_foldingDummyVariable = val;                                                                                  \
	} while (0)

#endif // DECOMP_H
