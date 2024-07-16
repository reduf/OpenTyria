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
}

GameConnection* GameSrv_GetConnection(GameSrv *srv, uintptr_t token)
{
    ptrdiff_t idx;
    if ((idx = stbds_hmgeti(srv->connections, token)) == -1) {
        return NULL;
    }
    return &srv->connections[(size_t)idx].value;
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

void GameSrv_Update(GameSrv *srv)
{
    GameSrv_ProcessInternalMessages(srv);
    GameSrv_Poll(srv);

    for (size_t idx = 0; idx < srv->events.size; ++idx) {
        Event event = array_at(&srv->events, idx);
        GameSrv_ProcessEvent(srv, event);
        array_add(&srv->connections_with_event, event.token);
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
