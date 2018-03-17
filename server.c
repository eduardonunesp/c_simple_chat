#include "server.h"

chat_status_enum_t
chat_init_server(
  unsigned int _port,
  chat_server_t **_chat_server) {

  *_chat_server = (chat_server_t*) malloc(sizeof(chat_server_t));

  if (!*_chat_server) {
    return CHAT_FAIL;
  }

  int opt = 1;
  (*_chat_server)->port = _port;

  // Creating socket file descriptor
  if (((*_chat_server)->server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    return CHAT_FAIL;
  }

  // Forcefully attaching socket to the port 8080
  if (setsockopt((*_chat_server)->server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    return CHAT_FAIL;
  }

  (*_chat_server)->address.sin_family = AF_INET;
  (*_chat_server)->address.sin_addr.s_addr = INADDR_ANY;
  (*_chat_server)->address.sin_port = htons(_port);

  // Forcefully attaching socket to the port 8080
  if (bind((*_chat_server)->server_fd, (struct sockaddr *)&(*_chat_server)->address, sizeof((*_chat_server)->address)) < 0) {
    return CHAT_FAIL;
  }

  if (listen((*_chat_server)->server_fd, 3) < 0) {
    return CHAT_FAIL;
  }

  return CHAT_SUCCESS;
}

void
chat_server_start(chat_server_t *_chat_server) {
  _chat_server->keep_running = 1;
  while(_chat_server->keep_running) {
    usleep(5000);
  }
}
