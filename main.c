#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "chat.h"
#include "server.h"
#include "room.h"

static chat_server_t *chat_server;

void intHandler(int _dummy) {
  chat_server->keep_running = 0;
}

int main(int argc, const char* argv[]) {
  chat_room_t *chat_room;

  struct sigaction act;
  act.sa_handler = intHandler;
  sigaction(SIGINT, &act, NULL);

  if (chat_init_server(8080, &chat_server) != CHAT_SUCCESS) {
    CHAT_ERR("FAILED TO SERVER");
    return -1;
  }

  if (chat_init_room("Friends", chat_server, &chat_room) != CHAT_SUCCESS) {
    CHAT_ERR("FAILED TO START ROOM");
    return -1;
  }

  chat_server_start(chat_server);

  return 0;
}
