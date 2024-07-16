#pragma once

#pragma pack(push, 1)
typedef union GameCliMsg {
    uint16_t          header;
    uint8_t           buffer[MSG_MAX_BUFFER_SIZE];
} GameCliMsg;

typedef union GameSrvMsg {
    uint16_t          header;
    uint8_t           buffer[MSG_MAX_BUFFER_SIZE];
} GameSrvMsg;
#pragma pack(pop)
