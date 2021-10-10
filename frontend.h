#ifndef FRONTEND_H_
#define FRONTEND_H_

#include "rpc.h"

#define CMD_QUIT        "quit"
#define CMD_EXIT        "exit"
#define CMD_SHUTDOWN    "shutdown"

#define BUFSIZE         2048

#define LOG(f_, ...) \
  do { \
    fprintf((f_), __VA_ARGS__); \
    fflush((f_)); \
  } while (0)

char* strip_whitespaces(char *line, size_t *len);
request_t create_request(const char *line, const size_t len);

#endif  // FRONTEND_H_

