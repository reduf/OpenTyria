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

GmAgent* GameSrv_GetAgentByPlayerId(GameSrv *srv, uint32_t player_id)
{
    GmPlayer *player;
    if ((player = GameSrv_GetPlayer(srv, player_id)) == NULL) {
        return NULL;
    }

    GmAgent *agent;
    if ((agent = GameSrv_GetAgent(srv, player->agent_id)) == NULL) {
        return NULL;
    }

    return agent;
}

void GameSrv_SendAgentHealthEnergy(GameSrv *srv, GameConnection *conn, GmAgent *agent)
{
    {
        GameSrvMsg *buffer = GameSrv_BuildMsg(srv, GAME_SMSG_UPDATE_AGENT_INT_PROPERTY);
        GameSrv_UpdateAgentIntProperty *msg = &buffer->update_agent_int_property;
        msg->agent_id = agent->agent_id;
        msg->prop_id = AgentProperty_Energy;
        msg->value = agent->energy_max;
        GameConnection_SendMessage(conn, buffer, sizeof(*msg));
        msg->prop_id = AgentProperty_Health;
        msg->value = agent->health_max;
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
    msg->position = agent->position;
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

void GameSrv_BroadcastAgentInitialEffects(GameSrv *srv, GmAgent *agent)
{
    GameSrvMsg *buffer = GameSrv_BuildMsg(srv, GAME_SMSG_AGENT_INITIAL_EFFECTS);
    GameSrv_InitialAgentEffects *msg = &buffer->initial_agent_effects;
    msg->agent_id = agent->agent_id;
    msg->effects = agent->effects;
    GameSrv_BroadcastMessage(srv, buffer, sizeof(*msg));
}

void GameSrv_BroadcastUpdateAgentVisualEquipment(GameSrv *srv, GmAgent *agent, GmBagArray *bags)
{
    GameSrvMsg *buffer = GameSrv_BuildMsg(srv, GAME_SMSG_UPDATE_AGENT_VISUAL_EQUIPMENT);
    GameSrv_UpdateAgentVisualEquipment *msg = &buffer->update_agent_visual_equipment;

    if (EquippedItemSlot_Count < bags->equipped_items.slot_count) {
        log_error("Expected %u item in equipped item bag, but found %u", EquippedItemSlot_Count, bags->equipped_items.slot_count);
        return;
    }

    uint32_t *items = bags->equipped_items.items;
    msg->agent_id = agent->agent_id;
    msg->weapon_item_id = items[EquippedItemSlot_Weapon];
    msg->offhand_item_id = items[EquippedItemSlot_OffHand];
    msg->body_item_id = items[EquippedItemSlot_Body];
    msg->boots_item_id = items[EquippedItemSlot_Legs];
    msg->legs_item_id = items[EquippedItemSlot_Head];
    msg->gloves_item_id = items[EquippedItemSlot_Boots];
    msg->head_item_id = items[EquippedItemSlot_Gloves];
    msg->costume_head_item_id = items[EquippedItemSlot_CostumeBody];
    msg->costume_body_item_id = items[EquippedItemSlot_CostumeHead];

    GameSrv_BroadcastMessage(srv, buffer, sizeof(*msg));
}

void GameSrv_SendUpdatePlayerAgent(GameSrv *srv, GameConnection *conn, GmAgent *agent)
{
    GameSrvMsg *buffer = GameSrv_BuildMsg(srv, GAME_SMSG_UPDATE_PLAYER_AGENT);
    GameSrv_UpdatePlayerAgent *msg = &buffer->update_player_agent;
    msg->agent_id = agent->agent_id;
    msg->unk0 = 3;
    GameConnection_SendMessage(conn, buffer, sizeof(*msg));
}

void GameSrv_BroadcastAgentPosition(GameSrv *srv, GmAgent *agent)
{
    GameSrvMsg *buffer = GameSrv_BuildMsg(srv, GAME_SMSG_AGENT_UPDATE_POSITION);
    GameSrv_UpdateAgentPostion *msg = &buffer->update_agent_position;
    msg->agent_id = agent->agent_id;
    msg->position = agent->position;
    msg->plane = agent->plane;

    GameSrv_BroadcastMessage(srv, buffer, sizeof(*msg));
}

void GameSrv_BroadcastWorldSimulationTick(GameSrv *srv, uint32_t delta_ms)
{
    GameSrvMsg *buffer = GameSrv_BuildMsg(srv, GAME_SMSG_WORLD_SIMULATION_TICK);
    GameSrv_UpdateWorldSimulationTick *msg = &buffer->world_simulation_tick;
    assert(0 < delta_ms);
    msg->delta_ms = delta_ms;
    GameSrv_BroadcastMessage(srv, buffer, sizeof(*msg));
}

void GameSrv_WorldTick(GameSrv *srv)
{
    int64_t delta_time = max_i64(0, srv->current_frame_time - srv->last_world_tick);
    float delta = (float)delta_time / 1000.f;

    GmAgentArray agents = srv->agents;
    for (size_t idx = 0; idx < agents.len; ++idx) {
        GmAgent *agent = &agents.ptr[idx];
        if (agent->agent_id != idx) {
            continue;
        }

        if (agent->speed != 0.f) {
            float dist = delta * agent->speed;
            float dx = agent->direction.x * dist;
            float dy = agent->direction.y * dist;

            // Prevent overrunning the target position.
            float max_dx = agent->destination.x - agent->position.x;
            float max_dy = agent->destination.y - agent->position.y;

            if (fabsf(max_dx) < fabsf(dx) || fabsf(max_dy) < fabsf(dy)) {
                agent->position = agent->destination;

                agent->speed = 0.f;
                agent->destination.x = 0.f;
                agent->destination.y = 0.f;
            } else {
                agent->position.x += dx;
                agent->position.y += dy;
            }
        }

        float health_diff = delta * agent->health_per_sec;
        agent->health = clampf(agent->health + health_diff, 0.f, 1.f);
        // @TODO: Check if the agent is dead!!!

        float energy_diff = delta * agent->energy_per_sec;
        agent->energy = clampf(agent->energy + energy_diff, 0.f, 1.f);
    }

    GameSrv_BroadcastWorldSimulationTick(srv, (uint32_t) delta_time);
    srv->last_world_tick = srv->current_frame_time;
}

void GameSrv_BroadcastMoveAgentToPoint(GameSrv *srv, GmAgent *agent)
{
    GameSrvMsg *buffer = GameSrv_BuildMsg(srv, GAME_SMSG_MOVE_AGENT_TO_POINT);
    GameSrv_MoveAgentToPoint *msg = &buffer->move_agent_to_point;
    msg->agent_id = agent->agent_id;
    msg->dest.x = agent->destination.x;
    msg->dest.y = agent->destination.y;
    msg->plane = 0;
    msg->current_plane = 0;

    GameSrv_BroadcastMessage(srv, buffer, sizeof(*msg));
}

int GameSrv_HandleMoveToCoord(GameSrv *srv, uint16_t player_id, GameSrv_MoveToCoord *msg)
{
    GmAgent *agent;
    if ((agent = GameSrv_GetAgentByPlayerId(srv, player_id)) == NULL) {
        log_error("Unknow player with player id %u", player_id);
        return ERR_SERVER_ERROR;
    }

    agent->destination.x = msg->pos.x;
    agent->destination.y = msg->pos.y;
    agent->speed = agent->speed_base;

    GameSrv_BroadcastMoveAgentToPoint(srv, agent);
    return ERR_OK;
}
