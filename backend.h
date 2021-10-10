#ifndef BACKEND_H_
#define BACKEND_H_

#include "rpc.h"

#define CMD_ADD         "add"
#define CMD_MULTIPLY    "multiply"
#define CMD_DIVIDE      "divide"
#define CMD_FACTORIAL   "factorial"
#define CMD_SLEEP       "sleep"
#define CMD_QUIT        "quit"
#define CMD_SHUTDOWN    "shutdown"
#define CMD_EXIT        "exit"

#define EXIT_SHUTDOWN   42

#define BUFSIZE         2048
#define MAX_NAME_SIZE   16
#define NHANDLERS       5
#define NPIDS           10

#define LOG(f_, ...) \
  do { \
    fprintf((f_), __VA_ARGS__); \
    fflush((f_)); \
  } while (0)

typedef void (*func)(const request_t*, response_t*);
struct handler_t {
  char name[MAX_NAME_SIZE];
  func fn;
} handlers[NHANDLERS];

int add_ints(int x, int y);
int multiply_ints(int x, int y);
float divide_floats(float x, float y);
int sleep_process(int x);
uint64_t factorial(uint64_t x);

void add_handler(const request_t *req, response_t *resp);
void multiply_handler(const request_t *req, response_t *resp);
void divide_handler(const request_t *req, response_t *resp);
void sleep_handler(const request_t *req, response_t *resp);
void factorial_handler(const request_t *req, response_t *resp);

void add_pid(pid_t *pids, size_t len, pid_t pid);
int wait_for_children(pid_t *pids, size_t len, int options);

void add_command_handler(char *name, func f);
void handle_client(int clientfd);

#endif  // BACKEND_H_

