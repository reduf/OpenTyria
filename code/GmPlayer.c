#pragma once

GmPlayer* GameSrv_CreatePlayer(
    GameSrv *srv,
    uintptr_t token,
    struct uuid account_id,
    struct uuid char_id)
{
    uint32_t player_id = GmIdAllocate(&srv->free_players_slots, &srv->players.base, sizeof(srv->players.ptr[0]));

    GmPlayer *player = &srv->players.ptr[player_id];
    memset(player, 0, sizeof(*player));
    player->player_id = (uint32_t) player_id;
    player->conn_token = token;
    player->account_id = account_id;
    player->char_id = char_id;

    ++srv->player_count;
    return player;
}

void GameSrv_RemovePlayer(GameSrv *srv, size_t player_id)
{
    // nothing to free in GmPlayer
    assert(player_id < srv->players.len);
    memset(&srv->players.ptr[player_id], 0, sizeof(srv->players.ptr[player_id]));
    --srv->player_count;
}

GmPlayer* GameSrv_GetPlayer(GameSrv *srv, size_t player_id)
{
    if ((player_id < srv->players.len) && (srv->players.ptr[player_id].player_id == player_id)) {
        return &srv->players.ptr[player_id];
    } else {
        return NULL;
    }
}

void GameSrv_SendHardModeUnlocked(GameConnection *conn)
{
    GameSrvMsg *buffer = GameConnection_BuildMsg(conn, GAME_SMSG_HARD_MODE_UNLOCKED);
    GameSrv_HardModeUnlocked *msg = &buffer->hard_mode_unlocked;
    msg->hard_mode_unlocked = true;
    GameConnection_SendMessage(conn, buffer, sizeof(*msg));
}

void GameSrv_SendPlayerFactions(GameConnection *conn)
{
    GameSrvMsg *buffer = GameConnection_BuildMsg(conn, GAME_SMSG_PLAYER_FACTION_MAX_KURZICK);
    buffer->kurzick_max.max_faction = 5000;
    GameConnection_SendMessage(conn, buffer, sizeof(buffer->kurzick_max));

    buffer = GameConnection_BuildMsg(conn, GAME_SMSG_PLAYER_FACTION_MAX_LUXON);
    buffer->luxon_max.max_faction = 5000;
    GameConnection_SendMessage(conn, buffer, sizeof(buffer->luxon_max));

    buffer = GameConnection_BuildMsg(conn, GAME_SMSG_PLAYER_FACTION_MAX_BALTHAZAR);
    buffer->balthazar_max.max_faction = 10000;
    GameConnection_SendMessage(conn, buffer, sizeof(buffer->balthazar_max));

    buffer = GameConnection_BuildMsg(conn, GAME_SMSG_PLAYER_FACTION_MAX_IMPERIAL);
    buffer->imperial_max.max_faction = 10000;
    GameConnection_SendMessage(conn, buffer, sizeof(buffer->imperial_max));
}

void GameSrv_SendPlayerAgentAttributes(GameConnection *conn, GmPlayer *player)
{
    GameSrvMsg *buffer = GameConnection_BuildMsg(conn, GAME_SMSG_AGENT_CREATE_ATTRIBUTES);
    buffer->agent_create_attribute.agent_id = player->agent_id;
    GameConnection_SendMessage(conn, buffer, sizeof(buffer->agent_create_attribute));
}

void GameSrv_SendInstanceLoadPlayerName(GameConnection *conn, GmPlayer *player)
{
    assert(!uuid_is_null(&player->character.char_id));
    GameSrvMsg *buffer = GameConnection_BuildMsg(conn, GAME_SMSG_INSTANCE_LOAD_PLAYER_NAME);
    GameSrv_InstancePlayerName *msg = &buffer->instance_player_name;
    STATIC_ASSERT(ARRAY_SIZE(msg->name) <= ARRAY_SIZE(player->character.charname.buf));
    msg->n_name = (uint32_t) player->character.charname.len;
    memcpy_u16(msg->name, player->character.charname.buf, msg->n_name);
    GameConnection_SendMessage(conn, buffer, sizeof(*msg));
}

void GameSrv_SendUnlockedProfession(GameConnection *conn, GmPlayer *player)
{
    GameSrvMsg *buffer = GameConnection_BuildMsg(conn, GAME_SMSG_PLAYER_UNLOCKED_PROFESSION);
    GameSrv_UnlockedProfession *msg = &buffer->unlocked_profession;
    msg->agent_id = player->agent_id;
    msg->unlocked = (1 << Profession_Count) - 1;
    GameConnection_SendMessage(conn, buffer, sizeof(*msg));
}

void GameSrv_SendPlayerAgentAttribute(GameConnection *conn, GmPlayer *player)
{
    {
        GameSrvMsg *buffer = GameConnection_BuildMsg(conn, GAME_SMSG_PLAYER_ATTR_SET);
        GameSrv_PlayerAttr *msg = &buffer->player_attr;
        msg->experience = 10000;
        msg->current_kurzick = 100;
        msg->total_earned_kurzick = 100000;
        msg->current_luxon = 100;
        msg->total_earned_luxon = 100000;
        msg->current_imperial = 500;
        msg->total_earned_imperial = 15000;
        msg->unk_faction4 = 0;
        msg->unk_faction5 = 0;
        msg->level = 0;
        msg->morale = 0;
        msg->current_balth = 200;
        msg->total_earned_balth = 200000;
        msg->current_skill_points = 0;
        msg->total_earned_skill_points = 0;
        GameConnection_SendMessage(conn, buffer, sizeof(*msg));
    }

    {
        GameSrvMsg *buffer = GameConnection_BuildMsg(conn, GAME_SMSG_AGENT_ATTR_UPDATE_INT);
        GameSrv_AgentAttrUpdateInt *msg = &buffer->agent_attr_update_int;
        msg->agent_id = player->agent_id;
        msg->attr_id = 41;
        msg->value = 20;
        GameConnection_SendMessage(conn, buffer, sizeof(*msg));
        msg->attr_id = 42;
        msg->value = 100;
        GameConnection_SendMessage(conn, buffer, sizeof(*msg));
        msg->attr_id = 64;
        msg->value = 0;
        GameConnection_SendMessage(conn, buffer, sizeof(*msg));
    }

    {
        GameSrvMsg *buffer = GameConnection_BuildMsg(conn, GAME_SMSG_AGENT_ATTR_UPDATE_FLOAT);
        GameSrv_AgentAttrUpdateFloat *msg = &buffer->agent_attr_update_float;
        msg->agent_id = player->agent_id;
        msg->attr_id = 43;
        msg->value = 0.033f;
        GameConnection_SendMessage(conn, buffer, sizeof(*msg));
    }
}

void GameSrv_SendUnlockedMaps(GameConnection *conn, GmPlayer *player)
{
    DbCharacter *ch = &player->character;

    GameSrvMsg *buffer = GameConnection_BuildMsg(conn, GAME_SMSG_UNLOCKED_MAPS);
    GameSrv_UnlockedMaps *msg = &buffer->unlocked_maps;

    STATIC_ASSERT(sizeof(msg->completed_missions_nm_buf) <= sizeof(ch->completed_missions_nm.buf));
    copy_u32_safe_or_abort(msg->completed_missions_nm_buf, sizeof(msg->completed_missions_nm_buf), ch->completed_missions_nm.buf, ch->completed_missions_nm.len);
    msg->completed_missions_nm_len = (uint32_t) ch->completed_missions_nm.len;

    STATIC_ASSERT(sizeof(msg->completed_bonuses_nm_buf) <= sizeof(ch->completed_bonuses_nm.buf));
    copy_u32_safe_or_abort(msg->completed_bonuses_nm_buf, sizeof(msg->completed_bonuses_nm_buf), ch->completed_bonuses_nm.buf, ch->completed_bonuses_nm.len);
    msg->completed_bonuses_nm_len = (uint32_t) ch->completed_bonuses_nm.len;

    STATIC_ASSERT(sizeof(msg->completed_missions_hm_buf) <= sizeof(ch->completed_missions_hm.buf));
    copy_u32_safe_or_abort(msg->completed_missions_hm_buf, sizeof(msg->completed_missions_hm_buf), ch->completed_missions_hm.buf, ch->completed_missions_hm.len);
    msg->completed_missions_hm_len = (uint32_t) ch->completed_missions_hm.len;

    STATIC_ASSERT(sizeof(msg->completed_bonuses_hm_buf) <= sizeof(ch->completed_bonuses_hm.buf));
    copy_u32_safe_or_abort(msg->completed_bonuses_hm_buf, sizeof(msg->completed_bonuses_hm_buf), ch->completed_bonuses_hm.buf, ch->completed_bonuses_hm.len);
    msg->completed_bonuses_hm_len = (uint32_t) ch->completed_bonuses_hm.len;

    STATIC_ASSERT(sizeof(msg->unlocked_maps_buf) <= sizeof(ch->unlocked_maps.buf));
    copy_u32_safe_or_abort(msg->unlocked_maps_buf, sizeof(msg->unlocked_maps_buf), ch->unlocked_maps.buf, ch->unlocked_maps.len);
    msg->unlocked_maps_len = (uint32_t) ch->unlocked_maps.len;

    GameConnection_SendMessage(conn, buffer, sizeof(*msg));
}

void GameSrv_SendUnlockedSkills(GameConnection *conn, GmPlayer *player)
{
    DbCharacter *ch = &player->character;

    GameSrvMsg *buffer = GameConnection_BuildMsg(conn, GAME_SMSG_UNLOCKED_SKILLS);
    GameSrv_UnlockedSkills *msg = &buffer->unlocked_skills;

    STATIC_ASSERT(sizeof(msg->unlocked_skills_buf) <= sizeof(ch->unlocked_skills.buf));
    copy_u32_safe_or_abort(msg->unlocked_skills_buf, sizeof(msg->unlocked_skills_buf), ch->unlocked_skills.buf, ch->unlocked_skills.len);
    msg->unlocked_skills_len = (uint32_t) ch->unlocked_skills.len;

    GameConnection_SendMessage(conn, buffer, sizeof(*msg));
}
