#pragma once

#define CHAT_DBG(STR, ...) fprintf(stdout, STR "\n", ##__VA_ARGS__)
#define CHAT_ERR(STR, ...) fprintf(stderr, STR "\n", ##__VA_ARGS__)

typedef enum {
  CHAT_SUCCESS = 0x0,
  CHAT_FAIL    = 0x1
} chat_status_enum_t;
