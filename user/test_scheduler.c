#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/fs.h"

int main(int argc, char *argv[]) {
  int pid;
  int k, nprocess;
  int z, steps;
  char buffer_src[1024], buffer_dst[1024];

  if (argc != 3) {
    printf("usage: %s <nsubprocess> <nsteps>\n", argv[0]);
    exit(1);
  }

  nprocess = atoi(argv[1]);
  steps = atoi(argv[2]);

  printf("usage: %s <nsubprocess> <nsteps>\n", argv[0]);

  for (k = 0; k < nprocess; k++) {
    pid = fork ();
    if (pid < 0) {
      printf("%d failed in fork!\n", getpid());
      exit(1);
    } else if (pid == 0) {  
      printf("[pid=%d] created\n", getpid());
      for (z = 0; z < steps; z += 1) {
         memmove(buffer_dst, buffer_src, 1024);
         memmove(buffer_src, buffer_dst, 1024);
      }
      exit(1);
    }
  }

  for (k = 0; k < nprocess; k++) {
    pid = wait(0);
    printf("[pid=%d] terminated\n", pid);
  }

  exit(1);
}