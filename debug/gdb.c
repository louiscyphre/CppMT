/* File: gdb.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

/* Printing an error after calling system function */
#define SYS_ERROR(msg) \
  fprintf(stderr, "*** stack print error *** at %s %d: %s (%s)\n", \
           __FILE__, __LINE__, msg, strerror(errno));

/* ATTENTION! The macro is only for using inside gdb_PrintStackGDB, because
 * it contains return. With this approach, we will remain undeleted temporary
 * file, but it was left to not to complicate the program, because we consider
 * such situations exceptional */
#define SYS_ASSERT(cond, err_msg) \
  do \
  { \
    if (cond) \
      ; \
    else \
      { \
        SYS_ERROR(err_msg); \
        return 0; \
      } \
  } while (0)

/* When starting the debugger, you will need to specify the path to the binary
 * so that the debugger knew where to read the symbol table and debugging
 * information. In linux, this can be done via the /proc/self/exe link, but this
 * is not a portable way (i.e. on other unixes this will not work).
 * Therefore, the simplest option is to extract the file name via argv[0],
 * but it won't work if the program was run via PATH or if argv[0] is dummy */
static char *gdb_ProgramName;

void gdb_SetProgName(char *prog_name) {
  gdb_ProgramName = prog_name;
}

/* In case of luck, return 1 (true), otherwise 0 (false). In case that it needed
 * "above" for some reason */
int gdb_PrintStackGDB (void) {
  char buff[L_tmpnam];
  const char *scr_file_name;
  FILE *scr_file;
  pid_t child_pid;

  /* Create the name of the temporary file. Using the "right" interfaces like
   * tempnam or tmpfile will not work, because we need to pass to gdb the name
   * of the file, and with these interfaces the file name is not available. When
   * linking the program under linux, we will see a warning that looks something
   * like: "warning: the use of 'tmpnam' is dangerous, better use 'mkstemp'" */
  scr_file_name = tmpnam(buff);
  SYS_ASSERT(scr_file_name != NULL, "failed to gen tmp file name");

  /* In the file we put orders for gdb. This file will be transferred by option
   * to the debugger. In our case, we need to print the stack (bt) and exit (q)
   */
  scr_file = fopen (scr_file_name, "w");
  SYS_ASSERT(scr_file, "child: failed access gdb script");
  fprintf(scr_file, "bt\nq\n");
  fclose(scr_file);

  /* Forking to run the debugger in the child process and waiting in the parent.
   * Before fork, you can create a pipe, in the child process through dup2
   * connect it to stdout and stderr, and in the parent process to read from
   * there a text (debugger output) to filter unnecessary rows. But we will not
   * do this, in order not to complicate the program. Moreover, in different
   * versions of the debugger the prints may look differently */
  child_pid = fork();
  SYS_ASSERT(child_pid >= 0, "fork failed");

  if (child_pid == 0) {

      char pid_str[32];

      /* Child process: run gdb
       * Pass to the debugger to the pid of the process to which it should
       * attach. Since we are in the child process after fork, then for us
       * this means the pid of the parent process */
      sprintf(pid_str, "%d", (int)getppid());

      /* We assuming that gdb is found in the PATH. If not, then instead of
       * "gdb" it will be necessary to write here the full path to the debugger
       */
      execlp("gdb", "gdb", gdb_ProgramName, pid_str, "-q", "--batch", "-x", scr_file_name, NULL);

      /* In case of successful run we will not get here */
      SYS_ASSERT(0, "child: failed to exec gdb");
    } else {
      pid_t exited_pid;
      int child_status;

      /* Parent process: waiting for child process's exit (the process that gdb
       * runned from) */
      exited_pid = wait(&child_status);
      SYS_ASSERT(exited_pid == child_pid, "parent: error waiting child to die");
      SYS_ASSERT(WIFEXITED (child_status) && WEXITSTATUS (child_status) == 0,
                  "parent: abnormal child termination");
    }

  /* Removing temporary file */
  unlink(scr_file_name);

  return 1;
}

