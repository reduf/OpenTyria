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
