/* Custom ASSERT with extended stack printing
 * This code was taken from here:
 * http://www.cyberforum.ru/blogs/18334/blog102.html
 * The author of the idea is user with nickname "era" from this forum.
 * Realisation by user with nickname "Evg", and on this link is one of his blog
 * posts within the forum.
 */

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

