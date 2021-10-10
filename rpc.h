#ifndef MESSAGE_H_
#define MESSAGE_H_

#define CMDLEN          20
#define ARGLEN          20
#define MSGLEN          128
#define NARGS           10

typedef struct request_t {
  char cmd[CMDLEN];
  size_t argctr;
  char args[NARGS][ARGLEN];
} request_t;

typedef struct response_t {
  bool err;
  char err_msg[MSGLEN];
  char msg[MSGLEN];
} response_t;

void execute(int sockfd, const request_t *req, response_t *resp);

#endif  // MESSAGE_H_

