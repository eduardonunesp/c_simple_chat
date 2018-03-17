#pragma once

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

#include "chat.h"

typedef struct {
  pthread_mutex_t mutex;
  struct sockaddr_in address;
  int server_fd;
  unsigned int port;
  short keep_running;
} chat_server_t;

chat_status_enum_t
chat_init_server(
  unsigned int _port,
  chat_server_t **_chat_server);

void
chat_server_start(chat_server_t *_chat_server);
