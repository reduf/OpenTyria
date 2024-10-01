#pragma once

GmParty* GameSrv_CreateParty(GameSrv *srv)
{
    uint32_t party_id = GmIdAllocate(&srv->free_parties_slots, &srv->parties.base, sizeof(srv->parties.ptr[0]));
    GmParty *party = &srv->parties.ptr[party_id];
    memset(party, 0, sizeof(*party));
    party->party_id = party_id;
    return party;
}

void GmParty_AddPlayer(GmParty *party, uint32_t agent_id, uint32_t player_id)
{
    GmPartyPlayer *pp = array_push(&party->players, 1);
    pp->agent_id = agent_id;
    pp->player_id = player_id;
    pp->ready = true;
    pp->connected = true;
}
