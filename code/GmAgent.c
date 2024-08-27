#pragma once

uint32_t GameSrv_CreateAgent(GameSrv *srv)
{
    if (srv->free_agents_slots.len == 0) {
        size_t new_size, idx;
        if (srv->agents.len == 0) {
            new_size = 32;
            idx = 1; // skip the first slot
        } else {
            new_size = srv->agents.len * 2;
            idx = srv->agents.len;
        }

        array_resize(&srv->agents, new_size);
        array_reserve(&srv->free_agents_slots, srv->agents.len - new_size);
        for (; idx < new_size; ++idx) {
            array_add(&srv->free_agents_slots, (uint32_t) idx);
        }
    }

    uint32_t agent_id = array_pop(&srv->free_agents_slots);
    memset(&srv->agents.ptr[agent_id], 0, sizeof(srv->agents.ptr[agent_id]));
    srv->agents.ptr[agent_id].agent_id = agent_id;
    return agent_id;
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

    return result;;
}

GmAgent* GameSrv_GetAgentOrAbort(GameSrv *srv, uint32_t agent_id)
{
    GmAgent *result;
    if ((result = GameSrv_GetAgent(srv, agent_id)) == NULL) {
        abort();
    }
    return result;
}
