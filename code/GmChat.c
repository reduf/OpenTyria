#pragma once

bool slice_u16_equals_ascii_lit(const uint16_t *s1, size_t s1_len, const char *s2, size_t s2_len)
{
    if (s1_len != s2_len) {
        return false;
    }

    for (size_t idx = 0; idx < s1_len; ++idx) {
        if (s1[idx] != (uint16_t) s2[idx]) {
            return false;
        }
    }

    return true;
}

int GameSrv_HandleChatMessage(GameSrv *srv, uint16_t player_id, GameSrv_ChatMessage *msg)
{
    const uint16_t *ptr = msg->message_buf;
    size_t len = msg->message_len;

    if (slice_u16_equals_ascii_lit(ptr, len, "/stuck", sizeof("/stuck") - 1)) {
        GmAgent *agent;
        if ((agent = GameSrv_GetAgentByPlayerId(srv, player_id)) != NULL) {
            GameSrv_BroadcastAgentPosition(srv, agent);
        } else {
            log_error("Player id %u doesn't have an agent", player_id);
        }
    }

    return 0;
}
