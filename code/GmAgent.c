#pragma once

GmAgent* GameSrv_CreateAgent(GameSrv *srv)
{
    uint32_t agent_id = GmIdAllocate(&srv->free_agents_slots, &srv->agents.base, sizeof(srv->agents.ptr[0]));
    memset(&srv->agents.ptr[agent_id], 0, sizeof(srv->agents.ptr[agent_id]));
    srv->agents.ptr[agent_id].agent_id = agent_id;
    return &srv->agents.ptr[agent_id];
}

GmAgent* GameSrv_GetAgent(GameSrv *srv, uint32_t agent_id)
{
    if (srv->agents.len <= agent_id) {
        return NULL;
    }

    GmAgent *result = &srv->agents.ptr[agent_id];
    if (result->agent_id != agent_id) {
        return NULL;
    }

    return result;
}

GmAgent* GameSrv_GetAgentOrAbort(GameSrv *srv, uint32_t agent_id)
{
    GmAgent *result;
    if ((result = GameSrv_GetAgent(srv, agent_id)) == NULL) {
        abort();
    }
    return result;
}

void GameSrv_RemoveAgentById(GameSrv *srv, uint32_t agent_id)
{
    GmAgent *result;
    if ((result = GameSrv_GetAgent(srv, agent_id)) == NULL) {
        return;
    }

    result->agent_id = 0;
    array_add(&srv->free_agents_slots, agent_id);

    GameSrvMsg *buffer = GameSrv_BuildMsg(srv, GAME_SMSG_AGENT_REMOVE);
    GameSrv_AgentRemove *msg = &buffer->agent_remove;
    msg->agent_id = agent_id;

    GameSrv_BroadcastMessage(srv, buffer, sizeof(*msg));
}

void GameSrv_SendAgentHealthEnergy(GameSrv *srv, GameConnection *conn, GmAgent *agent)
{
    {
        GameSrvMsg *buffer = GameSrv_BuildMsg(srv, GAME_SMSG_UPDATE_AGENT_INT_PROPERTY);
        GameSrv_UpdateAgentIntProperty *msg = &buffer->update_agent_int_property;
        msg->agent_id = agent->agent_id;
        msg->prop_id = AgentProperty_Energy;
        msg->value = agent->energy;
        GameConnection_SendMessage(conn, buffer, sizeof(*msg));
        msg->prop_id = AgentProperty_Health;
        msg->value = agent->health;
        GameConnection_SendMessage(conn, buffer, sizeof(*msg));
    }

    {
        GameSrvMsg *buffer = GameSrv_BuildMsg(srv, GAME_SMSG_UPDATE_AGENT_FLOAT_PROPERTY);
        GameSrv_UpdateAgentFloatProperty *msg = &buffer->update_agent_float_property;
        msg->agent_id = agent->agent_id;
        msg->prop_id = AgentProperty_EnergyRegen;
        msg->value = agent->energy_per_sec;
        GameConnection_SendMessage(conn, buffer, sizeof(*msg));
    }
}

void GameSrv_BroadcastAgentLevel(GameSrv *srv, GmAgent *agent)
{
    GameSrvMsg *buffer = GameSrv_BuildMsg(srv, GAME_SMSG_UPDATE_AGENT_INT_PROPERTY);
    GameSrv_UpdateAgentIntProperty *msg = &buffer->update_agent_int_property;
    msg->agent_id = agent->agent_id;
    msg->prop_id = AgentProperty_PublicLevel;
    msg->value = agent->level;
    GameSrv_BroadcastMessage(srv, buffer, sizeof(*msg));
}

void GameSrv_SendAgentLoadTime(GameSrv *srv, GameConnection *conn, GmAgent *agent)
{
    GameSrvMsg *buffer = GameSrv_BuildMsg(srv, GAME_SMSG_AGENT_LOAD_TIME);
    GameSrv_AgentLoadTime *msg = &buffer->agent_load_time;
    msg->load_time = agent->load_time;
    GameConnection_SendMessage(conn, buffer, sizeof(*msg));
}

void GameSrv_BroadcastCreateAgent(GameSrv *srv, GmAgent *agent)
{
    GameSrvMsg *buffer = GameSrv_BuildMsg(srv, GAME_SMSG_CREATE_AGENT);
    GameSrv_CreateAgentMsg *msg = &buffer->create_agent;
    msg->agent_id = agent->agent_id;
    msg->model_id = agent->model_id;
    msg->agent_type = agent->agent_type;
    msg->h000B = 5;
    msg->pos = agent->pos;
    msg->plane = agent->plane;
    msg->direction = agent->direction;
    msg->h001E = 1;
    msg->speed_base = agent->speed_base;
    msg->h0023 = 1.f;
    msg->h0027 = 0x41400000;
    msg->player_team_token = agent->player_team_token;
    msg->h003B = 0;
    msg->h004B.x = HUGE_VALF;
    msg->h004B.y = HUGE_VALF;
    msg->h0059.x = HUGE_VALF;
    msg->h0059.y = HUGE_VALF;
    GameSrv_BroadcastMessage(srv, buffer, sizeof(*msg));
}

void GameSrv_SendAgentInitialEffects(GameSrv *srv, GameConnection *conn, GmAgent *agent)
{
    GameSrvMsg *buffer = GameSrv_BuildMsg(srv, GAME_SMSG_AGENT_INITIAL_EFFECTS);
    GameSrv_InitialAgentEffects *msg = &buffer->initial_agent_effects;
    msg->agent_id = agent->agent_id;
    msg->effects = agent->effects;
    GameConnection_SendMessage(conn, buffer, sizeof(*msg));
}

void GameSrv_SendUpdatePlayerAgent(GameSrv *srv, GameConnection *conn, GmAgent *agent)
{
    GameSrvMsg *buffer = GameSrv_BuildMsg(srv, GAME_SMSG_UPDATE_PLAYER_AGENT);
    GameSrv_UpdatePlayerAgent *msg = &buffer->update_player_agent;
    msg->agent_id = agent->agent_id;
    msg->unk0 = 3;
    GameConnection_SendMessage(conn, buffer, sizeof(*msg));
}
