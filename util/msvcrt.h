#ifndef MSVCRT_H
#define MSVCRT_H

#include <math.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <cstdio>
#include <cstdlib>

extern "C"
{
#endif

	int msvcrt_vsnprintf(char* buffer, size_t count, const char* format, va_list args);
	int msvcrt_snprintf(char* buffer, size_t count, const char* format, ...);
	int msvcrt_vsprintf(char* buffer, const char* format, va_list args);
	int msvcrt_sprintf(char* buffer, const char* format, ...);
	int msvcrt__vsnprintf(char* buffer, size_t count, const char* format, va_list args);
	int msvcrt__snprintf(char* buffer, size_t count, const char* format, ...);
	int msvcrt__vscprintf(const char* format, va_list args);
	int msvcrt__scprintf(const char* format, ...);
	int msvcrt_vfprintf(FILE* stream, const char* format, va_list args);
	int msvcrt_fprintf(FILE* stream, const char* format, ...);
	int msvcrt_printf(const char* format, ...);
	int msvcrt_rand(void);
	void msvcrt_srand(unsigned int seed);
	double msvcrt_strtod(const char* text, char** end);
	double msvcrt_atof(const char* text);

#ifdef __cplusplus
}

namespace std {
using ::msvcrt_atof;
using ::msvcrt_fprintf;
using ::msvcrt_printf;
using ::msvcrt_rand;
using ::msvcrt_snprintf;
using ::msvcrt_sprintf;
using ::msvcrt_srand;
using ::msvcrt_strtod;
using ::msvcrt_vfprintf;
using ::msvcrt_vsnprintf;
using ::msvcrt_vsprintf;
} // namespace std
#endif

#undef RAND_MAX
#define RAND_MAX 0x7fff
#define sprintf msvcrt_sprintf
#define vsprintf msvcrt_vsprintf
#define snprintf msvcrt_snprintf
#define vsnprintf msvcrt_vsnprintf
#define _snprintf msvcrt__snprintf
#define _vsnprintf msvcrt__vsnprintf
#define _scprintf msvcrt__scprintf
#define _vscprintf msvcrt__vscprintf
#define fprintf msvcrt_fprintf
#define vfprintf msvcrt_vfprintf
#define printf msvcrt_printf
#define rand msvcrt_rand
#define srand msvcrt_srand
#define strtod msvcrt_strtod
#define atof msvcrt_atof

#endif // MSVCRT_H
