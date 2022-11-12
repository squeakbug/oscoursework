enum procstate { UNUSED, USED, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };

struct procps_status {
  uint sz;
  enum procstate state;
  int pid;
  uint total_sleep;
  uint total_running;
  uint total_waiting;
  uint time_slice;
  int priority;
  int static_priority;
  char name[16];
};
