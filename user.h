#pragma once

#include <netinet/in.h>
#include "server.h"
#include "chat.h"

typedef enum {
  USER_CONNECTING = 0x1,
  USER_CONNECTED  = 0x2
} chat_user_enum_t;

typedef struct {
  pthread_t thread_fd;
  int socket_fd;
  chat_server_t *chat_server;
  struct chat_room_t *chat_room;
  const char *nick_name;
} chat_user_t;

chat_status_enum_t
chat_init_user(
  const char *_nick_name,
  int _socket_fd,
  struct chat_room_t *_chat_room,
  chat_user_t **chat_user);

void
chat_close_user(chat_user_t *chat_user);

void
chat_send_user(chat_user_t *chat_user, const char *message);
