#include "user.h"
#include <string.h>
#include <pthread.h>
#include "room.h"

void *new_chat_user_thread(void *data) {
  chat_user_t *chat_user = (chat_user_t*) data;
  CHAT_DBG("NEW CONNECTION ARRIVE %d %p", chat_user->socket_fd, chat_user);

  while (chat_user->chat_server->keep_running) {
    char buffer[1024] = {0};
    short closed = 0;
    ssize_t n = read(chat_user->socket_fd, buffer, 1024);
    ssize_t n2;

    if (strlen(buffer) > 0) {
      CHAT_DBG("%s", buffer);

      if('\n' == buffer[strlen(buffer) - 1])
        buffer[strlen(buffer) - 1] = '\0';

      broadcast_others_message((chat_room_t *) chat_user->chat_room, chat_user, buffer);
      memset(buffer, '\0', sizeof(buffer));
    }

    if (n <= 0) {
      closed = 1;
    }

    n2 = send(chat_user->socket_fd, buffer, n, SO_NOSIGPIPE);
    if (n2 == -1) {
        closed = 1;
    }

    if (n2 != n) {
        closed = 1;
    }

    if (closed) {
      broadcast_others_message((chat_room_t *) chat_user->chat_room, chat_user, "User saiu");
      close(chat_user->socket_fd);
      CHAT_DBG("CONNETION LOST %d", chat_user->socket_fd);
      break;
    }

    usleep(5000);
  }

  return NULL;
}

chat_status_enum_t
chat_init_user(
  const char *_nick_name,
  int _socket_fd,
  struct chat_room_t *_chat_room,
  chat_user_t **_chat_user) {

  *_chat_user = (chat_user_t *)malloc(sizeof(chat_user_t));

  if (!*_chat_user) {
    return CHAT_FAIL;
  }

  (*_chat_user)->nick_name = strdup(_nick_name);
  (*_chat_user)->socket_fd = _socket_fd;
  (*_chat_user)->chat_server = ((chat_room_t *) _chat_room)->chat_server;
  (*_chat_user)->chat_room = _chat_room;

  CHAT_DBG("NEW SOCKET %d", _socket_fd);

  pthread_create(&(*_chat_user)->thread_fd, NULL, new_chat_user_thread, *_chat_user);

  return CHAT_SUCCESS;
}

void
chat_close_user(chat_user_t *chat_user) {
  close(chat_user->socket_fd);
}

void
chat_send_user(chat_user_t *chat_user, const char *message) {
  send(chat_user->socket_fd, message, sizeof(message), 0);
}
