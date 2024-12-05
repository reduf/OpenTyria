#pragma once

typedef enum Channel {
    Channel_Alliance  = 0,
    Channel_Allies    = 1, // coop with two groups for instance.
    Channel_Unknown2  = 2,
    Channel_All       = 3,
    Channel_Unknown4  = 4,
    Channel_Moderator = 5,
    Channel_Emote     = 6,
    Channel_Warning   = 7, // shows in the middle of the screen and does not parse <c> tags
    Channel_Unknown8  = 8,
    Channel_Guild     = 9,
    Channel_Global    = 10,
    Channel_Group     = 11,
    Channel_Trade     = 12,
    Channel_Advisory  = 13,
    Channel_Whisper   = 14,
    Channel_Count,
} Channel;

int GameSrv_HandleChatMessage(GameSrv *srv, uint16_t player_id, GameSrv_ChatMessage *msg);
