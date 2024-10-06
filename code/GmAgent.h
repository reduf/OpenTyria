#pragma once

typedef enum AgentType {
    AgentType_None   = 0,
    AgentType_Living = 1,
    AgentType_Gadget = 2,
    AgentType_Item   = 3,
} AgentType;

#define CHAR_CLASS_BASE_MASK    0xF0000000
#define CHAR_CLASS_PLAYER_BASE  0x30000000
#define CHAR_CLASS_MONSTER_BASE 0x20000000

typedef struct GmAgent {
    uint32_t  agent_id;
    Vec2f     pos;
    uint16_t  plane;
    Vec2f     direction;
    uint32_t  model_id;
    uint32_t  load_time;
    uint32_t  health;
    uint32_t  energy;
    float     health_per_sec;
    float     energy_per_sec;
    AgentType agent_type;
    float     speed_base;
    uint32_t  level;
    uint32_t  effects;
    uint16_t  party_id;
    uint32_t  player_team_token;
} GmAgent;
typedef array(GmAgent) GmAgentArray;

GmAgent* GameSrv_CreateAgent(GameSrv *srv);
GmAgent* GameSrv_GetAgent(GameSrv *srv, uint32_t agent_id);
GmAgent* GameSrv_GetAgentOrAbort(GameSrv *srv, uint32_t agent_id);
void     GameSrv_RemoveAgentById(GameSrv *srv, uint32_t agent_id);

void GameSrv_SendAgentHealthEnergy(GameSrv *srv, GameConnection *conn, GmAgent *agent);
void GameSrv_BroadcastAgentLevel(GameSrv *srv, GmAgent *agent);
void GameSrv_SendAgentLoadTime(GameSrv *srv, GameConnection *conn, GmAgent *agent);
void GameSrv_BroadcastCreateAgent(GameSrv *srv, GmAgent *agent);
void GameSrv_BroadcastAgentInitialEffects(GameSrv *srv, GmAgent *agent);
void GameSrv_BroadcastUpdateAgentVisualEquipment(GameSrv *srv, GmAgent *agent, GmBagArray *bags);
void GameSrv_SendUpdatePlayerAgent(GameSrv *srv, GameConnection *conn, GmAgent *agent);
