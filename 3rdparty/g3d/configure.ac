# Process this file with autoconf to produce a configure script.
AC_INIT([g3d-cpp], [6.04], [matrix@graphics3d.com])
AM_INIT_AUTOMAKE([1.6.1 foreign no-installinfo no-installman])
AC_CONFIG_SRCDIR([source/G3Dcpp/Box.cpp])
AM_CONFIG_HEADER(source/config.h)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB

# Check for X paths
AC_PATH_X

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([arpa/inet.h float.h malloc.h memory.h netdb.h netinet/in.h stddef.h stdlib.h string.h strings.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_STRUCT_TM

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_REALLOC
AC_FUNC_SETVBUF_REVERSED
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([atexit floor gethostbyname gethostname inet_ntoa memset mkdir pow select socket sqrt strchr strdup strrchr strstr])

AM_PATH_SDL(1.2.0, [], [AC_MSG_ERROR([Can't find SDL on your system])])

AC_CONFIG_FILES([Makefile
                 temp/Makefile
                 temp/debug/Makefile
                 temp/debug/g3d/Makefile
                 temp/debug/glg3d/Makefile
                 temp/release/Makefile
                 temp/release/g3d/Makefile
                 temp/release/glg3d/Makefile
                 source/Makefile])

AC_OUTPUT
