#pragma once

typedef enum AgentType {
    AgentType_None   = 0,
    AgentType_Living = 1,
    AgentType_Gadget = 2,
    AgentType_Item   = 3,
} AgentType;

typedef struct GmAgent {
    uint32_t  agent_id;
    Vec2f     pos;
    uint16_t  plane;
    Vec2f     direction;
    uint32_t  player_id;
    uint32_t  load_time;
    uint32_t  health;
    uint32_t  energy;
    float     health_per_sec;
    float     energy_per_sec;
    AgentType agent_type;
    float     speed_base;
    uint32_t  level;
    uint32_t  effects;
} GmAgent;
typedef array(GmAgent) GmAgentArray;

GmAgent* GameSrv_CreateAgent(GameSrv *srv);
GmAgent* GameSrv_GetAgent(GameSrv *srv, uint32_t agent_id);
GmAgent* GameSrv_GetAgentOrAbort(GameSrv *srv, uint32_t agent_id);

void GameSrv_SendAgentHealthEnergy(GameConnection *conn, GmAgent *agent);
void GameSrv_SendAgentLevel(GameConnection *conn, GmAgent *agent);
void GameSrv_SendAgentLoadTime(GameConnection *conn, GmAgent *agent);
void GameSrv_SendCreateAgent(GameConnection *conn, GmAgent *agent);
void GameSrv_SendAgentInitialEffects(GameConnection *conn, GmAgent *agent);
