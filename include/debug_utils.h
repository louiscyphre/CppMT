/* File: debug_utils.h */
#ifndef __DEBUG_UTILS_H__
#define __DEBUG_UTILS_H__

#include <stdio.h>
#include "gdb.h"

#ifdef DEBUG
/*
//#define ASSERT(cond) \
   // do \
   // { \
    //  if (cond) \
    //    ; \
  //    else \
   //     { \
  //        fprintf(stderr, "==========================\n"); \
 //         fprintf(stderr, "Assertion \"%s\" failed at %s:%d\n", #cond,
//__FILE__, __LINE__); \
 //         fprintf(stderr, "==========================\n"); \
          //gdb_PrintStackGDB(); \
  //        fprintf(stderr, "==========================\n"); \
  //      } \
  //  } while (0)*/
#else
#define ASSERT(cond) \
    do \
    { \
    } while (0)
#endif


#if defined(_MSC_VER)
    #define __FUNCNAME__ __FUNCTION__
#else
    #define __FUNCNAME__ __PRETTY_FUNCTION__
#endif

#ifdef DEBUG
#define LOG() \
    fprintf(stdout, "[DEBUG] In \"%s\": at %s:%d\n",__FUNCNAME__, \
__FILE__, __LINE__);
#else
#define LOG() \
    {};

#endif
#endif /* __DEBUG_UTILS_H__ */
