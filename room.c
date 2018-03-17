#include "room.h"
#include <string.h>
#include <pthread.h>
#include "user.h"

chat_status_enum_t add_new_user(chat_room_t *_room, chat_user_t *_user) {
  for (short i = 0; i < MAX_ROOM_SIZE; i++) {
    if (_room->users[i] == NULL) {
      _room->users[i] = _user;
      return CHAT_SUCCESS;
    }
  }

  return CHAT_FAIL;
}

void remove_user(chat_room_t *_room, chat_user_t *_user) {
  short i = 0;
  for (; i < MAX_ROOM_SIZE; i++) {
    if (_room->users[i] == _user) {
      break;
    }
  }

  _room->users[i] = NULL;
}

void broadcast_others_message(chat_room_t *_room, chat_user_t *chat_user, const char *msg) {
  for (short i = 0; i < MAX_ROOM_SIZE; i++) {
    if (_room->users[i] != NULL && _room->users[i] != chat_user) {
      chat_send_user(_room->users[i], strdup(msg));
    }
  }
}

void *new_chat_room_thread(void *data) {
  chat_room_t *chat_room = (chat_room_t*) data;

  while (chat_room->chat_server->keep_running) {
    int addrlen = sizeof(chat_room->chat_server->address);
    int new_socket_fd = accept(chat_room->chat_server->server_fd, (struct sockaddr *)&chat_room->chat_server->address, (socklen_t *)&addrlen);

    CHAT_DBG("NEW SOCKET %d", new_socket_fd);

    chat_user_t *new_chat_user;
    if (chat_init_user("user", new_socket_fd, (struct chat_room_t *) chat_room, &new_chat_user) != CHAT_SUCCESS) {
      fprintf(stderr, "FAIL TO INSTANTIATE NEW USER");
    }

    if (add_new_user(chat_room, new_chat_user) != CHAT_SUCCESS) {
      fprintf(stderr, "NO SPACE IN ROOM");
      chat_close_user(new_chat_user);
    }

    broadcast_others_message(chat_room, new_chat_user, "New user\n\n");

    CHAT_DBG("NEW USER %p", new_chat_user);

    usleep(5000);
  }

  return NULL;
}

chat_status_enum_t
chat_init_room(
  const char *_room_name,
  chat_server_t *_chat_server,
  chat_room_t **_chat_room) {

  *_chat_room = (chat_room_t*) malloc(sizeof(chat_room_t));

  if (!*_chat_room) {
    return CHAT_FAIL;
  }

  (*_chat_room)->room_name = strdup(_room_name);
  (*_chat_room)->chat_server = _chat_server;

  pthread_create(&(*_chat_room)->thread_fd, NULL, new_chat_room_thread, *_chat_room);

  return CHAT_SUCCESS;
}
