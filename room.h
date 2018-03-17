#pragma once

#include <pthread.h>
#include "chat.h"
#include "server.h"
#include "user.h"

#define MAX_ROOM_SIZE 10

typedef struct {
  pthread_t thread_fd;
  const char *room_name;
  chat_server_t *chat_server;
  chat_user_t *users[MAX_ROOM_SIZE];
} chat_room_t;

chat_status_enum_t
chat_init_room(
  const char *_room_name,
  chat_server_t *_chat_server,
  chat_room_t **chat_room);

void
broadcast_others_message(
  chat_room_t *_room,
  chat_user_t *chat_user,
  const char *msg);
