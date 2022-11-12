#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/proc_us.h"

void routine(int steps)
{
    int z;
    char buffer_src[1024], buffer_dst[1024];

    for (z = 0; z < steps; z += 1) {
        memmove(buffer_dst, buffer_src, 1024);
        memmove(buffer_src, buffer_dst, 1024);
    }
}

void routine_file(int steps)
{
    int z;
    char data[256] = "hell_world";

    for (z = 0; z < steps; z += 1) {
        int fd = open("test.txt", O_CREATE | O_WRONLY);
        write(fd, data, sizeof(char) * 256);
        close(fd);
    }
}

void output_results(struct procps_status *ptable)
{
  fprintf(1, "%d\t%d\t%d\t%d\n", ptable->pid, ptable->total_running,
    ptable->total_sleep, ptable->total_waiting);
}

int main(int argc, char *argv[]) {
  int pid;
  int k, nprocess;
  int steps;
  int r, runs;

  if (argc != 4) {
    printf("usage: %s <nsubprocess> <nsteps> <runs>\n", argv[0]);
    exit(1);
  }

  nprocess = atoi(argv[1]);
  steps = atoi(argv[2]);
  runs = atoi(argv[3]);

  printf("%s\t%s\t%s\t%s\n", "pid", "running", "sleep", "waiting");
  for (r = 0; r < runs; ++r) {
    for (k = 0; k < nprocess; k++) {
      pid = fork ();
      if (pid < 0) {
        printf("%d failed in fork!\n", getpid());
        exit(1);
      } else if (pid == 0) {
        routine(steps);
        exit(1);
      }
    }

    struct procps_status ptable;
    pid = wait_ptable(0, sizeof(ptable), &ptable);
    for (k = 1; k < nprocess; k++) {
      output_results(&ptable);
      pid = wait_ptable(0, sizeof(ptable), &ptable);
    }
    output_results(&ptable);
  }

  exit(1);
}