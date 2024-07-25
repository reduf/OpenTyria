#pragma once

#define GAME_READ_CHUNK_SIZE 4096

int GameSrv_Setup(GameSrv *srv)
{
    int err;
    if ((err = iocp_setup(&srv->iocp)) != 0) {
        return err;
    }

    if ((err = sys_mutex_init(&srv->mtx)) != 0) {
        return err;
    }

    return ERR_OK;
}

void GameSrv_Free(GameSrv *srv)
{
    iocp_free(&srv->iocp);
    stbds_hmfree(srv->connections);
    array_free(&srv->connections_with_event);
    array_free(&srv->connections_to_remove);
    array_free(&srv->events);
    sys_mutex_free(&srv->mtx);
    array_free(&srv->admin_messages);
    array_free(&srv->clients);
    array_free(&srv->players);
}

GameConnection* GameSrv_GetConnection(GameSrv *srv, uintptr_t token)
{
    ptrdiff_t idx;
    if ((idx = stbds_hmgeti(srv->connections, token)) == -1) {
        return NULL;
    }
    return &srv->connections[(size_t)idx].value;
}

int GameConnection_FlushOutgoingBuffer(GameConnection *conn)
{
    int err;
    size_t bytes_sent;
    if ((err = sys_send(conn->source.socket, conn->outgoing.data, conn->outgoing.size, &bytes_sent)) != 0) {
        if (sys_would_block(err)) {
            conn->writable = false;
            return ERR_OK;
        }

        log_error("Failed to send %zu bytes, err: %d", bytes_sent, err);
        return ERR_UNSUCCESSFUL;
    }

    if (bytes_sent != conn->outgoing.size) {
        conn->writable = false;
        array_remove_range_ordered(&conn->outgoing, 0, bytes_sent);
    } else {
        array_clear(&conn->outgoing);
    }

    return ERR_OK;
}

int GameConnection_SendMessage(GameConnection *conn, GameSrvMsg *msg, size_t size)
{
    int err;

    assert(msg->header < ARRAY_SIZE(GAME_SMSG_FORMATS));
    MsgFormat format = GAME_SMSG_FORMATS[msg->header];

    size_t size_before = array_size(&conn->outgoing);

    uint8_t *dst;
    if ((dst = array_push(&conn->outgoing, MSG_MAX_BUFFER_SIZE)) == NULL) {
        return ERR_OUT_OF_MEMORY;
    }

    size_t written;
    if ((err = pack_msg(format, &written, msg->buffer, size, dst, MSG_MAX_BUFFER_SIZE)) != 0) {
        array_shrink(&conn->outgoing, size_before);
        return err;
    }

    array_shrink(&conn->outgoing, size_before + written);
    arc4_crypt_inplace(&conn->cipher_enc, dst, written);
    return ERR_OK;
}

GameSrvMsg* GameConnection_BuildMsg(GameConnection *conn, uint16_t header)
{
    memset(&conn->srv_msg, 0, sizeof(conn->srv_msg));
    conn->srv_msg.header = header;
    return &conn->srv_msg;
}

void GameSrv_SendInstanceHead(GameConnection *conn)
{
    GameSrvMsg *srv_msg = GameConnection_BuildMsg(conn, GAME_SMSG_INSTANCE_LOAD_HEAD);
    GameSrv_InstanceHead *msg = &srv_msg->instance_head;
    // msg->b1 = ?;
    // msg->b2 = ?;
    // msg->b3 = ?;
    // msg->b4 = ?;
    GameConnection_SendMessage(conn, srv_msg, sizeof(*msg));
}

void GameSrv_SendCharacterCreationStart(GameConnection *conn)
{
    GameSrvMsg *msg = GameConnection_BuildMsg(conn, GAME_SMSG_INSTANCE_CHAR_CREATION_START);
    GameConnection_SendMessage(conn, msg, sizeof(msg->header));
}

void GameSrv_SendCharacterCreationReady(GameConnection *conn)
{
    GameSrvMsg *msg = GameConnection_BuildMsg(conn, GAME_SMSG_INSTANCE_CHAR_CREATION_READY);
    GameConnection_SendMessage(conn, msg, sizeof(msg->header));
}

void GameSrv_SendInstanceEarlyPacket(GameConnection *conn)
{
    GameSrvMsg *msg = GameConnection_BuildMsg(conn, GAME_SMSG_INSTANCE_EARLY_PACKET);
    GameConnection_SendMessage(conn, msg, sizeof(msg->header));
}

void GameSrv_SendInstanceLoadPlayerName(GameConnection *conn)
{
    GameSrvMsg *srv_msg = GameConnection_BuildMsg(conn, GAME_SMSG_INSTANCE_LOAD_PLAYER_NAME);
    GameSrv_InstancePlayerName *msg = &srv_msg->instance_player_name;
    // size_t   n_name;
    // uint16_t name[20];
    GameConnection_SendMessage(conn, srv_msg, sizeof(*msg));
}

void GameSrv_SendInstanceLoadInfo(GameSrv *srv, GameConnection *conn)
{
    GameSrvMsg *srv_msg = GameConnection_BuildMsg(conn, GAME_SMSG_INSTANCE_LOAD_INFO);
    GameSrv_InstanceInfo *msg = &srv_msg->instance_info;
    msg->agent = 0; // Should be the agent id of the player
    msg->map_id = srv->map_id;
    msg->is_explorable = 1; // what to put here?
    msg->district = srv->district_number;
    msg->language = DistrictLanguage_ToInt(srv->language);
    msg->is_observer = 0;
    GameConnection_SendMessage(conn, srv_msg, sizeof(*msg));
}

void GameSrv_SendItemStreamCreate(GameConnection *conn)
{
    GameSrvMsg *srv_msg = GameConnection_BuildMsg(conn, GAME_SMSG_ITEM_STREAM_CREATE);
    GameSrv_ItemStreamCreate *msg = &srv_msg->item_stream_create;
    // uint16_t stream_id;
    // uint8_t  is_hero;
    GameConnection_SendMessage(conn, srv_msg, sizeof(*msg));
}

void GameSrv_SendGoldStorageAdd(GameConnection *conn)
{
    GameSrvMsg *srv_msg = GameConnection_BuildMsg(conn, GAME_SMSG_GOLD_STORAGE_ADD);
    GameSrv_UpdateGold *msg = &srv_msg->update_gold;
    // uint16_t stream;
    // uint32_t gold;
    GameConnection_SendMessage(conn, srv_msg, sizeof(*msg));
}

void GameSrv_SendReadyForMapSpawn(GameConnection *conn)
{
    GameSrvMsg *srv_msg = GameConnection_BuildMsg(conn, GAME_SMSG_READY_FOR_MAP_SPAWN);
    GameSrv_ReadyForMapSpawn *msg = &srv_msg->ready_for_map_spawn;
    GameConnection_SendMessage(conn, srv_msg, sizeof(*msg));
}

void GameSrv_GetMessages(GameSrv *srv, GameConnection *conn)
{
    int err;
    uint16_t header;
    size_t total_consumed = 0;

    while (sizeof(header) <= (conn->incoming.size - total_consumed)) {
        const uint8_t *input = &conn->incoming.data[total_consumed];
        size_t size = conn->incoming.size - total_consumed;

        header = le16dec(input) & ~AUTH_CMSG_MASK;
        if (ARRAY_SIZE(AUTH_CMSG_FORMATS) <= header) {
            IoSource_free(&conn->source);
            array_add(&srv->connections_to_remove, conn->token);
            break;
        }

        MsgFormat format = AUTH_CMSG_FORMATS[header];

        size_t consumed;

        GamePlayerMsg *msg = array_push(&srv->player_messages, 1);
        msg->player_id = conn->player_id;

        if ((err = unpack_msg(format, &consumed, input, size, msg->msg.buffer, sizeof(msg->msg.buffer))) != 0) {
            array_pop(&srv->player_messages);

            if (err != ERR_NOT_ENOUGH_DATA) {
                log_warn("Received invalid message from client %04" PRIXPTR, conn->token);
                IoSource_free(&conn->source);
                array_add(&srv->connections_to_remove, conn->token);
            }

            break;
        }

        total_consumed += consumed;
    }

    array_remove_range_ordered(&conn->incoming, 0, total_consumed);
}

void GameSrv_ProcessEvent(GameSrv *srv, Event event)
{
    UNREFERENCED_PARAMETER(srv);

    int err;

    GameConnection *conn;
    if ((conn = GameSrv_GetConnection(srv, event.token)) == NULL) {
        return;
    }

    if ((event.flags & IOCPF_WRITE) != 0) {
        conn->writable = true;
    }

    if ((event.flags & IOCPF_READ) != 0) {
        for (;;) {
            uint8_t *buffer;

            const size_t size_at_starts = conn->incoming.size;
            if ((buffer = array_push(&conn->incoming, GAME_READ_CHUNK_SIZE)) == NULL) {
                log_error("Out of memory will reading from socket");
                break;
            }

            size_t bytes;
            if ((err = sys_recv(conn->source.socket, buffer, GAME_READ_CHUNK_SIZE, &bytes)) != 0) {
                array_shrink(&conn->incoming, size_at_starts);
                if (!sys_would_block(err)) {
                    log_error(
                        "Error while reading %04" PRIXPTR ", err: %d",
                        conn->source.socket,
                        err
                    );

                    // GameSrv_PeerDisconnected(srv, conn);
                }

                break;
            }

            uint8_t *ptr = &array_at(&conn->incoming, size_at_starts);
            arc4_crypt_inplace(&conn->cipher_dec, ptr, bytes);
            array_shrink(&conn->incoming, size_at_starts + bytes);

            if (bytes == 0) {
                // GameSrv_PeerDisconnected(srv, conn);
                break;
            }
        }

        GameSrv_GetMessages(srv, conn);
    }
}

void GameSrv_Poll(GameSrv *srv)
{
    int err;

    array_clear(&srv->events);
    while ((err = iocp_poll(&srv->iocp, &srv->events, 16)) != 0) {
        if (err == ERR_TIMEOUT) {
            break;
        }
        log_error("Failed to run 'iocp_poll', err: %d", err);
        abort();
    }
}

void GameSrv_CreatePlayer(GameSrv *srv, AdminMsg_TransferUser *msg, size_t *result)
{
    size_t player_idx;
    if (srv->player_count == array_size(&srv->players)) {
        (void) array_push(&srv->players, 1);
        player_idx = array_size(&srv->players) - 1;
    } else {
        for (player_idx = 0; player_idx < srv->players.size; ++player_idx) {
            if (srv->players.data[player_idx].player_id == 0) {
                break;
            }
        }
    }

    GamePlayer *player = &srv->players.data[player_idx];
    player->player_id = (uint32_t) (player_idx + 1);
    player->conn_token = msg->token;
    player->account_id = msg->account_id;
    player->char_id = msg->char_id;

    ++srv->player_count;
    *result = player->player_id;
}

void GameSrv_HandleTransferUserCmd(GameSrv *srv, AdminMsg_TransferUser *msg)
{
    GameConnection conn = {0};
    conn.token = msg->token;
    conn.source = msg->source;
    arc4_setup(&conn.cipher_enc, msg->cipher_init_key, sizeof(msg->cipher_init_key));
    conn.cipher_dec = conn.cipher_enc;

    int err;
    if ((err = iocp_register(&srv->iocp, &conn.source, conn.token, IOCPF_READ | IOCPF_WRITE)) != 0) {
        log_error("Failed to register a transfered user");
    }

    if (!msg->reconnection) {
        GameSrv_CreatePlayer(srv, msg, &conn.player_id);
    } else {
        abort();
    }

    switch (srv->map_type) {
    case MapType_CharacterCreation:
        GameSrv_SendInstanceHead(&conn);
        GameSrv_SendCharacterCreationStart(&conn);
        GameSrv_SendInstanceEarlyPacket(&conn);
        GameSrv_SendCharacterCreationReady(&conn);
        break;
    case MapType_MainTown:
    case MapType_MainExplorable:
    case MapType_GuildHall:
    case MapType_MissionOutpost:
    case MapType_MissionExplorable:
    case MapType_ArenaOutpost:
    case MapType_ArenaExplorable:
    case MapType_HeroesAscentOutpost:
        GameSrv_SendInstanceHead(&conn);
        GameSrv_SendInstanceEarlyPacket(&conn);
        GameSrv_SendInstanceLoadPlayerName(&conn);
        GameSrv_SendInstanceLoadInfo(srv, &conn);
        break;
    default:
        abort();
    }

    // @TODO: Send all initial packets?
    stbds_hmput(srv->connections, conn.token, conn);
}

void GameSrv_ProcessInternalMessages(GameSrv *srv)
{
    sys_mutex_lock(&srv->mtx);
    AdminMsgArray messages = srv->admin_messages;
    memset(&srv->admin_messages, 0, sizeof(srv->admin_messages));
    sys_mutex_unlock(&srv->mtx);

    for (size_t idx = 0; idx < messages.size; ++idx) {
        AdminMsg *msg = &messages.data[idx];
        switch (msg->cmd) {
        case AdminCmd_Quit:
            srv->quit_signaled = true;
            break;
        case AdminCmd_TransferUser:
            GameSrv_HandleTransferUserCmd(srv, &msg->transfer_user);
            break;
        }
    }
}

int GameSrv_HandleInstanceLoadRequestSpawn(GameSrv *srv)
{
    UNREFERENCED_PARAMETER(srv);
    // GAME_SMSG_INSTANCE_LOAD_SPAWN_POINT
    return ERR_OK;
}

int GameSrv_HandleInstanceLoadRequestPlayers(GameSrv *srv, size_t player_id, GameSrv_RequestPlayers *msg)
{
    UNREFERENCED_PARAMETER(srv);
    UNREFERENCED_PARAMETER(player_id);
    UNREFERENCED_PARAMETER(msg);
    // GAME_SMSG_ACCOUNT_CURRENCY
    // ...
    // GAME_SMSG_MAPS_UNLOCKED
    // GAME_SMSG_VANQUISH_PROGRESS
    // GameSrv_SendInstanceLoaded(&conn)
    // Send other stuff in the map
    return ERR_OK;
}

int GameSrv_HandleInstanceLoadRequestItems(GameSrv *srv, size_t player_id, GameSrv_RequestItems *msg)
{
    UNREFERENCED_PARAMETER(srv);
    UNREFERENCED_PARAMETER(player_id);
    UNREFERENCED_PARAMETER(msg);
    // GameSrv_SendItemStreamCreate(&conn);
    // Send all items and location GAME_SMSG_ITEM_GENERAL_INFO & GAME_SMSG_INVENTORY_ITEM_LOCATION
    // Send all carried item GAME_SMSG_ITEM_WEAPON_SET
    // GameSrv_SendGoldStorageAdd(&conn);
    // Send GAME_SMSG_QUEST_ADD
    // Send GAME_SMSG_PLAYER_ATTR_MAX_KURZICK
    // Send GAME_SMSG_PLAYER_ATTR_MAX_LUXON
    // Send GAME_SMSG_PLAYER_ATTR_MAX_BALTHAZAR
    // Send GAME_SMSG_PLAYER_ATTR_MAX_IMPERIAL
    // GameSrv_SendReadyForMapSpawn(&conn);
    return ERR_OK;
}

int GameSrv_ProcessPlayerMessage(GameSrv *srv, size_t player_id, GameCliMsg *msg)
{
    int err;
    switch (msg->header) {
    case GAME_CMSG_INSTANCE_LOAD_REQUEST_SPAWN:
        err = GameSrv_HandleInstanceLoadRequestSpawn(srv);
        break;
    case GAME_CMSG_INSTANCE_LOAD_REQUEST_PLAYERS:
        err = GameSrv_HandleInstanceLoadRequestPlayers(srv, player_id, &msg->request_players);
        break;
    case GAME_CMSG_INSTANCE_LOAD_REQUEST_ITEMS:
        err = GameSrv_HandleInstanceLoadRequestItems(srv, player_id, &msg->request_items);
        break;
    default:
        log_warn(
            "Unhandled GameSrv packet with header %" PRIu16 " (0x%" PRIX16 ")",
            msg->header & ~GAME_CMSG_MASK,
            msg->header & ~GAME_CMSG_MASK
        );
        err = ERR_BAD_USER_DATA;
    }
    return err;
}

void GameSrv_Update(GameSrv *srv)
{
    int err;

    GameSrv_ProcessInternalMessages(srv);
    GameSrv_Poll(srv);

    for (size_t idx = 0; idx < srv->events.size; ++idx) {
        Event event = array_at(&srv->events, idx);
        GameSrv_ProcessEvent(srv, event);
        array_add(&srv->connections_with_event, event.token);
    }

    GamePlayerMsgArray msgs = srv->player_messages;
    for (size_t idx = 0; idx < msgs.size; ++idx) {
        GameSrv_ProcessPlayerMessage(srv, msgs.data[idx].player_id, &msgs.data[idx].msg);
    }
    array_clear(&msgs);

    size_t n_connections = stbds_hmlen(srv->connections);
    for (size_t idx = 0; idx < n_connections; ++idx) {
        GameConnection *conn = &srv->connections[idx].value;
        if (array_empty(&conn->outgoing)) {
            continue;
        }

        if ((err = GameConnection_FlushOutgoingBuffer(conn)) != 0) {
            array_add(&srv->connections_to_remove, conn->token);
        }
    }

    for (size_t idx = 0; idx < srv->connections_to_remove.size; ++idx) {
        uintptr_t token = array_at(&srv->connections_to_remove, idx);
        stbds_hmdel(srv->connections, token);
    }

    array_clear(&srv->connections_to_remove);
}

void GameSrv_ThreadEntry(void *param)
{
    GameSrv *srv = (GameSrv *)param;
    while (!srv->quit_signaled) {
        GameSrv_Update(srv);
    }
}

int GameSrv_Start(GameSrv *srv)
{
    int err;
    if ((err = sys_thread_create(&srv->thread, GameSrv_ThreadEntry, srv)) != 0) {
        return err;
    }
    return ERR_OK;
}

void GameSrv_SendAdminMsg(GameSrv *srv, AdminMsg *msg)
{
    sys_mutex_lock(&srv->mtx);
    AdminMsg *dst = array_push(&srv->admin_messages, 1);
    *dst = *msg;
    sys_mutex_unlock(&srv->mtx);
}
