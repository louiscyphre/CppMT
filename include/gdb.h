/* File: gdb.h */
#ifndef __GDB_H__
#define __GDB_H__

#ifdef __cplusplus
extern "C" {
#endif

extern void
gdb_SetProgName (char *prog_name);

extern int
gdb_PrintStackGDB (void);

#ifdef __cplusplus
}
#endif

#endif /* __GDB_H__ */

