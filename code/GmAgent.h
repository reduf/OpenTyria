#pragma once

typedef struct GmAgent {
    uint32_t agent_id;
    float    x;
    float    y;
    uint32_t player_id;
} GmAgent;
typedef array(GmAgent) GmAgentArray;

uint32_t GameSrv_CreateAgent(GameSrv *srv);
GmAgent* GameSrv_GetAgent(GameSrv *srv, uint32_t agent_id);
GmAgent* GameSrv_GetAgentOrAbort(GameSrv *srv, uint32_t agent_id);
