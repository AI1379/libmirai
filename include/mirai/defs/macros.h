
#ifndef MIRAI_INCLUDE_MIRAI_DEFS_MACROS_H_
#define MIRAI_INCLUDE_MIRAI_DEFS_MACROS_H_

// Determining endianness
// reference: https://stackoverflow.com/questions/4239993/determining-endianness-at-compile-time
#define BYTEORDER_LITTLE_ENDIAN 0 // Little endian machine.
#define BYTEORDER_BIG_ENDIAN 1 // Big endian machine.

//#define BYTEORDER_ENDIAN BYTEORDER_LITTLE_ENDIAN

#ifndef BYTEORDER_ENDIAN
// Detect with GCC 4.6's macro.
#   if defined(__BYTE_ORDER__)
#       if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#           define BYTEORDER_ENDIAN BYTEORDER_LITTLE_ENDIAN
#       elif (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#           define BYTEORDER_ENDIAN BYTEORDER_BIG_ENDIAN
#       else
#           error "Unknown machine byteorder endianness detected. User needs to define BYTEORDER_ENDIAN."
#       endif
// Detect with GLIBC's endian.h.
#   elif defined(__GLIBC__)
#       include <endian.h>
#       if (__BYTE_ORDER == __LITTLE_ENDIAN)
#           define BYTEORDER_ENDIAN BYTEORDER_LITTLE_ENDIAN
#       elif (__BYTE_ORDER == __BIG_ENDIAN)
#           define BYTEORDER_ENDIAN BYTEORDER_BIG_ENDIAN
#       else
#           error "Unknown machine byteorder endianness detected. User needs to define BYTEORDER_ENDIAN."
#       endif
    // Detect with _LITTLE_ENDIAN and _BIG_ENDIAN macro.
#   elif defined(_LITTLE_ENDIAN) && !defined(_BIG_ENDIAN)
#       define BYTEORDER_ENDIAN BYTEORDER_LITTLE_ENDIAN
#   elif defined(_BIG_ENDIAN) && !defined(_LITTLE_ENDIAN)
#       define BYTEORDER_ENDIAN BYTEORDER_BIG_ENDIAN
    // Detect with architecture macros.
#   elif defined(__sparc) || defined(__sparc__) || defined(_POWER) || defined(__powerpc__) || defined(__ppc__) || defined(__hpux) || defined(__hppa) || defined(_MIPSEB) || defined(_POWER) || defined(__s390__)
#       define BYTEORDER_ENDIAN BYTEORDER_BIG_ENDIAN
#   elif defined(__i386__) || defined(__alpha__) || defined(__ia64) || defined(__ia64__) || defined(_M_IX86) || defined(_M_IA64) || defined(_M_ALPHA) || defined(__amd64) || defined(__amd64__) || defined(_M_AMD64) || defined(__x86_64) || defined(__x86_64__) || defined(_M_X64) || defined(__bfin__)
#       define BYTEORDER_ENDIAN BYTEORDER_LITTLE_ENDIAN
#   elif defined(_MSC_VER) && (defined(_M_ARM) || defined(_M_ARM64))
#       define BYTEORDER_ENDIAN BYTEORDER_LITTLE_ENDIAN
#   else
#       error "Unknown machine byteorder endianness detected. User needs to define BYTEORDER_ENDIAN."
#   endif
#endif


#endif //MIRAI_INCLUDE_MIRAI_DEFS_MACROS_H_
